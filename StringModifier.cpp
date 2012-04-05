#include <sstream>

#include "llvm/Support/raw_ostream.h"

#include "include/StringModifier.h"

//using namespace std;
using namespace CLTRS;

CLTRSTable * StringModifier::getTableByTableType(TableType tt)
{
 switch(tt)
	{
   case ASQ:
			 return ASQualifiers;
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

string StringModifier::modifyString(string in,TableType tt)
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
