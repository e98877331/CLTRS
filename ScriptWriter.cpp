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

				functionToRewrite.push_back(FD); 

				if(handleFunctionNameAndParameter(FD,arg_to_root))
						//   if(true)
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

bool ScriptWriter::handleFunctionNameAndParameter(FunctionDecl *FD,bool toRoot)
{
		SourceLocation sBegin,sEnd;
		//SourceRange srg = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange(); //in order to get function type end location
		sBegin = FD->getSourceRange().getBegin();
		sEnd = FD->getTypeSourceInfo()->getTypeLoc().getSourceRange().getEnd();
		//sBegin.dump(Rewrite.getSourceMgr());
		//sEnd.dump(Rewrite.getSourceMgr());
		stringstream newFunctionDecl;

		if(toRoot)
		{
				//case of Parameter number > 2
				if((int)(FD->getNumParams()) >2)
				{

						newFunctionDecl<<"void root( const CLTRSin * CLTRSinput ,";
						ParmVarDecl * PI = FD->getParamDecl(FD->getNumParams()-(unsigned int)1);
						std::string symName = PI->getName();
						std::string symType = PI->getTypeSourceInfo()->getType().getAsString();
						newFunctionDecl << symType << " " << symName;
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


		}
		else{ //end to end case

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

		llvm::errs()<<"<-----back level------\n";
		return ST;

}

Stmt *ScriptWriter::RewriteArraySubscript(ArraySubscriptExpr *ASE)
{
		llvm::errs() << " LHS: "<< TT(ASE->getLHS()) <<" RHS: "<<TT(ASE->getRHS()) << " BASE: "<< TT(ASE->getBase()) << " IDX: "<<TT(ASE->getIdx())<< "\n";



		Expr *Exp = new (Context) BinaryOperator (ASE->getLHS(), ASE->getRHS(), BO_Add, ASE->getType(), ASE->getValueKind(), OK_Ordinary, ASE->getSourceRange().getBegin());
		ParenExpr *PE = new (Context) ParenExpr(SourceLocation(), SourceLocation(),Exp); 
		UnaryOperator *UO = new (Context) UnaryOperator(PE, UO_Deref, ASE->getType(),
						VK_LValue, OK_Ordinary,                                                                                                                                              
						PE->getSourceRange().getBegin());

		//	ParenExpr *PE2 = new (Context) ParenExpr(SourceLocation(), SourceLocation(),UO); 
		Rewrite->ReplaceStmt(ASE,UO);
		return UO;

}

Stmt *ScriptWriter::RewriteCallExpr(CallExpr * CE)
{

		//llvm::errs() << CE->getDirectCallee()->getNameInfo().getAsString() << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"; 

		waitRewriteCallExpr.push_back(CE);
		if(!(CE->getDirectCallee()->getNameInfo().getAsString().compare("get_global_id")))
		{
				// distributeUnit.insert(std::make_pair(, symType)); 
				IntegerLiteral *IL = new (Context) IntegerLiteral (*Context, llvm::APInt(32,0), CE->getType(), CE->getSourceRange().getBegin());

				//ParenExpr *PE = new (Context) ParenExpr(SourceLocation(), SourceLocation(),IL); 
				Rewrite->ReplaceStmt(CE,IL);
				Rewrite->InsertText(IL->getSourceRange().getEnd(),"/*this 0 is generate by replace get_global_id*/");

				return IL;
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

				if(arg_to_root && (fn->getNumParams() > 2))
				{
						string CLTRSinput;
						CLTRSinput += "typedef struct CLTRSin {\n";
						for(int i = 0;i<(int)fn->getNumParams()-1;i++)
						{
								CLTRSinput += fn->getParamDecl(i)->getType().getAsString();
								CLTRSinput += " ";
								CLTRSinput += fn->getParamDecl(i)->getNameAsString();
								CLTRSinput +=";\n";
						}
						CLTRSinput += "} CLTRSin:\n\n";

						outstream << CLTRSinput;
				}

				outstream << output;
				while(!outstream.eof())
				{
						outstream.getline(line,256);

						finalOStream << Modifier->replaceStringAccordingToTable(line,MainTable)<< "\n";
				}
				llvm::errs() << finalOStream.str()<<"\n\n";
		}


}

void ScriptWriter::HandleTranslationUnit() 
{
  //generate global decl string
		SourceManager * SM = &Context->getSourceManager();
		  std::string gd;
				llvm::raw_string_ostream GS(gd);
		for (vector<Decl *>::iterator it = globalDecl.begin(); it<globalDecl.end();++it)
		{
		  if(SM->getFileID((*it)->getLocation()) != SM->getMainFileID())
    continue;
		  (*it)->print(GS);
				GS<<"\n";
		}
  GS<<"\n";
  //handle special function translate
		for ( vector<CallExpr *>::iterator it=waitRewriteCallExpr.begin() ; it < waitRewriteCallExpr.end(); it++ )
		{
				llvm::errs() << Rewrite->ConvertToString(*it) <<"\n";
				specialFinalFunctionCallHandle(*it);
		}

  //output one script per function 
		for (vector<FunctionDecl *>::iterator it = functionToRewrite.begin() ; it < functionToRewrite.end(); it++)
				printScript(llvm::errs(),*it,GS.str());

  //FIXME case of output all function once

/*
			
					PrintingPolicy Policy = Context->getPrintingPolicy();
					Policy.Dump = false;                                                                                                                                                                                      
					Context->getTranslationUnitDecl()->print(llvm::errs(), Policy, 0,
					true);// */




}


