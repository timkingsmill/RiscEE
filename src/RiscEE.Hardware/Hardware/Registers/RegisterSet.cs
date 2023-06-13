using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Hardware.Registers
{

    public class RegisterSet : IRegisterSet
    {
        UInt32 IRegisterSet.Read(RegisterID registerID)
        {
            throw new NotImplementedException();
        }

        void IRegisterSet.Write(RegisterID registerID, UInt32 value)
        {
            throw new NotImplementedException();
        }
    }
}
