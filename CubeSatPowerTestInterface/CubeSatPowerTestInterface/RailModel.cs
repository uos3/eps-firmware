using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    delegate void EnabledChangedEventHandler(RailModel sender, bool value);

    class RailModel : IRegisterMonitor, INotifyPropertyChanged
    {
        public event EnabledChangedEventHandler EnabledChanged;
        public event PropertyChangedEventHandler PropertyChanged;

        public int Id
        {
            get;
        }

        public string Name
        {
            get;
        }

        private bool _enabled;
        public bool Enabled
        {
            get
            {
                return _enabled;
            }
            set
            {
                if (_enabled == value)
                    return;

                _enabled = value;

                var enabledHandler = EnabledChanged;
                enabledHandler?.Invoke(this, value);

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(Enabled)));
            }
        }

        private bool _powered;
        public bool Powered
        {
            get
            {
                return _powered;
            }
            private set
            {
                if (_powered == value)
                    return;

                _powered = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(Powered)));
            }
        }

        private string _voltage;
        public string Voltage
        {
            get
            {
                return _voltage;
            }
            private set
            {
                if (_voltage == value)
                    return;

                _voltage = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(Voltage)));
            }
        }

        private string _current;
        public string Current
        {
            get
            {
                return _current;
            }
            private set
            {
                if (_current == value)
                    return;

                _current = value;

                var changedHandler = PropertyChanged;
                changedHandler?.Invoke(this, new PropertyChangedEventArgs(nameof(Current)));
            }
        }

        public RailModel(int id, string name)
        {
            Id = id;
            Name = name;
        }

        public void RegisterUpdated(Register reg, ushort value)
        {
            switch (reg)
            {
                case Register.SW_On:
                    Enabled = (value & (1 << Id)) != 0;
                    break;
                case Register.Power:
                    Powered = (value & (1 << Id)) != 0;
                    break;
            }
            int offsetBase = reg - Register.Rail1_Boot;
            int railIndex = offsetBase / 4;
            if (offsetBase >= 0 && railIndex == Id)
            {
                int offset = offsetBase % 4;
                if (offset == 0) // Boots
                {

                }
                else if (offset == 1) // Fails
                {

                }
                else if (offset == 2) // Voltage
                    Voltage = MeasurementConvert.Convert(reg, value);
                else if (offset == 3) // Current
                    Current = MeasurementConvert.Convert(reg, value);
            }
        }
    }
}
