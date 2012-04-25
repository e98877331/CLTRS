#ifndef __CLTRSTBL__
#define __CLTRSTBL__

#include <string>
using namespace std;
namespace clang
{
namespace CLTRS
{
enum TableType
{
 MainTable, //var Decl table
	FunctionTranlsateTable
};

typedef struct
{
string CLWord;
string RSWord;
} CLTRSTable;

typedef struct
{
string CLFunction;
string RSForm;
string args;
} CLTRSFunctionTable;

static CLTRSTable TranslateTable[] = 
{
{"__global",""},
{"__local",""},
{"__constant",""},
{"__private",""},
{"image2d_t","uchar4 *"},
{"CLTRS_TB_END","CLTRS_TB_END"}
};


static CLTRSFunctionTable FunctionTranslateTable[] =
{
{"read_imagef","rsUnpackColor8888(*%s)","1"},
{"write_imagef","*%s = rsPackColorTo8888(%s)","13"}
};


}
}
#endif
