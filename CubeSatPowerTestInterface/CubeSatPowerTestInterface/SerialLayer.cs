using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    public delegate void PacketEventHandler(SerialLayer serial, byte[] packet);
    public class SerialLayer : IDisposable
    {
        private SerialPort _port;
        private ConcurrentQueue<byte> _dataQueue;

        public event PacketEventHandler PacketReceived;
        public event PacketEventHandler PacketSent;

        public void Dispose()
        {
            ((IDisposable)_port).Dispose();
        }

        public bool Open(string port, int baudRate)
        {
            try
            {
                _dataQueue = new ConcurrentQueue<byte>();
                _port = new SerialPort(port)
                {
                    BaudRate = baudRate
                };
                _port.DataReceived += port_DataReceived;
                _port.Open();
                return true;
            }
            catch
            {
                return false;
            }
        }

        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var buff = new byte[_port.BytesToRead];
            Debug.Assert(buff.Length == _port.Read(buff, 0, buff.Length));
            foreach (var b in buff)
                _dataQueue.Enqueue(b);

            while (_dataQueue.Count >= 2)
            {
                var header = _dataQueue.Take(2).ToArray();
                int packetLen = header[1] + 2;
                if (_dataQueue.Count < packetLen) return;

                var packet = new byte[packetLen];
                for (int i = 0; i < packetLen; i++)
                {
                    byte b;
                    Debug.Assert(_dataQueue.TryDequeue(out b));
                    packet[i] = b;
                }

                var handler = PacketReceived;
                handler?.Invoke(this, packet);
            }
        }

        public void Flush()
        {
            _port.ReadExisting();
        }

        public void WritePacket(byte[] packet)
        {
            _port.Write(packet, 0, packet.Length);
            var handler = PacketSent;
            handler?.Invoke(this, packet);
        }

        Queue<byte> packetData = new Queue<byte>();

        private int GetPacketLength(IEnumerable<byte> packet, int length)
        {
            if (length < 2)
                return 256;

            return  2 + packet.Skip(1).First();
        }

        public byte[] ReadPacket(int length)
        {
            while (packetData.Count < length)
            {
                while (_dataQueue.Count < 2) ;

                var header = _dataQueue.Take(2).ToArray();
                int packetLen = header[1] + 2;
                while (_dataQueue.Count < packetLen) ;

                var buff = new byte[packetLen];
                for (int i = 0; i < packetLen; i++)
                {
                    byte b;
                    Debug.Assert(_dataQueue.TryDequeue(out b));
                    buff[i] = b;
                }

                switch (buff[0])
                {
                    case 0x80:
                        for (int i = 2; i < packetLen; i++)
                            packetData.Enqueue(buff[i]);
                        break;
                }
            }

            byte[] packet = new byte[length];
            for (int i = 0; i < packet.Length; i++)
                packet[i] = packetData.Dequeue();

            return packet;

            /*byte[] packet = new byte[length];
            int offset = 0;

            while (offset != packet.Length)
            {
                offset += _port.Read(packet, offset, packet.Length - offset);
            }
            return packet;*/
        }
    }
}
