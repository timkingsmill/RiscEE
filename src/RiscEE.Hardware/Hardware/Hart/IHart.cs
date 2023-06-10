using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Hardware.Hart
{
    /// <summary>
    /// Represents a RISC-V hart (CPU with Instruction fetch)
    /// </summary>
    public interface IHart
    {
        public Architecture Architecture { get; }

        /// <summary>
        /// Configures a hart
        /// </summary>
        /// <param name="configuration">the configuration</param>
        public void Configure(HartConfiguration configuration);

    }
}
