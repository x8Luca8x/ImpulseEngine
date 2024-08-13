using Impulse;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Text;

namespace ModuleBuildTool
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var l = ModuleFinder.GetBuildCSFiles();
            foreach (var f in l)
            {
                ModuleRules rules = new ModuleRules();
                rules.PublicDependencyModuleNames.Add("Test");

                string moduleDir = Path.GetDirectoryName(f)!;
                if (Timestamp.Current.NeedsRebuild(moduleDir))
                { 
                    ModuleDataFactory.WriteModuleData(ModuleDataFactory.GetModuleRules(f), Path.GetFileNameWithoutExtension(f)); 
                    Timestamp.Current.Modules.Add(moduleDir);
                }
            }

            Timestamp.Current.Write();
        }
    }
}
