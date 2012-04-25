#!/bin/bash
#usage: ./CLTRStest.ch [package name] [-to-root] .clfile
CLTRSInclude=/home/michael/NewDisk/llvmN/llvm/tools/clang/examples/CLTRS/CLinclude/
CLTRSso=/home/michael/NewDisk/llvmN/llvm/Debug+Asserts/lib/libCLTRS.so



if [ "$3" != "" ]; then

		clang -cc1 -x cl -triple i386-pc-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name tt.cpp -mrelocation-model static -mdisable-fp-elim -masm-verbose -mconstructor-aliases -target-cpu pentium4 -momit-leaf-frame-pointer  -resource-dir /opt/llvmMar12/bin/../lib/clang/3.1 -fmodule-cache-path /var/tmp/clang-module-cache -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4 -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4/i486-linux-gnu -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4/backward -internal-isystem /usr/local/include -internal-isystem /opt/llvmMar12/bin/../lib/clang/3.1/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -I"$CLTRSInclude"  -fdeprecated-macro -fdebug-compilation-dir /home/michael/NewDisk/llvmN/llvm/tools/clang/examples/PrintFunctionNames -ferror-limit 19 -fmessage-length 207 -mstackrealign -fgnu-runtime -fobjc-runtime-has-arc -fobjc-runtime-has-weak -fobjc-fragile-abi -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -load "$CLTRSso" -plugin CLTRS -plugin-arg-CLTRS -package -plugin-arg-CLTRS "$1" -plugin-arg-CLTRS "$2" "$3"


elif [ "$2" != "" ]; then

echo "in 1"

		clang -cc1 -x cl -triple i386-pc-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name tt.cpp -mrelocation-model static -mdisable-fp-elim -masm-verbose -mconstructor-aliases -target-cpu pentium4 -momit-leaf-frame-pointer  -resource-dir /opt/llvmMar12/bin/../lib/clang/3.1 -fmodule-cache-path /var/tmp/clang-module-cache -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4 -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4/i486-linux-gnu -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4/backward -internal-isystem /usr/local/include -internal-isystem /opt/llvmMar12/bin/../lib/clang/3.1/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -I"$CLTRSInclude"  -fdeprecated-macro -fdebug-compilation-dir /home/michael/NewDisk/llvmN/llvm/tools/clang/examples/PrintFunctionNames -ferror-limit 19 -fmessage-length 207 -mstackrealign -fgnu-runtime -fobjc-runtime-has-arc -fobjc-runtime-has-weak -fobjc-fragile-abi -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -load "$CLTRSso" -plugin CLTRS -plugin-arg-CLTRS -package -plugin-arg-CLTRS "$1" "$2"
else

echo "in 2"
		clang -cc1 -x cl -triple i386-pc-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name tt.cpp -mrelocation-model static -mdisable-fp-elim -masm-verbose -mconstructor-aliases -target-cpu pentium4 -momit-leaf-frame-pointer  -resource-dir /opt/llvmMar12/bin/../lib/clang/3.1 -fmodule-cache-path /var/tmp/clang-module-cache -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4 -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4/i486-linux-gnu -internal-isystem /usr/lib/gcc/i486-linux-gnu/4.4/../../../../include/c++/4.4/backward -internal-isystem /usr/local/include -internal-isystem /opt/llvmMar12/bin/../lib/clang/3.1/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -I"$CLTRSInclude"  -fdeprecated-macro -fdebug-compilation-dir /home/michael/NewDisk/llvmN/llvm/tools/clang/examples/PrintFunctionNames -ferror-limit 19 -fmessage-length 207 -mstackrealign -fgnu-runtime -fobjc-runtime-has-arc -fobjc-runtime-has-weak -fobjc-fragile-abi -fcxx-exceptions -fexceptions -fdiagnostics-show-option -fcolor-diagnostics -load "$CLTRSso" -plugin CLTRS -plugin-arg-CLTRS -package -plugin-arg-CLTRS test.tes.tt "$1"

fi
