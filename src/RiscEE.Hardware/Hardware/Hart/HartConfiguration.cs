using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Hardware.Hart
{
    public abstract class HartConfiguration
    {
        public HartConfiguration()
        {
            Architecture = Architecture.Unknown;
        }

        public Architecture Architecture { get; protected set; }

    }
}
