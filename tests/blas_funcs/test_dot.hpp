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

#pragma once
#include <gtest/gtest.h>
#include "test_helpers.hpp"

namespace iclgpu { namespace tests {

template<class Func>
struct test_dot : test_base_VV<Func>
{
    using data_type = typename test_base_VV<Func>::data_type;

    data_type result;
    data_type result_ref;

    typename Func::params get_params() override
    {
        return
        {
            this->num,
            this->x.data(),
            this->incx,
            this->y.data(),
            this->incy,
            &result
        };
    }

    typename Func::params get_params_ref() override
    {
        return
        {
            this->num,
            this->x.data(),
            this->incx,
            this->y.data(),
            this->incy,
            &result_ref
        };
    }
};

}}
