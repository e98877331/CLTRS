//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//
#ifndef __CLTRS__
#define __CLTRS__

#include "clang/Frontend/FrontendPluginRegistry.h"                                                                                                                                                             
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/AST.h"
#include "clang/Frontend/CompilerInstance.h"                                                                                                                                                                   
#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Rewriter.h"


using namespace clang;
//#include "PluginCLTRSAction.h"

namespace CLTRS {

		class CLTRSConsumer : public ASTConsumer {
    private:
						Rewriter Rewrite;
						//Diagnostic &Diags;
						//LangOptions const &LangOpts;
						//Preprocessor& PP;

						ASTContext *Context;
						SourceManager *SM;
						TranslationUnitDecl *TUDecl;
						FileID MainFileID;
						char const *MainFileStart, *MainFileEnd;




				public:
					virtual void Initialize(ASTContext &Ctx); 
					virtual bool HandleTopLevelDecl(DeclGroupRef DG);
       
#if 0
						virtual void HandleTranslationUnit(ASTContext &Context) {
								//pritn all type test
								llvm::errs() << " In handleTranslationUnit dump\n";
								for(ASTContext::type_iterator I = Context.types_begin(),E = Context.types_end();I != E; I++)
										(*I)->dump();
						} 
#endif											
   private:

						void HandleTopLevelSingleDecl(Decl *D);
						void HandleFuncDefinition(FunctionDecl *FD);
						void HandleStmt(Stmt *ST);

		};



}



#endif
