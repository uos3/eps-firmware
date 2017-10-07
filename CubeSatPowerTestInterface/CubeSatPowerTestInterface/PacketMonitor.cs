using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    public delegate void DutPacketReceivedEventHandler(PacketMonitor sender, byte[] packet);

    public class PacketMonitor
    {
        Queue<byte> packetData = new Queue<byte>();
        public event DutPacketReceivedEventHandler DutPacketReceived;

        public PacketMonitor(SerialLayer layer)
        {
            layer.PacketReceived += Serial_PacketReceived;
        }

        public void Serial_PacketReceived(SerialLayer serial, byte[] packet)
        {
            int packetLen = packet[1];
            switch (packet[0])
            {
                case 0x80:
                    for (int i = 0; i < packetLen; i++)
                        packetData.Enqueue(packet[i + 2]);
                    break;
            }

            while (packetData.Count >= 2)
            {
                int length = 4 + packetData.Skip(1).First() * 2;
                if (packetData.Count < length)
                    break;

                byte[] packetOut = new byte[length];
                for (int i = 0; i < packetOut.Length; i++)
                    packetOut[i] = packetData.Dequeue();

                var handler = DutPacketReceived;
                handler?.Invoke(this, packetOut);
            }
        }
    }
}
