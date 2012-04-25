int get_global_id(int index); 
__kernel void adder(__global const float* a, __global float* result)
{
int idx = get_global_id(get_global_id(get_global_id(0)));
}
