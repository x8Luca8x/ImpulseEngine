using Impulse;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace ModuleBuildTool
{
    internal class ModuleDataFactory
    {
        public static ModuleRules GetModuleRules(string BuildCSPath)
        {
            return new ModuleRules();
        }

        public static XmlDocument CreateModuleData(ModuleRules Rules)
        {
            XmlDocument doc = new XmlDocument();
            XmlElement module = doc.CreateElement("Module");

            AddString(doc, module, "ModuleType", Rules.ModuleType.ToString());
            AddList(doc, module, "PublicDependencyModuleNames", Rules.PublicDependencyModuleNames);
            AddList(doc, module, "PrivateDependencyModuleNames", Rules.PrivateDependencyModuleNames);
            AddList(doc, module, "PublicIncludePaths", Rules.PublicIncludePaths);
            AddList(doc, module, "PrivateIncludePaths", Rules.PrivateIncludePaths);

            doc.AppendChild(module);
            return doc;
        }

        public static void WriteModuleData(ModuleRules Rules, string ModuleName)
        {
            if (!Directory.Exists(BuildToolContext.Current.GetBuildModuleDirectory()))
                Directory.CreateDirectory(BuildToolContext.Current.GetBuildModuleDirectory());

            string outputPath = Path.Combine(BuildToolContext.Current.GetBuildModuleDirectory(), ModuleName + ".xml");

            XmlDocument doc = CreateModuleData(Rules);
            doc.Save(outputPath);
        }

        public static void AddString(XmlDocument Doc, XmlElement Element, string Name, string Value)
        {
            XmlElement elem = Doc.CreateElement(Name);
            elem.InnerText = Value;
            Element.AppendChild(elem);
        }

        private static void AddList(XmlDocument Doc, XmlElement Element, string ListName, List<string> Values)
        {
            XmlElement list = Doc.CreateElement(ListName);
            foreach (var value in Values)
            {
                XmlElement item = Doc.CreateElement("Item");
                item.InnerText = value;
                list.AppendChild(item);
            }

            Element.AppendChild(list);
        }
    }
}
