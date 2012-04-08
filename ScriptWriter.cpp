#include "include/ScriptWriter.h"
#include <sstream>
using namespace CLTRS;

bool ScriptWriter::handleFuncDefinition(FunctionDecl *FD)
{
		if(FD->hasBody()) // check if not prototype 
		{  

				if(handleFunctionNameAndParameter(FD))

						if(NamedDecl *ND = dyn_cast<NamedDecl>(FD))
						{
								//			FD->dump();
								//llvm::errs() << ND->getAttrs() << "\n";
								llvm::errs() <<"Function:"<< ND->getName()  << "\n";

								Stmt *ST = FD->getBody();
								if(ST)
								{
										//	  llvm::errs() << "going to handleStmt\n";
										handleStmt(ST);
								}
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
				symType = Modifier.modifyString(symType,MainTable);

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



		//llvm::errs() << FD->getType().getAsString() << "\n";
		return true;
}

#define TT(x) Rewrite->ConvertToString(x)

Stmt *ScriptWriter::handleStmt(Stmt *ST)
{


		llvm::errs() <<"stmt type:"<< ST->getStmtClassName() <<" stmt : "<< Rewrite->ConvertToString(ST) << "\n";


		/*/ print ArraySubscriptExpr for test
				if(ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(ST))
				{
				llvm::errs() << " LHS: "<< TT(ASE->getLHS()) <<" RHS: "<<TT(ASE->getRHS()) << " BASE: "<< TT(ASE->getBase()) << " IDX: "<<TT(ASE->getIdx())<< "\n";
				}
			*/

		for (Stmt::child_range CI = ST->children(); CI; ++CI) { 
				if(*CI)
				{
						//	llvm::errs() << "\ndumping stmt:----------------------------\n";
						//CI->dump();

								Stmt *childStmt = (*CI);
								Stmt *newStmt =  handleStmt(childStmt);                                                                                                                                       
								if (newStmt) {
										*CI = newStmt;
								}

 

				}
   }

				if(ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(ST))
				{
				llvm::errs() << " LHS: "<< TT(ASE->getLHS()) <<" RHS: "<<TT(ASE->getRHS()) << " BASE: "<< TT(ASE->getBase()) << " IDX: "<<TT(ASE->getIdx())<< "\n";


				Expr *Exp = new (Context) BinaryOperator (ASE->getLHS(), ASE->getRHS(), BO_Add, ASE->getType(), ASE->getValueKind(), OK_Ordinary, ASE->getSourceRange().getBegin());
				ParenExpr *PE = new (Context) ParenExpr(SourceLocation(), SourceLocation(),Exp); 
  		UnaryOperator *UO = new (Context) UnaryOperator(PE, UO_Deref, ASE->getType(),
								VK_LValue, OK_Ordinary,                                                                                                                                              
								PE->getSourceRange().getBegin());

    Rewrite->ReplaceStmt(ASE,UO);
				
				return UO;

				}

				llvm::errs()<<"-------------------\n";
		return ST;

}
