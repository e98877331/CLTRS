
#include "include/CLTRS.h"

//using namespace clang;
using namespace CLTRS;

namespace CLTRS{
class CLTRSConsumer;
class CLTRSAction : public PluginASTAction {
    private:
    string package;

				CLTRSConsumer *CLTRS;

				void settingArg()
				{
																				  CLTRS->setPackage(package);
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
							              package = args[i];															
																				}

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

static FrontendPluginRegistry::Add<CLTRSAction>
X("CLTRS", "print function names");    
