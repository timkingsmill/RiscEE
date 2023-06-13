using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Hardware.Hart
{
    public class Rv32IHartConfiguration : HartConfiguration
    {
        public Rv32IHartConfiguration() 
        {
            Architecture = Architecture.Rv32I;
        }
    }
}
