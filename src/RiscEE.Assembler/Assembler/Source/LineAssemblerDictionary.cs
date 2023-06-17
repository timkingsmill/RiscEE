using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

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

        public static void RegisterLineAssemblerFactory(string key, ILineAssemblerFactory factory)
        {
            _factoryDictionary[key] = factory;
        }

        // -----------------------------------------------------------------------------------
        // -----------------------------------------------------------------------------------

        private static readonly Dictionary<string, ILineAssemblerFactory> _factoryDictionary = new();

        // -----------------------------------------------------------------------------------

    }
}
