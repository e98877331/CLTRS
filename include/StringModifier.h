//this class changes opencl names to renderscript names

#ifndef __STRINGMODIFIER__
#define __STRINGMODIFIER__


#include "CLTRSTable.h"
#include "clang/AST/AST.h"
#include "clang/Frontend/CompilerInstance.h" 

#include "clang/Rewrite/Rewriter.h"
namespace clang
{
namespace CLTRS
{
		class StringModifier
		{
				public:
						StringModifier(){}
						string replaceStringAccordingToTable(string in,TableType tt);

    
     bool getModifiedFunctionString(CallExpr *CE,string &out);
    private:
				 Rewriter Rewrite;
				 //change charactor to integer
	    string toArg(char c,CallExpr *CE)
					{
					return Rewrite.ConvertToString(CE->getArg((int)c-49));		 
					}
	  
						string replaceTokAccordingTable(string s, CLTRSTable * tb)
						{
						bool temp;
						return replaceTokAccordingTable(s,tb,temp);

						}
						string replaceTokAccordingTable(string s, CLTRSTable * tb,bool &result);
								CLTRSTable * getTableByTableType(TableType tt);

		};




}
}
#endif

