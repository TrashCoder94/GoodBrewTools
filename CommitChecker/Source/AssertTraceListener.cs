using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommitChecker.Source
{
    public class AssertTraceListener : TraceListener
    {
        public override void Write(string msg)
        {
            throw new Exception(msg);
        }
        public override void WriteLine(string msg)
        {
            throw new Exception(msg);
        }
    }
}
