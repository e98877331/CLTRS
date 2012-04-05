#include "include/ScriptWriter.h"
#include <sstream>
using namespace CLTRS;

bool ScriptWriter::handleFuncDefinition(FunctionDecl *FD)
{
	if(FD->hasBody()) // check if not prototype 
		{  

    if(!handleFunctionNameAndParameter(FD))
				llvm::errs() << "handleFunctionNameAndParameter fault\n";

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

bool ScriptWriter::handleFunctionNameAndParameter(FunctionDecl *FD)
{
				SourceLocation sBegin,sEnd;
				//SourceRange srg = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange(); //in order to get function type end location
				sBegin = FD->getSourceRange().getBegin();
				sEnd = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange().getEnd();
				//sBegin.dump(Rewrite.getSourceMgr());
				//sEnd.dump(Rewrite.getSourceMgr());
    stringstream newFunctionDecl;
    newFunctionDecl<<"void root(";
				for(FunctionDecl::param_iterator PI =  FD->param_begin();PI != FD->param_end();PI++)
				{
								std::string symName = (*PI)->getName();
								std::string symType = (*PI)->getTypeSourceInfo()->getType().getAsString();
        symType = Modifier.modifyString(symType,ASQ);

        newFunctionDecl << symType << symName;
								
								if(PI != FD->param_end()-1)
								newFunctionDecl << ",";
       
	/*if((*PI)->getTypeSourceInfo()->getType().getTypePtr()->getPointeeType().isConstQualified())
								{ 

												llvm::errs() << "akl;jg;lakjgkl;djgl;kasdjgl;aksdjgkl;asdjgkl;asdjgkl;jkljdkgjkj,mcv./,jmkojsfjkl;jkjk\n";
												symType = "const";
								}
								else
												llvm::errs() << "no const find\n";
												*/


//								llvm::errs() << symName << "," << symType << "\n";
				}
				
      newFunctionDecl << ")";
SourceRange tempRange(sBegin,sEnd);
Rewrite->ReplaceText(tempRange,newFunctionDecl.str());

llvm::errs() << "\nttestst\n";
llvm::errs() << FD->getType().getAsString() << "\n";
return true;
}


void ScriptWriter::handleStmt(Stmt *ST)
{

		for (Stmt::child_range CI = ST->children(); CI; ++CI) { 
				if(*CI)
				{
					//	llvm::errs() << "\ndumping stmt:----------------------------\n";
						//CI->dump();

						llvm::errs() << CI->getStmtClassName() <<"\n";


				for (Stmt::child_range CII = CI->children(); CII; ++CII) 
				{ 
						if(*CII)
						{
						  
			//			llvm::errs() << "\ndumping single stmt:\n";
			//					CII->dumpAll();
						}
				}


				}


		}

}
