/*
Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**
Testcase Scenarios :
1) Test getting context of default stream / per thread stream, get the context of a stream created
  from a different context than the current
2) Negative tests for the hipStreamGetCtx api
*/

#include <hip_test_common.hh>

TEST_CASE("Unit_hipStreamGetCtx_BasicFunctionalities") {
  HIP_CHECK(hipFree(nullptr)); // Initialize the runtime API, required on NVIDIA platform
  hipStream_t stream;
  hipCtx_t ctx;

  SECTION("Get context of default stream and per thread streams") {
    REQUIRE(hipStreamGetCtx(0, &ctx) == hipSuccess);
    REQUIRE(hipStreamGetCtx(hipStreamPerThread, &ctx) == hipSuccess);
  }

  SECTION("Get context of stream that was created from a different context") {
    hipDevice_t device;
    hipCtx_t new_context;
    HIP_CHECK(hipDeviceGet(&device, 0));
    HIP_CHECK(hipCtxCreate(&new_context, 0, device));
    HIP_CHECK(hipStreamCreate(&stream));
    HIP_CHECK(hipCtxPopCurrent(nullptr));
    HIP_CHECK(hipStreamGetCtx(stream, &ctx));
    REQUIRE(new_context == ctx);

    HIP_CHECK(hipStreamDestroy(stream));
    HIP_CHECK(hipCtxDestroy(new_context));
  }
}

TEST_CASE("Unit_hipStreamGetCtx_Negative") {
  HIP_CHECK(hipFree(nullptr)); // Initialize the runtime API, required on NVIDIA platform
  hipStream_t stream;
  hipCtx_t ctx;

  SECTION("Get context of an invalid stream") {
    stream = (hipStream_t)-1;
    REQUIRE(hipStreamGetCtx(stream, &ctx) == hipErrorInvalidHandle);
  }
  SECTION("Pass a nullptr to the ctx parameter") {
    REQUIRE(hipStreamGetCtx(0, nullptr) == hipErrorInvalidValue);
  }
}

