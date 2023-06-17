using RiscEE.Assembler.Source;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler
{
    public class Assembler : IAssembler
    {

        // ---------------------------------------------------------------------------------

        public Assembler(LineAssemblerFactory lineAssemblerFactory)
        {
            _lineAssemblerFactory = lineAssemblerFactory;
        }

        // ---------------------------------------------------------------------------------

        IAssemblyFile IAssembler.Compile(Stream source)
        {
            using (StreamReader sr = new(source))
            {
                while (sr.Peek() >= 0)
                {
                    //var lineAssembler = 
                    Console.WriteLine(sr.ReadLine());
                }
            }
            return new AssemblyFile();
        }

        private readonly LineAssemblerFactory _lineAssemblerFactory;

    }
}
