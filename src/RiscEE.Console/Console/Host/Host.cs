using RiscEE.Exceptions;
using RiscEE.Hardware;
using RiscEE.Hardware.Hart;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Console.Host
{
    public class Host
    {
        public void Run(HartConfiguration hartConfiguration)
        {
            Logger.Info("CPU = {cpu}", hartConfiguration.Architecture);

            var hart = Build(hartConfiguration);
            hart.Configure(hartConfiguration);
            ReadOpcode(hartConfiguration, hart);



        }

        private IHart Build(HartConfiguration hartConfiguration)
        {
            IHart? hart = null;

            var architecture = hartConfiguration.Architecture;
            if (architecture == Architecture.Rv32E || architecture == Architecture.Rv32I)
            {
                hart = new Hart32Bit(architecture);
            }

            if (hart == null)
            {
                throw new RiscEEException("Unsupported architecture detected!");
            }

            return hart;

        }

        private void ReadOpcode(HartConfiguration config, IHart hart)
        {
            /***
            var lowLwevelParser = new Parser();
            var myProgram = lowLwevelParser.Parse(config.Source);
            Console.WriteLine("\n## Program details:\n");
            Console.WriteLine(myProgram.GetHumanReadableContent());

            //
            // Init the RISC V hart and start the simulation
            //

            hart.Init(myProgram.InitialProgramCounter);

            // Load each modules to the memory
            foreach (var subRoutineMarker in myProgram.GetSubRoutineMarker())
            {
                var data = myProgram.GetSubRoutine(subRoutineMarker);
                hart.Load(subRoutineMarker, data);
            }
            ***/
        }

        private static readonly NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();
    }
}
