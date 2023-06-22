using System.Collections.Generic;

namespace RiscEE.Assembler.Source
{

    public static class LineAssemblerDictionary
    {
        // -----------------------------------------------------------------------------------

        public static ILineAssembler? CreateLineAssembler(string line)
        {
            string[] words = line.Split(' ');
            var factory = _factoryDictionary[words[0]];
            return factory.CreateLineAssembler();
        }

        // -----------------------------------------------------------------------------------
        // -----------------------------------------------------------------------------------

        public static void RegisterLineAssemblerFactory(string key, ILineAssemblerFactory<ILineAssembler> lineAssemblerFactory)
        {
            _factoryDictionary[key] = lineAssemblerFactory;
        }

        // -----------------------------------------------------------------------------------
        // -----------------------------------------------------------------------------------

        private static readonly Dictionary<string, ILineAssemblerFactory<ILineAssembler>> _factoryDictionary = new();

        // -----------------------------------------------------------------------------------

    }
}
