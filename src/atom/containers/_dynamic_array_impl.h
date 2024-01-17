#pragma once
#include "atom/contracts.h"
#include "atom/core.h"
#include "atom/range/array_iter.h"

// #include <algorithm>

namespace atom
{
    template <typename in_elem_type, typename in_allocator_type>
    class _dynamic_array_impl
    {
    public:
        using elem_type = in_elem_type;
        using allocator_type = in_allocator_type;
        using iter_type = array_iter<elem_type>;
        using iter_end_type = iter_type;
        using mut_iter_type = mut_array_iter<elem_type>;
        using mut_iter_end_type = mut_iter_type;

    public:
        constexpr _dynamic_array_impl()
            : _arr{ nullptr }
            , _count{ 0 }
            , _capacity{ 0 }
            , _alloc{}
        {}

        constexpr _dynamic_array_impl(_dynamic_array_impl&& that) {}

        template <typename uiter, typename uiter_end>
        constexpr _dynamic_array_impl(uiter it, uiter_end it_end)
            : _dynamic_array_impl{}
        {
            insert_range_back(it, it_end);
        }

    public:
        constexpr auto on_destruct()
        {
            remove_all();
            release_unused_mem();
        }

        constexpr auto at(usize i) const -> const elem_type&
        {
            return _get_data()[i];
        }

        constexpr auto mut_at(usize i) const -> const elem_type&
        {
            return _get_mut_data()[i];
        }

        constexpr auto iter(usize i = 0) const -> iter_type
        {
            contracts::debug_expects(is_index_in_range(i));

            return iter_type{ _get_data() + i };
        }

        constexpr auto iter_end() const -> iter_end_type
        {
            return iter_end_type{ _get_data() + _get_count() };
        }

        constexpr auto mut_iter(usize i = 0) -> mut_iter_type
        {
            contracts::debug_expects(is_index_in_range(i));

            return mut_iter_type{ _get_mut_data() + i };
        }

        constexpr auto mut_iter_end() -> mut_iter_end_type
        {
            return mut_iter_end_type{ _get_mut_data() + _get_count() };
        }

        template <typename uiter, typename uiter_end>
        constexpr auto assign_range(uiter it, uiter_end it_end)
        {
            remove_all();
            insert_range_back(it, it_end);
        }

        template <typename... args_type>
        constexpr auto emplace_at(usize i, args_type&&... args)
        {
            return _emplace_at(i, forward<args_type>(args)...);
        }

        template <typename uiter, typename uiter_end>
        constexpr auto insert_range_at(usize i, uiter it, uiter_end it_end) -> usize
        {
            if constexpr (_can_get_range_size<uiter, uiter_end>())
            {
                return _insert_range_at_counted(i, it, _get_range_size(it, it_end));
            }
            else
            {
                return _insert_range_at_uncounted(i, mov(it), mov(it_end));
            }
        }

        template <typename... args_type>
        constexpr auto emplace_front(args_type&&... args)
        {
            return _emplace_front(forward<args_type>(args)...);
        }

        template <typename uiter, typename uiter_end>
        constexpr auto insert_range_front(uiter it, uiter_end it_end) -> usize
        {
            return insert_range_at(0, it, it_end);
        }

        template <typename... args_type>
        constexpr auto emplace_back(args_type&&... args)
        {
            return _emplace_at(_get_count() - 1, forward<args_type>(args)...);
        }

        template <typename uiter, typename uiter_end>
        constexpr auto insert_range_back(uiter it, uiter_end it_end) -> usize
        {
            if constexpr (_can_get_range_size<uiter, uiter_end>())
            {
                usize count = _get_range_size(it, it_end);
                _insert_range_back_counted(it, count);
                return count;
            }
            else
            {
                return _insert_range_back_uncounted(mov(it), mov(it_end));
            }
        }

        constexpr auto remove_at(usize i)
        {
            _destruct_at(i);
            _move_range_front(i, 1);
        }

        constexpr auto remove_range(usize begin, usize count)
        {
            _destruct_range(begin, count);
            _move_range_front(begin, count);
        }

        constexpr auto remove_all()
        {
            if (_get_count() > 0)
            {
                _destruct_range(0, _get_count().sub(1));
                _move_range_front(0, _get_count().sub(1));
            }
        }

        constexpr auto reserve(usize count)
        {
            return reserve_more(count);
        }

        constexpr auto reserve_more(usize count)
        {
            _ensure_cap_for(_get_count() + count);
        }

        constexpr auto release_unused_mem() {}

        constexpr auto capacity() const -> usize
        {
            return _get_capacity();
        }

        constexpr auto count() const -> usize
        {
            return _get_count();
        }

        constexpr auto data() const -> mem_ptr<elem_type>
        {
            return _get_data();
        }

        constexpr auto mut_data() -> mut_mem_ptr<elem_type>
        {
            return _get_mut_data();
        }

        constexpr auto alloc() const -> const allocator_type&
        {
            return _alloc;
        }

        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return i < _get_count();
        }

        constexpr auto is_index_in_range_or_end(usize i) const -> bool
        {
            return i <= _get_count();
        }

        constexpr auto is_iter_valid(iter_type it) const -> bool
        {
            return true;
        }

        constexpr auto index_for_iter(iter_type it) const -> usize
        {
            isize i = it.data() - _get_data();
            return i < 0 ? usize::max() : usize(i);
        }

        constexpr auto is_iter_in_range(iter_type it) const -> bool
        {
            return index_for_iter(it) < _get_count();
        }

        constexpr auto is_iter_in_range_or_end(iter_type it) const -> bool
        {
            return index_for_iter(it) <= _get_count();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// implementations
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    private:
        template <typename... args_type>
        constexpr auto _emplace_at(usize i, args_type&&... args) -> usize
        {
            _ensure_cap_for(i);
            _move_range_back(i, 1);
            _construct_at(i, forward<args_type>(args)...);

            return i;
        }

        template <typename uiter>
        constexpr auto _insert_range_at_counted(usize i, uiter it, usize count) -> usize
        {
            if (count == 0)
                return i;

            _ensure_cap_for(count);
            _move_range_back(i, count);

            // insert new elements
            for (usize i = 0; i < count; i++)
            {
                _construct_at(i + i, forward<decltype(it.value())>(it.value()));
                it.next();
            }

            return i;
        }

        template <typename uiter, typename uiter_end>
        constexpr auto _insert_range_at_uncounted(usize i, uiter it, uiter_end it_end) -> usize
        {
            usize rotate_size = _get_count() - i;
            _insert_range_back_uncounted(mov(it), mov(it_end));
            _rotate_range_back(i, rotate_size);

            return i;
        }

        template <typename u>
        constexpr auto _insert_back(u&& el)
        {
            _ensure_cap_for(1);
            _construct_at(_get_count(), forward<u>(el));
            _set_count(_get_count() + 1);
        }

        template <typename uiter>
        constexpr auto _insert_range_back_counted(uiter it, usize count)
        {
            if (count == 0)
                return;

            _ensure_cap_for(count);

            usize i = _get_count();
            for (usize j = 0; j < count; j++)
            {
                _construct_at(i + j, forward<decltype(it.value())>(it.value()));
                it.next();
            }

            _set_count(i + count);
        }

        template <typename uiter, typename uiter_end>
        constexpr auto _insert_range_back_uncounted(uiter it, uiter_end it_end) -> usize
        {
            usize i = _get_count();
            usize count = 0;
            while (not it.eq(it_end))
            {
                _ensure_cap_for(1);
                _construct_at(i + count, it.value());

                it.next();
                count++;
            }

            _set_count(i + count);
            return count;
        }

        constexpr auto _update_iter_debug_id() {}

        constexpr auto _calc_cap_growth(usize required) const -> usize
        {
            return required;
        }

        constexpr auto _ensure_cap_for(usize count)
        {
            _update_iter_debug_id();

            // we have enough capacity.
            if (_get_capacity() - _get_count() >= count)
                return;

            usize new_cap = _calc_cap_growth(count);
            mut_mem_ptr<elem_type> new_array = _alloc_mem(new_cap);

            _move_range_to(0, new_array);
            _dealloc_mem(_get_mut_data());
            _set_data(new_array);
            _set_capacity(new_cap);
        }

        template <typename... args_type>
        constexpr auto _construct_at(usize i, args_type&&... args) -> void
        {
            mut_mem_ptr<elem_type> src = _get_mut_data() + i;
            std::construct_at(src.unwrap(), forward<args_type>(args)...);
        }

        constexpr auto _destruct_at(usize i) -> void
        {
            mut_mem_ptr<elem_type> src = _get_mut_data() + i;
            std::destroy_at(src.unwrap());
        }

        constexpr auto _destruct_range(usize i, usize count) -> void
        {
            mut_mem_ptr<elem_type> begin = _get_mut_data() + i;
            mut_mem_ptr<elem_type> end = begin + count;
            std::destroy(begin.unwrap(), end.unwrap());
        }

        constexpr auto _move_range_front(usize i, usize count) -> void
        {
            mut_mem_ptr<elem_type> begin = _get_mut_data() + i;
            mut_mem_ptr<elem_type> end = _get_mut_data() + _get_count() - 1;
            mut_mem_ptr<elem_type> dest = begin - count;
            std::move(begin.unwrap(), end.unwrap(), dest.unwrap());
        }

        constexpr auto _move_range_back(usize i, usize count) -> void
        {
            mut_mem_ptr<elem_type> begin = _get_mut_data() + i;
            mut_mem_ptr<elem_type> end = _get_mut_data() + _get_count() - 1;
            mut_mem_ptr<elem_type> dest = begin + count;
            std::move_backward(begin.unwrap(), end.unwrap(), dest.unwrap());
        }

        constexpr auto _move_range_to(usize i, mut_mem_ptr<elem_type> dest) -> void
        {
            mut_mem_ptr<elem_type> begin = _get_mut_data() + i;
            mut_mem_ptr<elem_type> end = _get_mut_data() + _get_count() - 1;
            std::move_backward(begin.unwrap(), end.unwrap(), dest.unwrap());
        }

        constexpr auto _rotate_range_back(usize i, usize count) -> void
        {
            mut_mem_ptr<elem_type> begin = _get_mut_data();
            mut_mem_ptr<elem_type> mid = begin + i;
            mut_mem_ptr<elem_type> end = begin + _get_count() - 1;
            std::rotate(begin.unwrap(), mid.unwrap(), end.unwrap());
        }

        template <typename uiter, typename uiter_end>
        static constexpr auto _can_get_range_size() -> bool
        {
            return rfwd_iter_pair<uiter, uiter_end>;
        }

        template <typename uiter, typename uiter_end>
        static constexpr auto _get_range_size(uiter it, uiter_end it_end) -> usize
        {
            if constexpr (rjump_iter_pair<uiter, uiter_end>)
            {
                return it.compare(it_end).abs().template to<usize>();
            }

            usize count = 0;
            for (; it.eq(it_end); it.next())
                count++;

            return count;
        }

        constexpr auto _get_data() const -> mem_ptr<elem_type>
        {
            return _arr;
        }

        constexpr auto _get_mut_data() -> mut_mem_ptr<elem_type>
        {
            return _arr;
        }

        constexpr auto _set_data(mut_mem_ptr<elem_type> data)
        {
            _arr = data;
        }

        constexpr auto _get_count() const -> usize
        {
            return _count;
        }

        constexpr auto _set_count(usize count)
        {
            _count = count;
        }

        constexpr auto _get_capacity() const -> usize
        {
            return _capacity;
        }

        constexpr auto _set_capacity(usize capacity)
        {
            _capacity = capacity;
        }

        constexpr auto _alloc_mem(usize required) -> mut_mem_ptr<elem_type>
        {
            return _alloc.alloc(required);
        }

        constexpr auto _alloc_mem_at_least(usize required, usize hint) -> mut_mem_ptr<elem_type>
        {
            return _alloc.alloc(required);
        }

        constexpr auto _dealloc_mem(mut_mem_ptr<elem_type> mem)
        {
            _alloc.dealloc(mem);
        }

    private:
        mut_mem_ptr<elem_type> _arr;
        usize _count;
        usize _capacity;
        allocator_type _alloc;
    };
}
