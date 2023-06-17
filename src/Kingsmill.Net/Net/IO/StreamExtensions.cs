using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Kingsmill.Net.IO
{
    public static class StreamExtensions
    {

        // ------------------------------------------------------------------------------

        public static Stream ToStream(this string str) => ToStream(str, Encoding.UTF8);

        public static Stream ToStream(this string str, Encoding encoding)
            => new MemoryStream(encoding.GetBytes(str ?? string.Empty));

        // ------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------

    }
}
