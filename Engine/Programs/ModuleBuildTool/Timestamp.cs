using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModuleBuildTool
{
    internal class Timestamp
    {
        public const int Version = 1;
        public const string TimestampFileName = "ModuleBuildTimestamp";

        public static Timestamp Current { get; } = new Timestamp();

        /// <summary>
        /// Modules in the timestamp.
        /// </summary>
        public HashSet<string> Modules { get; private set; } = new HashSet<string>();

        public DateTime DateTime { get; private set; }

        /// <summary>
        /// Version read from the timestamp file.
        /// </summary>
        private int _version = 0;

        public string GetTimestampFilePath()
        {
            return Path.Combine(BuildToolContext.Current.GetBuildDirectory(), TimestampFileName);
        }

        public Timestamp()
        {
            // Load the timestamp from the file.

            if (File.Exists(GetTimestampFilePath()))
            {
                using (var reader = new StreamReader(GetTimestampFilePath()))
                {
                    // Every line in the timestamp file is a module.

                    while (!reader.EndOfStream)
                    {
                        string mod = reader.ReadLine() ?? string.Empty;
                        if (!string.IsNullOrWhiteSpace(mod))
                        {
                            // Check if the line is the version.

                            if (mod.StartsWith("Version: "))
                            {
                                _version = int.Parse(mod.Substring("Version: ".Length));
                                continue;
                            }

                            Modules.Add(mod);
                        }
                    }

                    // Get the updated date time of the timestamp file.

                    DateTime = File.GetLastWriteTime(GetTimestampFilePath());
                }
            }
            else
            {
                DateTime = DateTime.MinValue;
            }
        }

        public void Write()
        {
            if (!File.Exists(GetTimestampFilePath()))
            {
                Directory.CreateDirectory(Path.GetDirectoryName(GetTimestampFilePath())!);
                File.Create(GetTimestampFilePath()).Close();
            }

            StringBuilder sb = new StringBuilder();

            // Add version to the timestamp file.

            sb.AppendLine($"Version: {Version}");

            // Add modules to the timestamp file.

            foreach (var mod in Modules)
                sb.AppendLine(mod);

            File.WriteAllText(GetTimestampFilePath(), sb.ToString());
            DateTime = File.GetLastWriteTime(GetTimestampFilePath());
        }

        public bool NeedsRebuild(string Module)
        {
            // Check if the timestamp file is newer than the last build.

            if (_version != Version || !Modules.Contains(Module))
                return true;

            string moduleName = Path.GetFileName(Module)!;
            string moduleBuildCs = Path.Combine(Module, moduleName + ".Build.cs");

            if (!File.Exists(moduleBuildCs))
                return false;

            DateTime moduleBuildCsTime = File.GetLastWriteTime(moduleBuildCs);
            if (DateTime > moduleBuildCsTime)
                return false;

            return true;
        }
    }
}
