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
        public const string Linux = "Linux";
        public const string Windows = "Windows";

        // Configurations
        public const string Debug = "Debug";
        public const string Release = "Release";
        public const string Shipping = "Shipping";

        static PlatformData()
        {
        }

        private PlatformData()
        {
        }

        public static PlatformData Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
