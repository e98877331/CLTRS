/* this is header of RenderScript Script Writer */

#ifndef __SCRIPTWRITER__
#define __SCRIPTWRITER__

#include "clang/AST/AST.h"

#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Rewriter.h"

using namespace clang;
//using namespace CLTRS;

namespace CLTRS
{

class ScriptWriter
{
private:
Rewriter Rewrite;


public:
ScriptWriter(){}

inline void setRewriter(Rewriter &R)
{ Rewrite = R;}


bool handleFuncDefinition(FunctionDecl *FD);
bool handleFunctionNameAndParameter(FunctionDecl *FD);


void handleStmt(Stmt *ST);

};



}



#endif

