using Kingsmill.Net.Enumerations;

namespace RiscEE.Hardware.Registers
{
    public class RegisterID : Enumeration
    {
        protected RegisterID(int id, string name)
            : base(id, name)
        {
        }

        public static readonly RegisterID x0 = new(0, nameof(x0));
        public static readonly RegisterID x1 = new(0, nameof(x1));
        public static readonly RegisterID x2 = new(0, nameof(x2));
        public static readonly RegisterID x3 = new(0, nameof(x3));
        public static readonly RegisterID x4 = new(0, nameof(x4));
        public static readonly RegisterID x5 = new(0, nameof(x5));
        public static readonly RegisterID x6 = new(0, nameof(x6));
        public static readonly RegisterID x7 = new(0, nameof(x7));
        public static readonly RegisterID x8 = new(0, nameof(x8));
        public static readonly RegisterID x9 = new(0, nameof(x9));
        public static readonly RegisterID x10 = new(0, nameof(x10));
        public static readonly RegisterID x11 = new(0, nameof(x11));
        public static readonly RegisterID x12 = new(0, nameof(x12));
        public static readonly RegisterID x13 = new(0, nameof(x13));
        public static readonly RegisterID x14 = new(0, nameof(x14));
        public static readonly RegisterID x15 = new(0, nameof(x15));
        public static readonly RegisterID x16 = new(0, nameof(x16));
        public static readonly RegisterID x17 = new(0, nameof(x17));
        public static readonly RegisterID x18 = new(0, nameof(x18));
        public static readonly RegisterID x19 = new(0, nameof(x19));
        public static readonly RegisterID x20 = new(0, nameof(x20));
        public static readonly RegisterID x21 = new(0, nameof(x21));
        public static readonly RegisterID x22 = new(0, nameof(x22));
        public static readonly RegisterID x23 = new(0, nameof(x23));
        public static readonly RegisterID x24 = new(0, nameof(x24));
        public static readonly RegisterID x25 = new(0, nameof(x25));
        public static readonly RegisterID x26 = new(0, nameof(x26));
        public static readonly RegisterID x27 = new(0, nameof(x27));
        public static readonly RegisterID x28 = new(0, nameof(x28));
        public static readonly RegisterID x29 = new(0, nameof(x29));
        public static readonly RegisterID x30 = new(0, nameof(x30));
        public static readonly RegisterID x31 = new(0, nameof(x31));
    }
}
