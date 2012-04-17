#define FLT_DIG  6
#define FLT_MANT_DIG 24
#define FLT_MAX_10_EXP  +38
#define FLT_MAX_EXP +128
#define FLT_MIN_10_EXP  -37
#define FLT_MIN_EXP -125
#define FLT_RADIX  2
#define FLT_MAX 0x1.fffffep127f
#define FLT_MIN 0x1.0p-126f
#define FLT_EPSILON 0x1.0p-23f

typedef unsigned int uint;
typedef int size_t;

typedef __attribute__((ext_vector_type(2)))  int int2;                                                                                                                                                         
typedef __attribute__((ext_vector_type(3)))  float float3;
typedef __attribute__((ext_vector_type(4)))  float float4;
typedef void* image2d_t;
typedef void* sampler_t;
float4 read_imagef ( image2d_t image,sampler_t sampler,int2 coord);
void write_imagef ( image2d_t image,int2 coord,float4 color);
float dot(float3,float3);
int get_global_id(int index);

