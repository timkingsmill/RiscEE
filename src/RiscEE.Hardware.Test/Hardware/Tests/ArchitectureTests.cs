using NUnit.Framework;
using System.Runtime.CompilerServices;

namespace RiscEE.Hardware.Tests
{
    public class ArchitectureTests
    {
        [SetUp]
        public void Setup()
        {
        }

        [Test]
        public void ArchitectureComparisonTest()
        {
            Assert.Multiple(() =>
            {
                Assert.That(Architecture.Unknown, Is.EqualTo(_ = Architecture.Unknown));
                Assert.That(Architecture.Rv32I, Is.EqualTo(_ = Architecture.Rv32I));
                Assert.That(Architecture.Rv32E, Is.EqualTo(_ = Architecture.Rv32E));
                Assert.That(Architecture.Rv64I, Is.EqualTo(_ = Architecture.Rv64I));
                Assert.That(Architecture.Custom, Is.EqualTo(_ = Architecture.Custom));

                Assert.That(Architecture.Rv32I, Is.GreaterThan(_ = Architecture.Unknown));

                Assert.That(Architecture.Rv32I, Is.Not.Null);
            });
        }
    }
}