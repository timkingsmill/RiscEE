
using RiscEE.Hardware.Hart;

namespace RiscEE.Console
{
    internal class Program
    {
        private static readonly NLog.ILogger Logger = NLog.LogManager.GetCurrentClassLogger();

        static void Main(string[] args)
        {
            Logger.Info("Simulator started");

            var host = new Host.Host();
            host.Run(new Rv32IHartConfiguration());

        }
    }
}