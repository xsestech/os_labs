CMAKE_BUILD_PATH = cmake-build-linux
#CC = clang
#export CC
CHILD_EXEC_PATH = ${CMAKE_BUILD_PATH}/child/child
FREEBLOCKS_DSYM_PATH = ${CMAKE_BUILD_PATH}/liballoc-freeblocks/libliballoc-freeblocks.so
BUDDY_DSYM_PATH = ${CMAKE_BUILD_PATH}/liballoc-buddy/libliballoc-buddy.so
export CHILD_EXEC_PATH
build:
	cmake -S . -B cmake-build-linux -DCMAKE_BUILD_TYPE=Debug
	cmake --build cmake-build-linux

run_freeblocks: build
	./${CMAKE_BUILD_PATH}/alloc/alloc ${FREEBLOCKS_DSYM_PATH}

run_buddy: build
	./${CMAKE_BUILD_PATH}/alloc/alloc ${BUDDY_DSYM_PATH}

strace_buddy: build
	strace -f ./${CMAKE_BUILD_PATH}/alloc/alloc ${BUDDY_DSYM_PATH}

strace_freeblocks: build
	strace -f ./${CMAKE_BUILD_PATH}/alloc/alloc ${FREEBLOCKS_DSYM_PATH}

debug: build
	gdb ./cmake-build-linux/alloc/alloct -iex "set auto-load safe-path ."
