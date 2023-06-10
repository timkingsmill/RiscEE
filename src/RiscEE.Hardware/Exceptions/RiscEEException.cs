using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Exceptions
{
    public class RiscEEException : ApplicationException
    {
        public RiscEEException(string message) : base(message) { }
        public RiscEEException(string message, System.Exception inner) : base(message, inner) { }
    }
}
