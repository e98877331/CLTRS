//this class changes opencl names to renderscript names

#ifndef __STRINGMODIFIER__
#define __STRINGMODIFIER__


#include "CLTRSTable.h"


namespace CLTRS
{
		class StringModifier
		{
				public:
						StringModifier(){}
						string modifyString(string in,TableType tt);
    private:
						string replaceTokAccordingTable(string s, CLTRSTable * tb)
						{
						bool temp;
						return replaceTokAccordingTable(s,tb,temp);

						}
						string replaceTokAccordingTable(string s, CLTRSTable * tb,bool &result);
								CLTRSTable * getTableByTableType(TableType tt);

		};




}

#endif

