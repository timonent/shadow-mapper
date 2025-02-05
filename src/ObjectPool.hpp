#pragma once

#include "Handle.hpp"
#include "Stack.hpp"
#include "common.hpp"

#include <memory>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Inspired by: https://twitter.com/SebAaltonen/status/1535176343847043072.

//template<std::default_initializable T>
template<typename T>
requires (std::default_initializable<T>)
class ObjectPool
{
public:
    explicit ObjectPool(size_t size = OBJECT_POOL_INIT_SIZE)
        : m_size{size},
          m_freeList{size}
    {
        m_pool.resize(m_size);
        m_generations.resize(m_size);

        for (size_t idx : stdv::iota(0u, m_size) | stdv::reverse) {
            m_freeList.push(idx);
        }
    }

    ~ObjectPool()
    {
        if constexpr (requires { T::freeResources(); }) {
            for (const T& item : m_pool) {
                item.freeResources();
            }
        }
    }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = default;
    ObjectPool& operator=(ObjectPool&&) = default;

    [[nodiscard]] size_t size() { return m_size; }

    template<typename... Args>
    requires std::constructible_from<T, Args...>
    [[nodiscard]] Handle<T> allocate(Args&& ...args)
    {
        const Handle<T> handle = handleToNextFree();
        std::construct_at(&m_pool[handle.m_index], std::forward<Args>(args)...);
        return handle;
    }

    [[nodiscard]] Handle<T> allocate(const T& item)
    requires std::copyable<T>
    {
        const Handle<T> handle = handleToNextFree();
        m_pool[handle.m_index] = item;
        return handle;
    }

    [[nodiscard]] Handle<T> allocate(T&& item)
    requires std::movable<T>
    {
        const Handle<T> handle = handleToNextFree();
        m_pool[handle.m_index] = item;
        return handle;
    }

    void deallocate(const Handle<T>& handle)
    {
        const T* ptr = get(handle);
        if (ptr == nullptr) [[unlikely]] return;
        if constexpr (requires { T::freeResources(); }) ptr->freeResources();
        const uint32_t deallocIdx = handle.m_index;
        m_freeList.push(deallocIdx);
        ++m_generations[deallocIdx];
    }

    [[nodiscard]] T* get(const Handle<T>& handle)
    {
        const uint32_t idx = handle.m_index;
        if (handle.m_generation < m_generations[idx]) [[unlikely]] return nullptr;
        return &m_pool[handle.m_index];
    }

private:
    [[nodiscard]] Handle<T> handleToNextFree()
    {
        if (m_freeList.size() == 0) [[unlikely]] resize();
        const uint32_t nextFreeIdx = m_freeList.top();
        m_freeList.pop();
        return Handle<T>(nextFreeIdx, ++m_generations[nextFreeIdx]);
    }

    void resize()
    {
        const size_t size_prev = m_size;
        m_size = std::max(implicit_cast<size_t>(1), m_size) * DYNAMIC_STORAGE_GROWTH_FACTOR;

        m_pool.resize(m_size);
        m_generations.resize(m_size);
        m_freeList.resize(m_size);

        for (size_t idx : stdv::iota(size_prev, m_size) | stdv::reverse) {
            m_freeList.push(idx);
        }
    }

    size_t m_size;
    std::vector<T> m_pool;
    std::vector<uint32_t> m_generations;
    Stack<uint32_t> m_freeList;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
