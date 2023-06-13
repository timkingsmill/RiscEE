using System;
using System.Collections;
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
        public static readonly Architecture Unknown = new(0, nameof(Unknown));
        public static readonly Architecture Rv32I = new(1, nameof(Rv32I));
        public static readonly Architecture Rv32E = new(2, nameof(Rv32E));
        public static readonly Architecture Rv64I = new(3, nameof(Rv64I));
        public static readonly Architecture Custom = new(4, nameof(Custom));

        protected Architecture(int id, string name)
            : base(id, name)
        {
        }

    }
}
