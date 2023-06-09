﻿using Kingsmill.Net.IO;
using NUnit.Framework;
using RiscEE.Assembler.Source;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RiscEE.Assembler.Tests
{
    public class AssemblerTests
    {
        // ----------------------------------------------------------------------------------------------

        [SetUp]
        public void Setup()
        {
            LineAssemblerDictionary.RegisterLineAssemblerFactory("add", new LineAssemblerFactory<NullLineAssembler>());

            assembler = new Assembler();
        }

        // ----------------------------------------------------------------------------------------------
        // ----------------------------------------------------------------------------------------------

        [Test]
        public void DummyTest()
        {
            Stream source = "add World".ToStream();
            assembler.Compile(source);
        }

        // ----------------------------------------------------------------------------------------------
        // ----------------------------------------------------------------------------------------------

        private IAssembler assembler;

        // ----------------------------------------------------------------------------------------------
    }
}
