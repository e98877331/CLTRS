
#include "include/CLTRS.h"

//using namespace clang;
using namespace CLTRS;

namespace CLTRS{
class CLTRSConsumer;
class CLTRSAction : public PluginASTAction {
				protected:
								ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
												return new CLTRSConsumer();
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
												}
												if (args.size() && args[0] == "help")
																PrintHelp(llvm::errs());

												return true;
								}
								void PrintHelp(llvm::raw_ostream& ros) {
												ros << "Help for PrintFunctionNames plugin goes here\n";
								}

};

}

static FrontendPluginRegistry::Add<CLTRSAction>
X("print-fns", "print function names");    
