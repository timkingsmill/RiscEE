﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler.Source
{
    public interface ILineAssemblerFactory
    {
        public ILineAssembler CreateLineAssembler();
    }
}
