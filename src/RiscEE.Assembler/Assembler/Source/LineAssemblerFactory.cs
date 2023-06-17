using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler.Source
{
    public class LineAssemblerFactory
    {

        public LineAssemblerFactory(LineAssemblerDictionary lineAssemblerDictionary) 
        {
            _lineAssemblerDictionary = lineAssemblerDictionary;
        }

        // --------------------------------------------------------------------------------
        // --------------------------------------------------------------------------------

        public ILineAssembler GetLineAssembler(string line)
        {

            throw new NotImplementedException();
        }

        // --------------------------------------------------------------------------------

        private readonly LineAssemblerDictionary _lineAssemblerDictionary;

        // --------------------------------------------------------------------------------
        // --------------------------------------------------------------------------------

    }
}
