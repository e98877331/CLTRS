#include <string>
#include "include/CLTRS.h"
#include "include/ScriptWriter.h"
#include "include/StringModifier.h"
using namespace clang::CLTRS;
using namespace std;


void CLTRSConsumer::Initialize(ASTContext &Ctx)
{
  SWriter = new ScriptWriter();
		Modifier = new StringModifier();
		Context = &Ctx;
		SM = &Context->getSourceManager();
		//TUDecl = Context->getTranslationUnitDecl();

		MainFileID = SM->getMainFileID();
		const llvm::MemoryBuffer *MainBuf = SM->getBuffer(MainFileID);
		MainFileStart = MainBuf->getBufferStart();
		MainFileEnd = MainBuf->getBufferEnd();

		Rewrite.setSourceMgr(Context->getSourceManager(), Context->getLangOptions());
  SWriter->initialize(this);
  //SWriter.setRewriter(&Rewrite);
		//SWriter.setContext(Context);



/*
    if (RewriteBuffer const *RewriteBuf =
				          Rewrite.getRewriteBufferFor(MainFileID)) {
														    llvm::errs() << "Rewriting...\n";
																llvm::errs() << std::string(RewriteBuf->begin(), RewriteBuf->end());
																}
																		    
 */
}


void CLTRSConsumer::HandleTranslationUnit(ASTContext &Context) {

    SWriter->HandleTranslationUnit();
						  //llvm::errs()<<"handling translation unit \n";
																
								//pritn all type test
/*								llvm::errs() << " In handleTranslationUnit dump\n";
								for(ASTContext::type_iterator I = Context.types_begin(),E = Context.types_end();I != E; I++)
										(*I)->dump();
										*/
						} 


bool CLTRSConsumer::HandleTopLevelDecl(DeclGroupRef DG) 
{ 
/*
										  Rewrite.InsertText(SM->getLocForStartOfFile(MainFileID), Preamble, false);
*/

		for (DeclGroupRef::iterator I = DG.begin(), E = DG.end(); I != E; ++I) {
				HandleTopLevelSingleDecl(*I);
		}

		/*    
								for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
								const Decl *D = *i;
								D->print(llvm::errs());
								llvm::errs() << "\ntop-level-decl type: \"" << D->getDeclKindName() << "\"\n";                                                                                                                   
								if (const NamedDecl *ND = dyn_cast<NamedDecl>(D))
//       llvm::errs() << "top-level-decl: \"" << ND->getName()<< " ||| " << "\"\n";
llvm::errs() << "top-level-decl: \"" << ND->getNameAsString() << "\"\n\n";
}
//*/
return true;
}

#if 0
virtual void HandleTranslationUnit(ASTContext &Context) {
		//pritn all type test
		llvm::errs() << " In handleTranslationUnit dump\n";
		for(ASTContext::type_iterator I = Context.types_begin(),E = Context.types_end();I != E; I++)
				(*I)->dump();
} 
#endif           
void CLTRSConsumer::HandleTopLevelSingleDecl(Decl *D)
{
		if (FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
				SWriter->handleFuncDefinition(FD);
		}
		else
		{
    SWriter->recordGlobalDecl(D);

		}


}
//unused function should be deleted
void CLTRSConsumer::HandleFuncDefinition(FunctionDecl *FD)
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
						HandleStmt(ST);
				}
		}

}

//unused function should be deleted
void CLTRSConsumer::HandleStmt(Stmt *ST)
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
		//  ST->dump();

}
