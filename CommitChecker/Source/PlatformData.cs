using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommitChecker
{
    public sealed class PlatformData
    {
        private static readonly PlatformData instance = new PlatformData();
        
        // Platforms
        public static readonly string Linux = "Linux";
        public static readonly string Windows = "Windows";

        // Configurations
        public static readonly string Debug = "Debug";
        public static readonly string Release = "Release";
        public static readonly string Shipping = "Shipping";

        public static PlatformData Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
