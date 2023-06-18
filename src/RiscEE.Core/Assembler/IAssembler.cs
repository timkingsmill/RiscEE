using System.IO;

namespace RiscEE.Assembler
{
    public interface IAssembler
    {
        public IAssemblyFile Compile(Stream source);
    }
}