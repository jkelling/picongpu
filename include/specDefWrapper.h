#pragma once

// this test is to not break pic-build or cmake builds
#ifndef CMAKE_VERSION
    #define CMAKE_VERSION = NO_CMAKE_IN_SPEC
    #ifdef SPEC_CUDA
        #define CUPLA_STREAM_ASYNC_ENABLED 1
        #define ALPAKA_ACC_GPU_CUDA_ENABLED 1
        #define ALPAKA_ACC_GPU_CUDA_ONLY_MODE
    #elif defined SPEC_HIP
        #define CUPLA_STREAM_ASYNC_ENABLED 1
        #define ALPAKA_ACC_GPU_HIP_ENABLED 1
        #define ALPAKA_ACC_GPU_HIP_ONLY_MODE
    #elif defined SPEC_OPENMP
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #define ALPAKA_ACC_CPU_B_OMP2_T_SEQ_ENABLED 1
        // #define ALPAKA_ACC_CPU_B_SEQ_T_OMP2_ENABLED
    #elif defined SPEC_OPENMP_TARGET
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #define ALPAKA_ACC_ANY_BT_OMP5_ENABLED 1
    #elif defined SPEC_CPP11THREADS
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #define ALPAKA_ACC_CPU_B_SEQ_T_THREADS_ENABLED 1
    #elif defined SPEC_OPENACC
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #define ALPAKA_ACC_ANY_BT_OACC_ENABLED 1
    #else
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #define ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED 1
    #endif

    #ifdef SPEC_ALPAKA_OFFLOAD_MAX_BLOCK_SIZE
        #define ALPAKA_OFFLOAD_MAX_BLOCK_SIZE SPEC_ALPAKA_OFFLOAD_MAX_BLOCK_SIZE
    #endif
    #ifdef SPEC_ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KB
        #define ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KB SPEC_ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KB
    #endif
#endif
