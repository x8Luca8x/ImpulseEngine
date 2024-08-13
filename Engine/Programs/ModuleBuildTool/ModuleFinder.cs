using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModuleBuildTool
{
    internal class ModuleFinder
    {
        public static List<string> GetBuildCSFiles()
        {
            var buildCSFiles = new List<string>();
            string[] sourceDirs = Directory.GetDirectories(BuildToolContext.Current.SourceDirectory);

            foreach (var directory in sourceDirs)
            {
                var buildCSFile = SearchDirectory(directory);
                buildCSFiles.AddRange(buildCSFile);
            }

            return buildCSFiles;
        }

        private static List<string> SearchDirectory(string directory)
        {
            List<string> files = new List<string>();
            string? buildCSFile = SearchForBuildCSFile(directory);

            if (buildCSFile != null)
            {
                files.Add(buildCSFile);
                return files; // Return early if we found a build.cs file.
            }

            string[] directories = Directory.GetDirectories(directory);
            foreach (var subDirectory in directories)
            {
                var result = SearchDirectory(subDirectory);
                if (result != null)
                    files.AddRange(result);
            }

            return files;
        }

        private static string? SearchForBuildCSFile(string directory)
        {
            foreach (var file in Directory.GetFiles(directory))
            {
                if (file.EndsWith(".Build.cs"))
                {
                    return file;
                }
            }

            return null;
        }
    }
}
