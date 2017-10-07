using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    static class MeasurementConvert
    {
        static Dictionary<Register, ConvertInfo> convertInfos = new Dictionary<Register, ConvertInfo>()
        {
            { Register.Config,     new ConvertInfo(ConvertType.Bitfield, 1) },
            { Register.Status,     new ConvertInfo(ConvertType.Bitfield, 1) },
            { Register.SW_On,      new ConvertInfo(ConvertType.Bitfield, 1) },
            { Register.Power,      new ConvertInfo(ConvertType.Bitfield, 1) },
            { Register.Bat_V,      new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Bat_I,      new ConvertInfo(ConvertType.Current,  1) },
            { Register.Bat_T,      new ConvertInfo(ConvertType.Number,   1) },
            { Register.Solar_N1_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_N2_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_S1_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_S2_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_E1_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_E2_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_W1_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_W2_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_T1_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Solar_T2_I, new ConvertInfo(ConvertType.Current,  0.042) },
            { Register.Reserved,   new ConvertInfo(ConvertType.Number,   1) },
            { Register.Solar_2_V,  new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail1_Boot, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail1_Fail, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail1_V,    new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail1_I,    new ConvertInfo(ConvertType.Current,  0.250) },
            { Register.Rail2_Boot, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail2_Fail, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail2_V,    new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail2_I,    new ConvertInfo(ConvertType.Current,  0.038) },
            { Register.Rail3_Boot, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail3_Fail, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail3_V,    new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail3_I,    new ConvertInfo(ConvertType.Current,  0.114) },
            { Register.Rail4_Boot, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail4_Fail, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail4_V,    new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail4_I,    new ConvertInfo(ConvertType.Current,  0.450) },
            { Register.Rail5_Boot, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail5_Fail, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail5_V,    new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail5_I,    new ConvertInfo(ConvertType.Current,  0.045) },
            { Register.Rail6_Boot, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail6_Fail, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Rail6_V,    new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Rail6_I,    new ConvertInfo(ConvertType.Current,  0.114) },
            { Register.Supply_3_V, new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Supply_3_I, new ConvertInfo(ConvertType.Current,  0.250) },
            { Register.Supply_5_V, new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Supply_5_I, new ConvertInfo(ConvertType.Current,  0.125) },
            { Register.Charge_I,   new ConvertInfo(ConvertType.Current,  0.250) },
            { Register.Mptt_Bus_V, new ConvertInfo(ConvertType.Voltage,  100.0 / (100.0 + 310.0)) },
            { Register.Crc_Er_Cnt, new ConvertInfo(ConvertType.Number,   1) },
            { Register.Drop_Cnt,   new ConvertInfo(ConvertType.Number,   1) },
        };

        private static string ConvertNumber(Register reg, double value)
        {
            return value.ToString();
        }

        private static string ConvertCurrent(Register reg, double value)
        {
            return (value * 2.5 / 0x3FF / 100 * 1000).ToString("0") + " mA";
        }

        private static string ConvertVoltage(Register reg, double value)
        {
            return (value * 2.5 / 0x3FF * 1000).ToString("0") + " mV";
        }

        private static string ConvertBitfield(Register reg, double value)
        {
            return "TODO";
        }

        public static string Convert(Register reg, int value)
        {
            ConvertInfo info;
            if (convertInfos.TryGetValue(reg, out info))
            {
                var converted = value / info.Factor;
                switch (info.Type)
                {
                    case ConvertType.Number:
                        return ConvertNumber(reg, converted);
                    case ConvertType.Current:
                        return ConvertCurrent(reg, converted);
                    case ConvertType.Voltage:
                        return ConvertVoltage(reg, converted);
                    case ConvertType.Bitfield:
                        return ConvertBitfield(reg, converted);
                    default:
                        return "Unknown";
                }
            }
            else
                return "-";
        }

        struct ConvertInfo
        {
            public ConvertType Type;
            public double Factor;

            public ConvertInfo(ConvertType type, double factor)
            {
                Type = type;
                Factor = factor;
            }
        }

        enum ConvertType
        {
            Bitfield,
            Current,
            Voltage,
            Number
        }
    }
}
