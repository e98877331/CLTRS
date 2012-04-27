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

//#include "include/ScriptWriter.h"
#include <string>
#include <sstream>
using namespace std;
using namespace clang;
//#include "PluginCLTRSAction.h"
namespace clang
{
namespace CLTRS {
  class ScriptWriter;
		class StringModifier;
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

      ScriptWriter *SWriter;
						StringModifier *Modifier;

      //arguments from plugin
      string arg_package;
						bool arg_to_root;

				public:
				 
					void setPackage(string s){arg_package = s;}
					void setToRoot(bool b){arg_to_root = b;}
					
     
					inline Rewriter *getRewriter(){return &Rewrite;}
					inline ASTContext *getASTContext(){return Context;}
					inline string getArgPackage(){return arg_package;}
					inline bool getArgToRoot(){return arg_to_root;}
     inline FileID getMainFileID(){return MainFileID;}

					virtual void Initialize(ASTContext &Ctx); 
					virtual bool HandleTopLevelDecl(DeclGroupRef DG);
       
 
						virtual void HandleTranslationUnit(ASTContext &Context); 											
   private:

						void HandleTopLevelSingleDecl(Decl *D);
						void HandleFuncDefinition(FunctionDecl *FD);
						void HandleStmt(Stmt *ST);

		};



}

}
#endif
