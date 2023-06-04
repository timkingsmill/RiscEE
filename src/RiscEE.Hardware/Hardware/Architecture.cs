using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;

using Kingsmill.Net.Enumerations;
using Microsoft.VisualBasic;

namespace RiscEE.Hardware
{
    public class Architecture : Enumeration
    {
        public static Architecture Unknown => new(0, nameof(Unknown));
        public static Architecture Rv32I => new(1, nameof(Rv32I));
        public static Architecture Rv32E => new(2, nameof(Rv32E));
        public static Architecture Rv64I => new(3, nameof(Rv64I));
        public static Architecture Custom => new(4, nameof(Custom));

        public Architecture(int id, string name)
            : base(id, name)
        {
        }
    }
}
