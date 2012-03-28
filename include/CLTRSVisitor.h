#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclGroup.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Rewriter.h"
#include "clang/Lex/Preprocessor.h"

#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

#include <sstream>
#include <string>
#include <set>
#include <map>

using namespace clang;

namespace CLTRS 
{
	class CLTRSVisitor : public RecursiveASTVisitor<CLTRSVisitor>
	{
		public:
			SourceManager *SM;


		public:
			CLTRSVisitor():SM(0){}
			


	};

}
