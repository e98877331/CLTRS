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

using namespace clang;
//#include "PluginCLTRSAction.h"

namespace CLTRS {

				class CLTRSConsumer : public ASTConsumer {
								public:
												virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
																for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
																				const Decl *D = *i;
#if 1													
																	   	D->print(llvm::errs());
																				llvm::errs() << "\ntop-level-decl type: \"" << D->getDeclKindName() << "\"\n";
																				if (const NamedDecl *ND = dyn_cast<NamedDecl>(D))
																								//       llvm::errs() << "top-level-decl: \"" << ND->getName()<< " ||| " << "\"\n";
																								llvm::errs() << "top-level-decl: \"" << ND->getNameAsString() << "\"\n\n";
#endif
}

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
				};



}



#endif
