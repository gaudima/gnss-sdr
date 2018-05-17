#ifndef __CUDA_KERNELS_H__
#define __CUDA_KERNELS_H__

#include <glog/logging.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuComplex.h>
#include <cufft.h>
#include <cfloat>
#include <volk/volk.h>

typedef struct {
    int16_t r;
    int16_t i;
} int16c_t;

#define gpuErrchk(ans) gpuAssert((ans), __FILE__, __LINE__)
inline int gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "CUDA Error: %s(%d) %s %d\n", cudaGetErrorString(code), code, file, line);
        DLOG(ERROR) << "CUDA Error: " << cudaGetErrorString(code) << "(" << code << ")" << " " << file << " " << line;
        if (abort) exit(code);
    }
    return code;
}

#define cufftErrchk(ans) cufftAssert((ans), __FILE__, __LINE__)
inline int cufftAssert(cufftResult code, const char *file, int line, bool abort=true)
{
    if (code != CUFFT_SUCCESS)
    {
        fprintf(stderr, "CUFFT Error: code: %d %s %d\n", code, file, line);
        DLOG(ERROR) << "CUFFT Error: code: " << code << " " << file << " " << line;
        if (abort) exit(code);
    }
    return code;
}

int init_cuda();

__global__ void cuda_conj_vector(cuComplex* out, cuComplex* in, unsigned int size);

__global__ void cuda_sincos(cuComplex* out, float phase_inc, float phase, unsigned int size);

__global__ void cuda_mul_vectors(cuComplex* out, cuComplex* in1, cuComplex* in2, unsigned int size);

__global__ void cuda_convert_cshort(cuComplex* out, lv_16sc_t* in, unsigned int size);

__global__ void cuda_max_magt_sq_and_index_stage1(float* magt, int* magt_idx, cuComplex* in, unsigned int size);

__global__ void cuda_max_magt_sq_and_index_stage2(float* magt, int* magt_idx, unsigned int size);

__global__ void cuda_magt_sq_sum_stage1(float* sum, cuComplex* in, unsigned int size);

__global__ void cuda_magt_sq_sum_stage2(float* sum, unsigned int size);

__global__ void cuda_xn_resampler_xn(float* result, const float* local_code,
                                     float rem_code_phase_chips, float code_phase_step_chips,
                                     float* shifts_chips, unsigned int code_length_chips, int num_out_vectors,
                                     unsigned int num_points);

__global__ void cuda_x2_dot_prod_xn_stage1(cuComplex* result, const cuComplex* in_common,
                                           cuComplex* phase, float* in_a, int num_a_vectors,
                                           unsigned int num_points);

__global__ void cuda_x2_dot_prod_xn_stage2(cuComplex* end_result, cuComplex* result, int num_a_vectors,
                                           unsigned int num_points);

#endif //__CUDA_KERNELS_H__