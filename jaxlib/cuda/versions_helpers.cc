/* Copyright 2023 The JAX Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "jaxlib/cuda/versions_helpers.h"

#include <cstddef>
#include <stdexcept>

#include "jaxlib/gpu/gpu_kernel_helpers.h"
#include "jaxlib/gpu/vendor.h"

namespace jax::cuda {

#if CUDA_VERSION < 11080
#error "JAX requires CUDA 11.8 or newer."
#endif  // CUDA_VERSION < 11080

int CudaRuntimeGetVersion() {
  int version;
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cudaRuntimeGetVersion(&version)));
  return version;
}

int CudaDriverGetVersion() {
  int version;
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cudaDriverGetVersion(&version)));
  return version;
}

uint32_t CuptiGetVersion() {
  uint32_t version;
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cuptiGetVersion(&version)));
  return version;
}

int CufftGetVersion() {
  int version;
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cufftGetVersion(&version)));
  return version;
}

int CusolverGetVersion() {
  int version;
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cusolverGetVersion(&version)));
  return version;
}

int CublasGetVersion() {
  int version;
  // NVIDIA promise that it's safe to parse nullptr as the handle to this
  // function.
  JAX_THROW_IF_ERROR(
      JAX_AS_STATUS(cublasGetVersion(/*handle=*/nullptr, &version)));
  return version;
}

int CusparseGetVersion() {
  // cusparseGetVersion is unhappy if passed a null library handle. But
  // cusparseGetProperty doesn't require one.
  int major, minor, patch;
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cusparseGetProperty(MAJOR_VERSION, &major)));
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cusparseGetProperty(MINOR_VERSION, &minor)));
  JAX_THROW_IF_ERROR(JAX_AS_STATUS(cusparseGetProperty(PATCH_LEVEL, &patch)));
  return major * 1000 + minor * 100 + patch;
}
size_t CudnnGetVersion() {
  size_t version = ::cudnnGetVersion();
  // If the cudnn stub in TSL can't find the library, it will use a dummy stub
  // that returns 0, since cudnnGetVersion() cannot fail.
  if (version == 0) {
    throw std::runtime_error("cuDNN not found.");
  }
  return version;
}

}  // namespace jax::cuda