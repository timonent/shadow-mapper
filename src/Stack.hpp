#pragma once

#include "AlignedResource.hpp"

#include <optional>
#include <type_traits>
#include <utility>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Because std::stack is horrendously slow and std::pmr::deque with
// a std::pmr::monotonic_buffer_resource is not always applicable as a remedy.

template<typename T>
class Stack
{
public:
    Stack(size_t maxSize) : m_maxSize{maxSize}, m_underlying{AlignedResource<T>(m_maxSize)} {}
    ~Stack() = default;

    Stack(const Stack&) = default;
    Stack& operator=(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(Stack&&) = default;

    [[nodiscard]] size_t getSize() const noexcept { return m_size; }
    [[nodiscard]] size_t getMaxSize() const noexcept { return m_maxSize; }

    [[nodiscard]] std::optional<std::reference_wrapper<const T>> top() const noexcept
    {
        if (m_size == 0) [[unlikely]]
            return std::nullopt;

        return m_underlying.at(m_size - 1);
    }

    bool pop() noexcept
    {
        if (m_size == 0) [[unlikely]]
            return false;

        --m_size;
        return true;
    }

    bool push(const T& item) noexcept
    requires std::copyable<T>
    {
        if (m_size == m_maxSize) [[unlikely]]
            return false;

        m_underlying[m_size++] = item;
        return true;
    }

    bool push(T&& item) noexcept
    requires std::movable<T>
    {
        if (m_size == m_maxSize) [[unlikely]]
            return false;

        m_underlying[m_size++] = std::move(item);
        return true;
    }

    template<typename... Args>
    requires std::is_constructible_v<T, Args...>
    bool emplace(Args&& ...args) noexcept
    {
        if (m_size == m_maxSize) [[unlikely]]
            return false;

        new (&m_underlying[m_size++]) T(std::forward<Args>(args)...);
        return true;
    }

private:
    size_t m_maxSize;
    AlignedResource<T> m_underlying;
    size_t m_size = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
