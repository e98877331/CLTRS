#ifndef __CLTRSTBL__
#define __CLTRSTBL__

#include <string>
using namespace std;

namespace CLTRS
{
enum TableType
{
 MainTable //var Decl table
};

typedef struct
{
string CLWord;
string RSWord;
} CLTRSTable;


static CLTRSTable TranslateTable[] = 
{
{"__global",""},
{"__local",""},
{"__constant",""},
{"__private",""},
{"CLTRS_TB_END","CLTRS_TB_END"}
};

}
#endif
