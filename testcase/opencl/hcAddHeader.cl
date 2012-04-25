
typedef __attribute__((ext_vector_type(2)))  int int2;
typedef __attribute__((ext_vector_type(3)))  float float3;
typedef __attribute__((ext_vector_type(4)))  float float4;
typedef void* image2d_t;
typedef void* sampler_t;


float4 read_imagef (	image2d_t image,
 	sampler_t sampler,
	 	int2 coord);


void write_imagef (	image2d_t image,
 	int2 coord,
	 	float4 color);

float dot(float3,float3);

int get_global_id(int index);



__kernel void adder(__read_only image2d_t srcImg,__write_only image2d_t dstImg,sampler_t sampler, int width, int height)
{
float3 gMonoMult = (float3) (0.299f, 0.587f, 0.114f);

float4 f4 = read_imagef(srcImg, sampler, (int2)(get_global_id(0), get_global_id(1)));

float4 Mono = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

Mono.xyz = dot(f4.xyz,gMonoMult);

write_imagef(dstImg, (int2)(get_global_id(0), get_global_id(1)), Mono);

}
