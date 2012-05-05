#include <clangCL.h>

float3 gMonoMult = (float3) (0.299f, 0.587f, 0.114f);

__kernel void adder(__read_only image2d_t srcImg,sampler_t sampler, int width, int height,__write_only image2d_t dstImg)
{
float4 f4 = read_imagef(srcImg, sampler, (int2)(get_global_id(0), get_global_id(1)));

float4 Mono = (float4){0.0f, 0.0f, 0.0f, 0.0f};

Mono.xyz = dot(f4.xyz,gMonoMult);

write_imagef(dstImg, (int2)(get_global_id(0), get_global_id(1)), Mono);

}
