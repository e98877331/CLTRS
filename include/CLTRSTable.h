#ifndef __CLTRSTBL__
#define __CLTRSTBL__

#include <string>
using namespace std;

namespace CLTRS
{
enum TableType
{
 ASQ //ASQualifiers
};

typedef struct
{
string CLWord;
string RSWord;
} CLTRSTable;


//address space qualifiers
static CLTRSTable ASQualifiers[] = 
{
{"__global",""},
{"__local",""},
{"__constant",""},
{"__private",""},
{"CLTRS_TB_END","CLTRS_TB_END"}
};

}
#endif
