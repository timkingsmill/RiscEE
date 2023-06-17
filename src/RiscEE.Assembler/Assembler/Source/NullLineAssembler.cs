using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler.Source
{
    public class NullLineAssembler : ILineAssembler
    {
    }


    // ---------------------------------------------------------------------------------------------

    public class NullLineAssemblerFactory : ILineAssemblerFactory
    {
        ILineAssembler ILineAssemblerFactory.CreateLineAssembler()
        {
            return new NullLineAssembler();
        }
    }

    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------

}
