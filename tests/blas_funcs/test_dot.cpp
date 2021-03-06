// Copyright (c) 2017-2018 Intel Corporation
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>
#include "test_helpers.hpp"
#include "test_dot.hpp"

#include <functions/Sdot.hpp>
#include <functions/Cdotu.hpp>
#include <functions/Cdotc.hpp>

namespace iclgpu { namespace tests {

using ::testing::Combine;
using ::testing::Values;

template<>
struct func_traits<iclgpu::functions::Sdot>
{
    using data_type = float;
    static void reference(iclgpu::functions::Sdot::params& params)
    {
        double result = 0;
        for (int i = 0; i < params.n; i++)
        {
            const auto x = static_cast<double>(params.x[i * params.incx]);
            const auto y = static_cast<double>(params.y[i * params.incy]);
            result += x * y;
        }
        *params.result = static_cast<float>(result);
    }
};

using test_Sdot = test_dot<iclgpu::functions::Sdot>;

TEST_P(test_Sdot, basic)
{
    ASSERT_NO_FATAL_FAILURE(run_function<iclgpu::functions::Sdot>(params, impl_name));
    EXPECT_FLOAT_EQ(result_ref, result);
}

INSTANTIATE_TEST_CASE_P(
    S2K,
    test_Sdot,
    Combine(
        Values("naive", "opt_6"),
        Values(2 << 10),
        Values(1, 3),
        Values(1, 3)
    ),
    testing::internal::DefaultParamName<test_Sdot::ParamType> // workaround for gTest + GCC -Wpedantic incompatibility
);

INSTANTIATE_TEST_CASE_P(
    S5K13,
    test_Sdot,
    Combine(
        Values("opt_6"),
        Values((5 << 10) + 13),
        Values(1, 3),
        Values(1, 3)
    ),
    testing::internal::DefaultParamName<test_Sdot::ParamType> // workaround for gTest + GCC -Wpedantic incompatibility
);

template<>
struct func_traits<iclgpu::functions::Cdotu>
{
    using data_type = iclgpu::complex_t;
    static void reference(iclgpu::functions::Cdotu::params& params)
    {
        std::complex<double> result = 0;
        for (int i = 0; i < params.n; i++)
        {
            const std::complex<double> x(params.x[i * params.incx]);
            const std::complex<double> y(params.y[i * params.incy]);
            result += x * y;
        }
        *params.result = static_cast<iclgpu::complex_t>(result);
    }
};

using test_Cdotu = test_dot<iclgpu::functions::Cdotu>;

TEST_P(test_Cdotu, basic)
{
    ASSERT_NO_FATAL_FAILURE(run_function<iclgpu::functions::Cdotu>(params, impl_name));
    EXPECT_COMPLEX_EQ(result_ref, result);
}

INSTANTIATE_TEST_CASE_P(
    S2K,
    test_Cdotu,
    Combine(
        Values(""),
        Values(2 << 10),
        Values(1, 3),
        Values(1, 3)
    ),
    testing::internal::DefaultParamName<test_Cdotu::ParamType> // workaround for gTest + GCC -Wpedantic incompatibility
);

template<>
struct func_traits<iclgpu::functions::Cdotc>
{
    using data_type = iclgpu::complex_t;

    static void reference(iclgpu::functions::Cdotc::params& params)
    {
        std::complex<double> result = 0;
        for (int i = 0; i < params.n; i++)
        {
            const std::complex<double> x(params.x[i * params.incx]);
            const std::complex<double> y(params.y[i * params.incy]);
            result += conj(x) * y;
        }
        *params.result = static_cast<iclgpu::complex_t>(result);
    }
};

using test_Cdotc = test_dot<iclgpu::functions::Cdotc>;

TEST_P(test_Cdotc, basic)
{
    ASSERT_NO_FATAL_FAILURE(run_function<iclgpu::functions::Cdotc>(params, impl_name));
    EXPECT_COMPLEX_EQ(result_ref, result);
}

INSTANTIATE_TEST_CASE_P(
    S2K,
    test_Cdotc,
    Combine(
        Values(""),
        Values(2 << 10),
        Values(1, 3),
        Values(1, 3)
    ),
    testing::internal::DefaultParamName<test_Cdotc::ParamType> // workaround for gTest + GCC -Wpedantic incompatibility
);

}}
