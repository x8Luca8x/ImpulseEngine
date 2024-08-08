namespace Impulse
{
    public class ModuleRules
    {
        /// <summary>
        /// List of public module references.
        /// If another module references this module, this will automatically add the module to the public dependencies.
        /// </summary>
        public List<string> PublicDependencyModuleNames = new();

        /// <summary>
        /// List of private module references.
        ///  Every module in this list should <c>ONLY</c> be used in the <c>PRIVATE</c> implementation of this module.
        /// </summary>
        public List<string> PrivateependencyModuleNames = new();

        /// <summary>
        /// List of public include paths.
        /// If another module references this module, this will automatically add the include path to the public include paths.
        /// </summary>
        public List<string> PublicIncludePaths = new();

        /// <summary>
        /// List of private include paths.
        /// Every include path in this list should <c>ONLY</c> be used in the <c>PRIVATE</c> implementation of this module.
        /// </summary>
        public List<string> PrivateIncludePaths = new();
    }
}
