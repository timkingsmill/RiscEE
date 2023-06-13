using System.Collections.Generic;

namespace RiscEE.Instructions
{
    public class InstructionSet : IInstructionSet
    {
        public InstructionSet()
        {

        }

        private List<OpCode> _opCodes = new();
    }
}