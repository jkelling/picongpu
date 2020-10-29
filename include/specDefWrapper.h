#pragma once

// this test is to not break pic-build or cmake builds
#ifdef SPEC
    #define CMAKE_VERSION = NO_CMAKE_IN_SPEC
    #ifdef SPEC_CUDA
        #define CUPLA_STREAM_ASYNC_ENABLED 1
        #define ALPAKA_ACC_GPU_CUDA_ONLY_MODE
        #include <cupla/config/GpuCudaRt.hpp>
    #elif defined SPEC_HIP
        #define CUPLA_STREAM_ASYNC_ENABLED 1
        #define ALPAKA_ACC_GPU_HIP_ONLY_MODE
        #include <cupla/config/GpuHipRt.hpp>
    #elif defined SPEC_OPENMP
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #include <cupla/config/CpuOmp2Blocks.hpp>
    #elif defined SPEC_OPENMP_TARGET
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #include <cupla/config/AnyOmp5.hpp>
    #elif defined SPEC_CPP11THREADS
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #include <cupla/config/CpuThreads.hpp>
    #elif defined SPEC_OPENACC
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #include <cupla/config/AnyOacc.hpp>
    #else
        #define CUPLA_STREAM_ASYNC_ENABLED 0
        #include <cupla/config/CpuSerial.hpp>
    #endif

    #ifdef SPEC_ALPAKA_OFFLOAD_MAX_BLOCK_SIZE
        #define ALPAKA_OFFLOAD_MAX_BLOCK_SIZE SPEC_ALPAKA_OFFLOAD_MAX_BLOCK_SIZE
    #endif
    #ifdef SPEC_ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KIB
        #define ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KIB SPEC_ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KIB
    #endif

    #include <alpaka/core/BoostPredef.hpp>
    #if (BOOST_LANG_CUDA || BOOST_COMP_CUDA) && ! defined SPEC_CUDA
        #error "CUDA compiler must only be used with -DSPEC_CUDA ."
    #endif
    #if (BOOST_LANG_HIP || BOOST_COMP_HIP) && ! defined SPEC_HIP
        #error "HIP compiler must only be used with -DSPEC_HIP ."
    #endif
#endif
