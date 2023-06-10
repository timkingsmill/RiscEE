namespace RiscEE.Hardware.Hart
{
    public abstract class HartBase : IHart
    {
        public HartBase(Architecture architecture)
        {
            _architecture = architecture;
            _isInitialized = false;
        }

        public virtual void Configure(HartConfiguration configuration)
        {
        }

        protected static readonly NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        protected bool _isInitialized;


        
        private readonly Architecture _architecture;
        public Architecture Architecture
        { 
            get
            { 
                return _architecture; 
            }
        }

    }
}
