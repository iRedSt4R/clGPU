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
#include <memory>
#include <unordered_map>

namespace iclgpu
{
/// @addtogroup context Context management
/// @{

/// @brief Base class for service lookup containers
template <class C>
class container
{
    class element_base {};
    using element_id_t = char;

public:
    /// @brief Context holder base class
    class holder
    {
    public:
        using container_type = C;

        explicit holder(const std::shared_ptr<C>& ctx)
            : _context(ctx) {}

        virtual ~holder() = default;
        /// @brief Get container/context reference
        std::shared_ptr<C> context() const { return _context; }
    private:
        std::shared_ptr<C> _context;
    };

    /// @brief Base class for objects onwed by container/context
    template <class T>
    class element : public element_base
    {
    public:
        using container_type = C;

        explicit element(const std::shared_ptr<C>& ctx)
            : _context(ctx) {}

        virtual ~element() = default;
        /// @brief Get container/context reference
        std::shared_ptr<C> context() const { return _context.lock(); }
    private:
        std::weak_ptr<C> _context;
        friend class container;
        static element_id_t* id() { return &_id; }
        static element_id_t  _id;

    public:
                    element(const element&   other) = delete;
                    element(element&&        other) noexcept = delete;
        element& operator=(const element& other) = delete;
        element& operator=(element&&      other) noexcept = delete;
    };

    /// @brief Construct or get an object owned by container (singleton lifetime).
    /// @tparam T inherited from class element
    template <class T, typename ... Args>
    typename std::enable_if<std::is_base_of<element_base, T>::value, std::shared_ptr<T>>::type
    get(Args&&...args)
    {
        auto key = T::id();
        auto it  = _elements.find(key);
        if (it != _elements.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }
        auto result = std::make_shared<T>(static_cast<C*>(this)->shared_from_this(), std::forward<Args>(args)...);
        _elements.insert(make_pair(key, result));
        return result;
    }

    /// @brief Constructs an object is initialized but not owned by container
    template <typename T, typename ... Args>
    typename std::enable_if<std::is_base_of<holder, T>::value, std::shared_ptr<T>>::type
    get(Args&&...args)
    {
        return std::make_shared<T>(static_cast<C*>(this)->shared_from_this(), std::forward<Args>(args)...);
    }

private:
    std::unordered_map<element_id_t*, std::shared_ptr<element_base>> _elements;
};

/// @}
} // namespace iclgpu

/// @brief Define element class id
#define DEFINE_CLASS_ID(T) template<> template<> iclgpu::container<T::container_type>::element_id_t T::container_type::element<T>::_id = 0;
