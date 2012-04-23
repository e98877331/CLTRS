
#include "include/CLTRS.h"

//using namespace clang;
using namespace CLTRS;
namespace clang{
namespace CLTRS{
class CLTRSConsumer;
class CLTRSAction : public PluginASTAction {
    private:
    string arg_package;
				bool arg_to_root = false;

				CLTRSConsumer *CLTRS;

				void settingArg()
				{
																				  CLTRS->setPackage(arg_package); 
																						CLTRS->setToRoot(arg_to_root); //if arg_to_root is true, then replace all function name to root when output;
				}
				
				protected:
								ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
								    CLTRS = new CLTRSConsumer();
												settingArg();
												return CLTRS;
								}

								bool ParseArgs(const CompilerInstance &CI,
																const std::vector<std::string>& args) {
												for (unsigned i = 0, e = args.size(); i != e; ++i) {
																llvm::errs() << "PrintFunctionNames arg = " << args[i] << "\n";

																// Example error handling.
																if (args[i] == "-an-error") {
																				DiagnosticsEngine &D = CI.getDiagnostics();
																				unsigned DiagID = D.getCustomDiagID(
																												DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
																				D.Report(DiagID);
																				return false;
																}
																//assign java package name
																else if (args[i] == "-package") {
																				++i;
																		  if(i > args.size()-1)
																				{
                    llvm::errs() << "error in setting package "<<args.size()<< " " << i<< "\n";
																				return false;
																				}
																				else
																				{
     //               llvm::errs() << " in setting package "<<args.size()<< " " << args[i]<< "\n";
							              arg_package = args[i];															
																				}

																}
														 else if (args[i] == "-to-root"){
																		 arg_to_root = true;
															}
															
												}
												if (args.size() && args[0] == "help")
																PrintHelp(llvm::errs());

												return true;
								}
								void PrintHelp(llvm::raw_ostream& ros) {
												ros << "Help for CLTRS plugin goes here\n";
								}

};

}
}//end of namespace clang
static FrontendPluginRegistry::Add<CLTRSAction>
X("CLTRS", "print function names");    
