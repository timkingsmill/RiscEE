using Kingsmill.Net.Enumerations;

namespace RiscEE.Hardware.Hart
{
    public class HartState : Enumeration
    {
        protected HartState(int id, string name)
            : base(id, name)
        {
        }

        public static readonly HartState Initilized = new(0, nameof(Initilized));
        public static readonly HartState Waiting = new(1, nameof(Waiting));
        public static readonly HartState Running = new(2, nameof(Running));
        public static readonly HartState Stopped = new(3, nameof(Stopped));
        public static readonly HartState Error = new(4, nameof(Error));

    }
}
