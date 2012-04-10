/* this is header of RenderScript Script Writer */

#ifndef __SCRIPTWRITER__
#define __SCRIPTWRITER__

#include <map>
#include <vector>

#include "clang/AST/AST.h"

#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Rewriter.h"

#include "StringModifier.h"
using namespace clang;
//using namespace CLTRS;

namespace CLTRS
{

class ScriptWriter
{
private:
Rewriter *Rewrite;
StringModifier Modifier;
ASTContext *Context;

std::map<std::string,std::string> distributeUnit;

public:
ScriptWriter(){}

inline void setRewriter(Rewriter *R)
{ Rewrite = R;}

inline void setContext(ASTContext *C)
{ Context = C;}

bool handleFuncDefinition(FunctionDecl *FD);
bool handleFunctionNameAndParameter(FunctionDecl *FD,bool toRoot = false);


Stmt *handleStmt(Stmt *ST);



Stmt *RewriteArraySubscript(ArraySubscriptExpr *ASE);

Stmt *RewriteCallExpr(CallExpr * CE);

};



}



#endif

