

__kernel void adder(__global const float* a, __global float* result)
{
int idx = get_global_id(0);
result[idx] = a[idx] * 2;
}
