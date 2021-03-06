#include "include/StringModifier.h"
#include "include/ScriptWriter.h"
#include "include/CLTRS.h"
#include <sstream>
#include "llvm/ADT/APInt.h"


using namespace clang::CLTRS;
using namespace std;

void ScriptWriter::initialize(CLTRSConsumer *consumer)
{
		CLTRS = consumer;                                                                                                                                                                                             
		Rewrite = CLTRS->getRewriter();
		Context = CLTRS->getASTContext();
		arg_package = CLTRS->getArgPackage();
		arg_to_root = CLTRS->getArgToRoot();


}

void ScriptWriter::recordGlobalDecl(Decl *D)
{
 globalDecl.push_back(D);
}

bool ScriptWriter::handleFuncDefinition(FunctionDecl *FD)
{
		if(FD->hasBody()) // check if not prototype 
		{  

				//functionToRewrite.push_back(FD); 

				if(handleFunctionNameAndParameter(FD,arg_to_root))
				{
       

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
		}


  //generate global decl string
		SourceManager * SM = &Context->getSourceManager();
		  std::string gd;
				llvm::raw_string_ostream GS(gd);
		for (vector<Decl *>::iterator it = globalDecl.begin(); it<globalDecl.end();++it)
		{
		  if(SM->getFileID((*it)->getLocation()) != SM->getMainFileID())
    continue;
		  (*it)->print(GS);
				GS<<";\n";
		}
  GS<<"\n";
  //handle special function translate
		for ( vector<CallExpr *>::iterator it=waitRewriteCallExpr.begin() ; it < waitRewriteCallExpr.end(); it++ )
		{
				llvm::errs() << Rewrite->ConvertToString(*it) <<"\n";
				specialFinalFunctionCallHandle(*it);
		}
				printScript(llvm::errs(),FD,GS.str());
  
		
		clearMaps();
		
		return true;
}

bool ScriptWriter::handleFunctionNameAndParameter(FunctionDecl *FD,bool toRoot)
{
		SourceLocation sBegin,sEnd;
		//SourceRange srg = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange(); //in order to get function type end location
		sBegin = FD->getSourceRange().getBegin();
		sEnd = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange().getEnd();
		//sBegin.dump(Rewrite.getSourceMgr());
		//sEnd.dump(Rewrite.getSourceMgr());
		stringstream newFunctionDecl;

  
		//register params to table
		for(FunctionDecl::param_iterator PI = FD->param_begin();PI != FD->param_end();PI++)
		{
		  llvm::errs()<< (*PI)->getType().getAsString() << " " <<(*PI)->getType().getTypePtr()->isPointerType()<<" ";
				//setting output parameter for this function
				if(PI == FD->param_end()-1)
		  paramTable.setOut(*PI);
				//setting other parameter for this function
				else 
		  paramTable.add(*PI);
		}
		llvm::errs()<<"\n";


		if(toRoot)
		{
				//case of Parameter number > 2
			//	if((int)(FD->getNumParams()) >2) //temp command out
				if(1)
				{

						newFunctionDecl<<"void root( ";
						ParmVarDecl * PI = FD->getParamDecl(FD->getNumParams()-(unsigned int)1);
						std::string symName = PI->getName();
						std::string symType = PI->getTypeSourceInfo()->getType().getAsString();
						newFunctionDecl << symType << " " << symName;

						newFunctionDecl <<", const void *usrData, uint32_t x , uint32_t y";
				}
				else //case of Parameter number <= 2 
				{
						newFunctionDecl<<"void root( ";
						for(FunctionDecl::param_iterator PI = FD->param_begin();PI != FD->param_end();PI++)
						{
								std::string symName = (*PI)->getName();
								std::string symType = (*PI)->getTypeSourceInfo()->getType().getAsString();
								newFunctionDecl <<symType <<" "<< symName;
								if(PI != FD->param_end()-1)
										newFunctionDecl << " , ";
						}
				}

    //add CLTRSinput Decl at the start of function body
    CompoundStmt * cs = dyn_cast<CompoundStmt>(FD->getBody());
    Rewrite->InsertText((*(cs->body_begin()))->getLocStart(),"CLTRSin *CLTRSinput = (CLTRSin *)usrData;\n");
		}
		else{ //case of !toRoot

				newFunctionDecl<<"void " << FD->getNameInfo().getAsString() << "( ";
				for(FunctionDecl::param_iterator PI =  FD->param_begin();PI != FD->param_end();PI++)
				{
						std::string symName = (*PI)->getName();
						std::string symType = (*PI)->getTypeSourceInfo()->getType().getAsString();
						//symType = Modifier.replaceStringAccordingToTable(symType,MainTable);

						newFunctionDecl <<symType <<" "<< symName;

						if(PI != FD->param_end()-1)
								newFunctionDecl << " , ";
				}
		}


		newFunctionDecl << " )";
		SourceRange tempRange(sBegin,sEnd);
		Rewrite->ReplaceText(tempRange,newFunctionDecl.str());



		//llvm::errs() << FD->getType().getAsString() << "\n";
		return true;
}

#define TT(x) Rewrite->ConvertToString(x)

Stmt *ScriptWriter::handleStmt(Stmt *ST)
{


		llvm::errs() <<"stmt type:"<< ST->getStmtClassName() <<" stmt : "<< Rewrite->ConvertToString(ST) << "\n";


		for (Stmt::child_range CI = ST->children(); CI; ++CI)  
				if(*CI)	{
						//	llvm::errs() << "\ndumping stmt:----------------------------\n";
						//CI->dump();
						Stmt *childStmt = (*CI);
						Stmt *newStmt =  handleStmt(childStmt);                                                                                                                                       
						if (newStmt) {
								*CI = newStmt;
						}
				}

		if(ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(ST))
				return RewriteArraySubscript(ASE);

		if(CallExpr *CE = dyn_cast<CallExpr>(ST))
				return RewriteCallExpr(CE);

		if(InitListExpr *ILE = dyn_cast<InitListExpr>(ST))
				return RewirteInitListExpr(ILE);

		if(BinaryOperator *BO = dyn_cast<BinaryOperator>(ST))
				return RewriteBinaryOperator(BO);

		if(DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(ST))
		  return RewriteDeclRefExpr(DRE);

		llvm::errs()<<"<-----back level------\n";
		return ST;

}

Stmt *ScriptWriter::RewriteArraySubscript(ArraySubscriptExpr *ASE)
{
		llvm::errs() << " LHS: "<< TT(ASE->getLHS()) <<" RHS: "<<TT(ASE->getRHS()) << " BASE: "<< TT(ASE->getBase()) << " IDX: "<<TT(ASE->getIdx())<< "\n";
 
	 string lhs = Rewrite->ConvertToString(ASE->getLHS());
  string rhs = Rewrite->ConvertToString(ASE->getRHS());
  if(paramTable.find(lhs)&&CLTRS->getArgToRoot())
		{
  //  AStoRewrite.push_back(ASE);
   ParmVarDecl *pvd = paramTable.findForParmDecl(lhs);
			string newString;
			newString += "*(";
			newString += pvd->getType().getAsString();
			newString += ")rsGetElementAt(CLTRSinput->";
			newString += pvd->getNameAsString();
			newString += ",";
			newString += rhs;
			newString += ")";
    
   Rewrite->ReplaceText(ASE->getSourceRange(),newString);
			return 0;
		}

  // if the handling one is result, we can't use other cell because of the output cell is handled before
  if(paramTable.getOutput()->getNameAsString() == Rewrite->ConvertToString(ASE->getLHS())) 
		{
    
		UnaryOperator *UO = new (Context) UnaryOperator(ASE->getLHS(), UO_Deref, ASE->getType(),
						VK_LValue, OK_Ordinary,ASE->getSourceRange().getBegin());
				
		Rewrite->ReplaceStmt(ASE,UO); 
				return UO;
		}

/* ori code

		Expr *Exp = new (Context) BinaryOperator (ASE->getLHS(), ASE->getRHS(), BO_Add, ASE->getType(), ASE->getValueKind(), OK_Ordinary, ASE->getSourceRange().getBegin());
		ParenExpr *PE = new (Context) ParenExpr(SourceLocation(), SourceLocation(),Exp); 
		UnaryOperator *UO = new (Context) UnaryOperator(PE, UO_Deref, ASE->getType(),
						VK_LValue, OK_Ordinary,PE->getSourceRange().getBegin());

		Rewrite->ReplaceStmt(ASE,UO); 
		return UO;
*/
  return 0;
}

Stmt *ScriptWriter::RewriteCallExpr(CallExpr * CE)
{

		//llvm::errs() << CE->getDirectCallee()->getNameInfo().getAsString() << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"; 

		waitRewriteCallExpr.push_back(CE);
		if(!(CE->getDirectCallee()->getNameInfo().getAsString().compare("get_global_id")))
		{
				//IntegerLiteral *IL = new (Context) IntegerLiteral (*Context, llvm::APInt(32,0), CE->getType(), CE->getSourceRange().getBegin());

			
			 //Rewrite->ReplaceStmt(CE,IL);
				//Rewrite->InsertText(IL->getSourceRange().getEnd(),"/*this 0 is generate by replace get_global_id*/");
				//return IL;
    Expr * arg = CE->getArg(0);
				if(Rewrite->ConvertToString(arg) == "0")
    Rewrite->ReplaceText(CE->getSourceRange(),"x");
				else if(Rewrite->ConvertToString(arg) == "1")
				Rewrite->ReplaceText(CE->getSourceRange(),"y");
				return 0;
		}

		return 0;
}

//Rewrite InitListExpr to fix wrong pretty print by clang
Stmt *ScriptWriter::RewirteInitListExpr(InitListExpr *ILE)
{
		ILE = new (Context) InitListExpr(*Context, ILE->getLBraceLoc() , ILE->getInits(), ILE->getNumInits(), ILE->getRBraceLoc());

		return ILE;

}

Stmt *ScriptWriter::RewriteBinaryOperator(BinaryOperator *BO)
{

		if(ExtVectorElementExpr* EVEE = dyn_cast<ExtVectorElementExpr>(BO->getLHS()))
		{ 
		  Stmt * old = BO->getLHS();
				BO->setLHS(EVEE->getBase());
				Rewrite->ReplaceStmt(old,EVEE->getBase());
		}
		return BO;

}

Stmt *ScriptWriter::RewriteDeclRefExpr(DeclRefExpr *DRE)
{
  if(paramTable.find(DRE->getFoundDecl()->getNameAsString()) && CLTRS->getArgToRoot())
		 DREtoRewrite.push_back(DRE);

 return DRE;
}


void ScriptWriter::specialFinalFunctionCallHandle(CallExpr * CE)
{ 
		string s;
		bool b = Modifier->getModifiedFunctionString(CE,s);

		if(b)
		{
				//	 llvm::errs() <<"heheheheh "<<s<<"\n";
				Rewrite->ReplaceText(CE->getSourceRange(),s);
		}
}

void ScriptWriter::printScript(llvm::raw_ostream &out,FunctionDecl *fn,string globalDecls)
{
		if (Rewrite->getRewriteBufferFor(CLTRS->getMainFileID())) {
				llvm::errs() << "Rewriting...\n";
				std::string output = Rewrite->getRewrittenText(fn->getSourceRange()); //std::string(RewriteBuf->begin(), RewriteBuf->end());
				std::stringstream outstream,finalOStream;
				char line[256];

				//final text replacement, pure string handle
				//FIXME:very ugly code


				std::string Preamble;
				Preamble += "#pragma version(1)\n";
				Preamble += "#pragma rs java_package_name(";
				Preamble += arg_package;
				Preamble += ")\n\n";
				//	Rewrite.InsertText(SM->getLocForStartOfFile(MainFileID), Preamble, true);
				finalOStream <<Preamble;

				finalOStream <<globalDecls;

    //print global bind decls
    std::vector<ParmVarDecl *> params = paramTable.getParms();
    string declForBind;
				for(std::vector<ParmVarDecl *>::iterator it = params.begin(); it < params.end(); ++it)
				{
      if((*it)->getType()->isPointerType())
      declForBind +="rs_allocation";
						else
						declForBind += (*it)->getType().getAsString();

      declForBind += " ";
						declForBind += (*it)->getNameAsString();
						declForBind += ";\n";

				}


      if(paramTable.getOutput()->getType()->isPointerType())
      declForBind +="rs_allocation";
						else
						declForBind += paramTable.getOutput()->getType().getAsString();
    declForBind += " ";
				 declForBind += paramTable.getOutput()->getNameAsString();
				 declForBind += ";\nrs_script gScript;\n\n";

     outstream << declForBind;


    //print CLTRSin struct
				//if(arg_to_root && (fn->getNumParams() > 2))
				if(arg_to_root)
				{
						string CLTRSinput;
						CLTRSinput += "typedef struct CLTRSin {\n";
						for(int i = 0;i<(int)fn->getNumParams()-1;i++)
						{
						  if(fn->getParamDecl(i)->getType()->isPointerType())
								CLTRSinput += "rs_allocation";
								else
								CLTRSinput += fn->getParamDecl(i)->getType().getAsString();
								
								CLTRSinput += " ";
								CLTRSinput += fn->getParamDecl(i)->getNameAsString();
								CLTRSinput +=";\n";
						}
						CLTRSinput += "} CLTRSin;\n\n";

						outstream << CLTRSinput;
				}

				outstream << output;


    //add filter
				outstream << "\n\nvoid execute()\n{\nCLTRSin CLTRSinput;\n";

				for(std::vector<ParmVarDecl *>::iterator it = params.begin(); it < params.end(); ++it)
				{
      outstream <<"CLTRSinput."<<(*it)->getNameAsString()<<" = "<<(*it)->getNameAsString()<<";\n";
				}
				 outstream << "rs_allocation unused;\n";
     outstream << "rsForEach(gScript,unused," << paramTable.getOutput()->getNameAsString();
					outstream << ",&CLTRSinput,sizeof(CLTRSinput));\n}\n";


    //final replacement
				while(!outstream.eof())
				{
						outstream.getline(line,256);

						finalOStream << Modifier->replaceStringAccordingToTable(line,MainTable)<< "\n";
				}
				out << finalOStream.str()<<"\n\n";
		}


}

void ScriptWriter::HandleTranslationUnit() 
{
		SourceManager * SM = &Context->getSourceManager();

   llvm::errs()<<"Print debug information:\n";

  for(vector<ArraySubscriptExpr *>::iterator it = AStoRewrite.begin(); it<AStoRewrite.end();++it)
		{
    llvm::errs()<<Rewrite->ConvertToString(*it)<<":";
				(*it)->getExprLoc().print(llvm::errs(),*SM);
    llvm::errs()<<"\n";



		}
		llvm::errs()<<"\n";
  for(vector<DeclRefExpr *>::iterator it = DREtoRewrite.begin(); it<DREtoRewrite.end();++it)
		{
    llvm::errs()<<Rewrite->ConvertToString(*it)<<" ";
				(*it)->getExprLoc().print(llvm::errs(),*SM);
    llvm::errs()<<"\n";
				
			// string ts = "CLTRSin." + (*it)->getFoundDecl()->getNameAsString();
			//	Rewrite->ReplaceText((*it)->getSourceRange(),ts);
		}
		llvm::errs()<<"\n";



  //output one script per function 
	//	for (vector<FunctionDecl *>::iterator it = functionToRewrite.begin() ; it < functionToRewrite.end(); it++)

  //FIXME case of output all function once

/*
			
					PrintingPolicy Policy = Context->getPrintingPolicy();
					Policy.Dump = false;                                                                                                                                                                                      
					Context->getTranslationUnitDecl()->print(llvm::errs(), Policy, 0,
					true);// */




}


