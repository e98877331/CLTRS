/* this is header of RenderScript Script Writer */

#ifndef __SCRIPTWRITER__
#define __SCRIPTWRITER__

#include <map>
#include <vector>

#include "clang/AST/AST.h"

#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Rewriter.h"


//#include "include/CLTRS.h"
#include "Config.h"
using namespace clang;
using namespace CLTRS;


namespace CLTRS
{
class StringModifier;
class CLTRSConsumer;
class ScriptWriter
{
private:
Rewriter *Rewrite;
StringModifier *Modifier;
ASTContext *Context;

CLTRSConsumer *CLTRS;

std::map<std::string,std::string> distributeUnit;
std::vector<CallExpr *> waitRewriteCallExpr;


bool arg_to_root;

public:
ScriptWriter(){}

/*
inline void setRewriter(Rewriter *R)
{ Rewrite = R;}

inline void setContext(ASTContext *C)
{Context = C;}

inline void setArgToRoot(bool b)
{arg_to_root = b;}
*/
void initialize(CLTRSConsumer *consumer);


bool handleFuncDefinition(FunctionDecl *FD);
bool handleFunctionNameAndParameter(FunctionDecl *FD,bool toRoot);


void HandleTranslationUnit();
Stmt *handleStmt(Stmt *ST);



Stmt *RewriteArraySubscript(ArraySubscriptExpr *ASE);

Stmt *RewriteCallExpr(CallExpr * CE);

Stmt *RewirteInitListExpr(InitListExpr *ILE);
Stmt *RewriteBinaryOperator(BinaryOperator *BO);
};



}



#endif

