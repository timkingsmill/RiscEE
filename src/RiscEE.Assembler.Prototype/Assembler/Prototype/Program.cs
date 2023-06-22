using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace RiscEE.Assembler.Prototype
{
    internal class Program
    {

        // ----------------------------------------------------------------------------------------

        static void Main(string[] args)
        {
            Console.WriteLine("\nRiscEE Assembler Prototype\n");

            var files = GetTestCaseFileNames();
            DisplaySelectTestCasePrompt(files);

            var index = SelectTestCaseFileIndex(files);

            if ((index > -1) && (index < files.Count()))
            {
                var filename = files.Select(r => r.ToString()).ElementAt(index);
                AssembleFile(filename);
            }
        }

        // ----------------------------------------------------------------------------------------

        private static void AssembleFile(string filename) 
        {
            using (StreamReader reader = new StreamReader(filename))
            {
                var assembler = new Assembler();
                string? line;
                while ((line = reader.ReadLine()) != null)
                {
                    assembler.ParseLine(line);
                }
            }
        }

        // ----------------------------------------------------------------------------------------

        // ----------------------------------------------------------------------------------------

        private static void DisplaySelectTestCasePrompt(IEnumerable<string> files)
        {
            var index = 0;
            foreach (var file in files)
            {
                var prompt = $"  {index++,-4}{Path.GetFileName(file)}";
                Console.WriteLine(prompt);
            }
        }

        // ----------------------------------------------------------------------------------------

        private static IEnumerable<string> GetTestCaseFileNames() 
        {
            string path = Environment.CurrentDirectory + "\\TestCases";
            return from file in Directory.EnumerateFiles(path) select file;
        }

        // ----------------------------------------------------------------------------------------

        private static int SelectTestCaseFileIndex(IEnumerable<string> files)
        {
            Console.WriteLine();
            Int32 index = -1; 
            while (index < 0)
            {
                Console.Write("Select example source file: ");
                var input = Console.ReadLine();
                if (Int32.TryParse(input, out Int32 result))
                {
                    if ((result > -1) && (result < files.Count()))
                    {
                        return result;
                    }
                    Console.WriteLine("Invalid index. Try again.");
                }
                else
                {
                    Console.WriteLine("Invalid input. Try again.");
                }
            }
            return -1;
        }

        // ----------------------------------------------------------------------------------------
        // ----------------------------------------------------------------------------------------

    }
}