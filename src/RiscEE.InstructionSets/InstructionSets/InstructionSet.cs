using System.Collections.Generic;

namespace RiscEE.InstructionSets
{
    public class InstructionSet : IInstructionSet
    {
        public InstructionSet()
        {

        }

        private List<OpCode> _opCodes = new();
    }
}