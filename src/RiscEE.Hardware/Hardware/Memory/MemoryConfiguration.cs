using Kingsmill.Net.Enumerations;
using RiscEE.Hardware.Registers;

namespace RiscEE.Hardware.Memory
{
    public class MemoryConfiguration : Enumeration
    {
        protected MemoryConfiguration(int id, string name)
            : base(id, name)
        {
        }

        public static readonly MemoryConfiguration Unknown = new(0, nameof(Unknown));
        public static readonly MemoryConfiguration Dynamic = new(1, nameof(Dynamic));
        public static readonly MemoryConfiguration Fixed = new(2, nameof(Fixed));

    }
}
