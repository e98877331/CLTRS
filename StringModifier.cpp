#include <sstream>
#include <stdio.h>
#include "llvm/Support/raw_ostream.h"

#include "include/StringModifier.h"

using namespace std;
using namespace clang;
using namespace clang::CLTRS;

//transfer special opencl function to renderscript
bool StringModifier::getModifiedFunctionString(CallExpr *CE,string &out)
{
		char temp[100];
		
		for(unsigned int i = 0;i< sizeof(FunctionTranslateTable)/sizeof(CLTRSFunctionTable);i++)
		{
	   if(!(CE->getDirectCallee()->getNameInfo().getAsString().compare(FunctionTranslateTable[i].CLFunction)))
				{
	     CLTRSFunctionTable entry = FunctionTranslateTable[i];			  
				  if(entry.args.length() == 1)
						{ 
      sprintf(temp,entry.RSForm.c_str(), toArg(entry.args[0],CE).c_str());
						}
						else if(FunctionTranslateTable[i].args.length() == 2)
						{
						sprintf(temp ,entry.RSForm.c_str() ,toArg(entry.args[0],CE).c_str() ,toArg(entry.args[1],CE).c_str());
						}
						else if(FunctionTranslateTable[i].args.length() == 3)
						{
      sprintf(temp,entry.RSForm.c_str(),toArg(entry.args[0],CE).c_str(),toArg(entry.args[1],CE).c_str(),toArg(entry.args[2],CE).c_str());
						}
      string o(temp);
      out = o;
						return true;
				}
				 
		}
//case of not HIT table entry
return false;
}


CLTRSTable * StringModifier::getTableByTableType(TableType tt)
{
 switch(tt)
	{
   case MainTable:
			 return TranslateTable;
		 default:
			assert("no such table type ");

	}

}

string StringModifier::replaceTokAccordingTable(string s, CLTRSTable * tb,bool &result)
{
  for(int i = 0;(tb[i].CLWord.compare("CLTRS_TB_END"));i++)
		{
		 if(!s.compare(tb[i].CLWord))
			{
			 result = true;
			 return tb[i].RSWord;
			}
		}
  result = false;
		return s;
}

string StringModifier::replaceStringAccordingToTable(string in,TableType tt)
{
		istringstream iss(in);
  stringstream outss;
		CLTRSTable * table = getTableByTableType(tt);

		do
		{
				string sub;
				iss >> sub;
    
    sub = replaceTokAccordingTable(sub,table);

				outss << sub;
				
				if(sub.compare(""))
			 outss 	<< " ";
//				llvm::errs() << "Substring: "<<sub<<" | "<< replaceTokAccordingTable(sub,table) << "\n";
		} while (iss);

   // llvm::errs() << in << " | " << outss.str() << "\n";

		return outss.str();
}
