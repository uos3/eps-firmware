using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CubeSatPowerTestInterface
{
    interface IRegisterMonitor
    {
        void RegisterUpdated(Register reg, ushort value);
    }
}
