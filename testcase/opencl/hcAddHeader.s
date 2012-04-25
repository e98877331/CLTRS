; ModuleID = 'hcAddHeader.cl'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

define void @adder(i8* %srcImg, i8* %dstImg, i8* %sampler, i32 %width, i32 %height) nounwind {
entry:
  %call = call i32 @get_global_id(i32 0) nounwind
  %vecinit = insertelement <2 x i32> undef, i32 %call, i32 0
  %call2 = call i32 @get_global_id(i32 1) nounwind
  %vecinit3 = insertelement <2 x i32> %vecinit, i32 %call2, i32 1
  %0 = bitcast <2 x i32> %vecinit3 to x86_mmx
  %call4 = call <4 x float> @read_imagef(i8* %srcImg, i8* %sampler, x86_mmx %0) nounwind
  %1 = shufflevector <4 x float> %call4, <4 x float> undef, <3 x i32> <i32 0, i32 1, i32 2>
  %call6 = call float @dot(<3 x float> %1, <3 x float> <float 0x3FD322D0E0000000, float 0x3FE2C8B440000000, float 0x3FBD2F1AA0000000>) nounwind
  %2 = insertelement <3 x float> undef, float %call6, i32 0
  %3 = shufflevector <3 x float> %2, <3 x float> undef, <4 x i32> <i32 0, i32 0, i32 0, i32 undef>
  %4 = shufflevector <4 x float> <float undef, float undef, float undef, float 0.000000e+00>, <4 x float> %3, <4 x i32> <i32 4, i32 5, i32 6, i32 3>
  %call8 = call i32 @get_global_id(i32 0) nounwind
  %vecinit9 = insertelement <2 x i32> undef, i32 %call8, i32 0
  %call10 = call i32 @get_global_id(i32 1) nounwind
  %vecinit11 = insertelement <2 x i32> %vecinit9, i32 %call10, i32 1
  %5 = bitcast <2 x i32> %vecinit11 to x86_mmx
  call void @write_imagef(i8* %dstImg, x86_mmx %5, <4 x float> %4) nounwind
  ret void
}

declare <4 x float> @read_imagef(i8*, i8*, x86_mmx)

declare i32 @get_global_id(i32)

declare float @dot(<3 x float>, <3 x float>)

declare void @write_imagef(i8*, x86_mmx, <4 x float>)

!opencl.kernels = !{!0}

!0 = metadata !{void (i8*, i8*, i8*, i32, i32)* @adder}
