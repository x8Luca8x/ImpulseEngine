using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModuleBuildTool
{
    internal class BuildToolContext
    {
        private static BuildToolContext? _current = null;

        public static BuildToolContext Current 
        { 
            get
            {
                if (_current is null)
                {
                    Parser.Default.ParseArguments<BuildToolContext>(Environment.GetCommandLineArgs())
                        .WithParsed(parsed => _current = parsed);

                    if (_current is null)
                    {
                        throw new InvalidOperationException("Failed to parse command line arguments.");
                    }
                }

                return _current;
            }
        }

        [Option('s', "source", Required = true, HelpText = "The source directory to build.")]
        public string SourceDirectory { get; set; }

        /// <summary>
        /// Gets the path to the intermediate directory.
        /// </summary>
        /// <returns></returns>
        public string GetIntermediateDirectory()
        {
            return Path.Combine(SourceDirectory, "..", "Intermediate");
        }

        /// <summary>
        /// Gets the path to the build directory.
        /// In the build directory will be every build relevant files stored.
        /// </summary>
        /// <returns></returns>
        public string GetBuildDirectory()
        {
            return Path.Combine(SourceDirectory, "..", "Build");
        }

        /// <summary>
        /// Gets the output directory for the build.
        /// </summary>
        /// <returns></returns>
        public string GetBuildModuleDirectory()
        {
            return Path.Combine(GetBuildDirectory(), "Modules");
        }
    }
}
