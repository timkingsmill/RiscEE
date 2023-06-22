using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler.Source
{

    public interface ILineAssemblerFactory<out TLineAssembler> where TLineAssembler : class, ILineAssembler//, new() 
    {
        public TLineAssembler? CreateLineAssembler();
    }


    public class LineAssemblerFactory<TLineAssembler> : ILineAssemblerFactory<ILineAssembler> where TLineAssembler : class, ILineAssembler
    {
        public ILineAssembler? CreateLineAssembler()
        {
            return Activator.CreateInstance(typeof(TLineAssembler)) as ILineAssembler;
        }
    }
}
