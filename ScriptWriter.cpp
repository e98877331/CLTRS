#include "include/ScriptWriter.h"
using namespace CLTRS;

bool ScriptWriter::handleFuncDefinition(FunctionDecl *FD)
{
	if(FD->hasBody())
		{
   if(NamedDecl *ND = dyn_cast<NamedDecl>(FD))
			{
				llvm::errs() << "\ndumping name:\n";
	//			FD->dump();
    //llvm::errs() << ND->getAttrs() << "\n";
				llvm::errs() << ND->getName()  << "\n";

				Stmt *ST = FD->getBody();
				if(ST)
						handleStmt(ST);
				}
		}


return true;
}


void ScriptWriter::handleStmt(Stmt *ST)
{

		for (Stmt::child_range CI = ST->children(); CI; ++CI) { 
				if(*CI)
				{
						llvm::errs() << "\ndumping stmt:----------------------------\n";
						//CI->dump();

						llvm::errs() << CI->getStmtClassName() <<"\n";


				for (Stmt::child_range CII = CI->children(); CII; ++CII) 
				{ 
						if(*CII)
						{
						  
						llvm::errs() << "\ndumping single stmt:\n";
								CII->dumpAll();
						}
				}


				}


		}

}
