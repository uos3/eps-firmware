using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    class RegisterModel : IRegisterMonitor, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private string _regName = "00 - Config";
        public string RegName
        {
            get
            {
                return _regName;
            }
            private set
            {
                if (_regName == value)
                    return;

                _regName = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(RegName)));
            }
        }

        private string _regConvert = "";
        public string RegConvert
        {
            get
            {
                return _regConvert;
            }
            private set
            {
                if (_regConvert == value)
                    return;

                _regConvert = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(RegConvert)));
            }
        }

        private Register _regId = Register.Config;
        public Register RegId
        {
            get
            {
                return _regId;
            }
            set
            {
                if (_regId == value)
                    return;

                _regId = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(RegId)));

                RegName = $"{(int)value:X2} - {value}";
            }
        }

        private string _regValue = "0x0000";
        public string RegValue
        {
            get
            {
                return _regValue;
            }
            set
            {
                if (_regValue == value)
                    return;

                _regValue = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(RegValue)));
            }
        }

        private int _regValueInt = 0;
        private int RegValueInt
        {
            get { return _regValueInt; }
            set
            {
                if (_regValueInt == value)
                    return;

                _regValueInt = value;

                Convert(value);

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(RegValueInt)));

                RegValue = $"0x{value:X4}";

                for (int i = 0; i < 15; i++)
                    B[i] = (value & (1<<i)) != 0;

                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(B)));
            }
        }
        

        private bool[] _b = new bool[16];
        public bool[] B
        {
            get { return _b; }
            set
            {
                if (_b == value)
                    return;

                _b = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(B)));
            }
        }

        private void Convert(int input)
        {
            RegConvert = MeasurementConvert.Convert(RegId, input);
        }

        public void RegisterUpdated(Register reg, ushort value)
        {
            if (reg == RegId)
                RegValueInt = value;
        }
    }
}
