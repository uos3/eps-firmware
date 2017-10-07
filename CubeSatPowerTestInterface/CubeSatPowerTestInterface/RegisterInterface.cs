using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    class RegisterInterface
    {
        private List<IRegisterMonitor> _monitors = new List<IRegisterMonitor>();
        private SerialLayer _physicalLayer;

        public RegisterInterface(SerialLayer layer, PacketMonitor monitor)
        {
            _physicalLayer = layer;
            monitor.DutPacketReceived += Monitor_DutPacketReceived;
        }

        private void Monitor_DutPacketReceived(PacketMonitor sender, byte[] packet)
        {
            var res = ParseResponse(packet);
            if (res != null)
                NotifyMonitors(res);
        }

        public void Set(Register reg, ushort value)
        {
            int regId = (int)reg;
            byte[] packet = new byte[5]
            {
                (byte)((1 << 7) | (regId & 0x7F)),
                (byte)(value & 0xFF),
                (byte)((value >> 8) & 0xFF),
                0,
                0
            };
            ushort crc = GetCrc(packet, 0, 3);
            packet[3] = (byte)(crc & 0xFF);
            packet[4] = (byte)((crc >> 8) & 0xFF);
            _physicalLayer.Flush();
            _physicalLayer.WritePacket(new byte[] { 0, (byte)packet.Length }.Concat(packet).ToArray());
        }

        public void AddMonitor(IRegisterMonitor mon)
        {
            _monitors.Add(mon);
        }

        public void Get(Register reg, int count)
        {
            int regId = (int)reg;
            byte[] packet = new byte[5]
            {
                (byte)((0 << 7) | (regId & 0x7F)),
                (byte)(count & 0x7F),
                0,
                0,
                0
            };
            ushort crc = GetCrc(packet, 0, 3);
            packet[3] = (byte)(crc & 0xFF);
            packet[4] = (byte)((crc >> 8) & 0xFF);
            _physicalLayer.Flush();
            _physicalLayer.WritePacket(new byte[] { 0, (byte)packet.Length }.Concat(packet).ToArray());
        }

        void NotifyMonitors(Dictionary<int, ushort> registers)
        {
            foreach (var regKvp in registers)
                foreach (var monitor in _monitors)
                    monitor.RegisterUpdated((Register)regKvp.Key, regKvp.Value);
        }

        Dictionary<int, ushort> ParseResponse(byte[] packet)
        {
            bool interrupt = (packet[0] & 0x80) != 0;
            int regStart = packet[0] & 0x7F;
            int noReg = packet[1] & 0x7F;
            var regs = new Dictionary<int, ushort>();
            for (int i = 0; i < noReg; i++)
            {
                ushort val = (ushort)(packet[2 + i * 2] | (packet[3 + i * 2] << 8));
                regs.Add(regStart + i, val);
            }
            int crc = packet[2 + 2 * noReg] | (packet[3 + 2 * noReg] << 8);
            int calcCrc = GetCrc(packet, 0, packet.Length - 2);
            if (crc != calcCrc)
            {
                return null;
            }
            return regs;
        }

        private static ushort GetCrc(byte[] message, int offset, int length)
        {
            ushort crcFull = 0xFFFF;

            for (var i = offset; i < length; i++)
            {
                crcFull = (ushort)(crcFull ^ message[i]);

                for (var j = 0; j < 8; j++)
                {
                    var crclsb = (char)(crcFull & 0x0001);
                    crcFull = (ushort)((crcFull >> 1) & 0x7FFF);

                    if (crclsb == 1)
                        crcFull = (ushort)(crcFull ^ 0xA001);
                }
            }
            return crcFull;
        }
    }
}
