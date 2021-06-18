#pragma once

// this test is to not break pic-build or cmake builds
#ifdef SPEC
    #ifndef ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KIB
        // the internal default of alpaka changed upstream to 47u, this is too
        // much for IBM XL on nvptx and not required by picongpu. Setting spec
        // default to the old value, but allowing override via flag because
        // picongpu can live with less, too.
        #define ALPAKA_BLOCK_SHARED_DYN_MEMBER_ALLOC_KIB 30u
    #endif

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
    #else
        #define ALPAKA_OFFLOAD_MAX_BLOCK_SIZE 1
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

    #ifndef SPEC_NO_PIC_USE_MALLOCMC
        #define PIC_USE_MALLOCMC
    #endif
#endif
