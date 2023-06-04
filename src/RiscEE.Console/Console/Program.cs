namespace RiscEE.Console
{

    internal class Program
    {
        private static readonly NLog.ILogger _logger = NLog.LogManager.GetCurrentClassLogger();

        static void Main(string[] args)
        {
            _logger.Info("Simulator started");

            //Console.WriteLine("Hello, World!");
        }
    }
}