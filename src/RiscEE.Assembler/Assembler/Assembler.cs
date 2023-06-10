using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler
{
    public class Assembler : IAssembler
    {
        public Assembler() { }

        IAssembly IAssembler.Compile()
        {
            throw new NotImplementedException();
        }


    }
}
