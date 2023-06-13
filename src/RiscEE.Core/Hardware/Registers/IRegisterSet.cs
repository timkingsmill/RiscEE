using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Hardware.Registers
{
    public interface IRegisterSet
    {
        public UInt32 Read(RegisterID registerID);
        void Write(RegisterID registerID, UInt32 value);

    }
}
