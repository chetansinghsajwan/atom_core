export module atom.core:containers.dynamic_array_impl;

import std;
import :core;
import :ranges;
import :contracts;
import :math;

namespace atom
{
    template <typename in_elem_t, typename in_allocator_t>
    class _dynamic_array_impl
    {
        using this_t = _dynamic_array_impl;

    public:
        using value_t = in_elem_t;
        using allocator_t = in_allocator_t;
        using iterator_t = array_iterator<value_t>;
        using iterator_end_t = iterator_t;
        using mut_iterator_t = mut_array_iterator<value_t>;
        using mut_iterator_end_t = mut_iterator_t;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class range_tag
        {};

    public:
        constexpr _dynamic_array_impl()
            : _data{ nullptr }
            , _count{ 0 }
            , _capacity{ 0 }
            , _allocator{}
        {}

        constexpr _dynamic_array_impl(copy_tag, const _dynamic_array_impl& that)
            : this_t{ range_tag(), that.get_iterator(), that.get_iterator_end() }
        {}

        constexpr _dynamic_array_impl(move_tag, _dynamic_array_impl& that)
            : _data{ that._data }
            , _count{ that._count }
            , _capacity{ that._capacity }
            , _allocator{ that._allocator }
        {
            that._data = nullptr;
            that._count = 0;
            that._capacity = 0;
            that._allocator = allocator_t();
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr _dynamic_array_impl(range_tag, other_iterator_t it, other_iterator_end_t it_end)
            : _dynamic_array_impl{}
        {
            insert_range_back(move(it), move(it_end));
        }

        constexpr _dynamic_array_impl(_with_count_type, usize count)
            : _count{ count }
            , _capacity{ count }
            , _allocator{}
        {
            _data = (value_t*)_allocator.alloc(count);
        }

        constexpr _dynamic_array_impl(_with_count_type, usize count, const value_t& value)
            : _count{ count }
            , _capacity{ count }
            , _allocator{}
        {
            _data = (value_t*)_allocator.alloc(count);

            for (usize i = 0; i < _count; i++)
                _data[i] = value;
        }

        constexpr _dynamic_array_impl(_with_capacity_type, usize capacity)
            : _count{ 0 }
            , _capacity{ capacity }
            , _allocator{}
        {
            _data = (value_t*)_allocator.alloc(capacity);
        }

        constexpr ~_dynamic_array_impl()
        {
            if (_count != 0)
            {
                _destruct_all();
                _release_all_mem();
            }
        }

    public:
        constexpr auto move_this(this_t& that) -> void
        {
            if (_count != 0)
            {
                _destruct_all();
                _release_all_mem();
            }

            _data = that._data;
            _count = that._count;
            _capacity = that._capacity;
            _allocator = that._allocator;
        }

        constexpr auto get_at(usize index) const -> const value_t&
        {
            return _data[index];
        }

        constexpr auto get_mut_at(usize index) -> value_t&
        {
            return _data[index];
        }

        constexpr auto get_iterator() const -> iterator_t
        {
            return iterator_t(_data);
        }

        constexpr auto get_iterator_at(usize index) const -> iterator_t
        {
            return iterator_t(_data + index);
        }

        constexpr auto get_iterator_end() const -> iterator_end_t
        {
            return iterator_end_t(_data + _count);
        }

        constexpr auto get_mut_iterator() -> mut_iterator_t
        {
            return mut_iterator_t(_data);
        }

        constexpr auto get_mut_iterator_at(usize index) -> mut_iterator_t
        {
            return mut_iterator_t(_data + index);
        }

        constexpr auto get_mut_iterator_end() -> mut_iterator_end_t
        {
            return mut_iterator_end_t(_data + _count);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto assign_range(other_iterator_t it, other_iterator_end_t it_end)
        {
            remove_all();
            insert_range_back(move(it), move(it_end));
        }

        template <typename... arg_ts>
        constexpr auto emplace_at(usize index, arg_ts&&... args)
        {
            return _emplace_at(index, forward<arg_ts>(args)...);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto insert_range_at(
            usize index, other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            if constexpr (_can_get_range_size<other_iterator_t, other_iterator_end_t>())
            {
                return _insert_range_at_counted(index, it, _get_range_size(move(it), move(it_end)));
            }
            else
            {
                return _insert_range_at_uncounted(index, move(it), move(it_end));
            }
        }

        template <typename... arg_ts>
        constexpr auto emplace_front(arg_ts&&... args)
        {
            return _emplace_front(forward<arg_ts>(args)...);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto insert_range_front(other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            return insert_range_at(0, move(it), move(it_end));
        }

        template <typename... arg_ts>
        constexpr auto emplace_back(arg_ts&&... args)
        {
            return _emplace_at(_count, forward<arg_ts>(args)...);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto insert_range_back(other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            if constexpr (_can_get_range_size<other_iterator_t, other_iterator_end_t>())
            {
                usize count = _get_range_size(move(it), move(it_end));
                _insert_range_back_counted(it, count);
                return count;
            }
            else
            {
                return _insert_range_back_uncounted(move(it), move(it_end));
            }
        }

        constexpr auto remove_at(usize index)
        {
            _destruct_at(index);
            _shift_range_front(index, 1);
        }

        constexpr auto remove_range(usize begin, usize count)
        {
            _destruct_range(begin, count);
            _shift_range_front(begin + count, count);
        }

        constexpr auto remove_all()
        {
            if (_count != 0)
            {
                _destruct_all();
                _count = 0;
            }
        }

        constexpr auto reserve(usize count)
        {
            _ensure_cap_for(count);
        }

        constexpr auto reserve_more(usize count)
        {
            _ensure_cap_for(_count + count);
        }

        // todo: implement this.
        constexpr auto release_unused_mem() {}

        constexpr auto get_capacity() const -> usize
        {
            return _capacity;
        }

        constexpr auto get_count() const -> usize
        {
            return _count;
        }

        constexpr auto get_data() const -> const value_t*
        {
            return _data;
        }

        constexpr auto get_mut_data() -> value_t*
        {
            return _data;
        }

        constexpr auto get_allocator() const -> const allocator_t&
        {
            return _allocator;
        }

        constexpr auto is_index_in_range(usize index) const -> bool
        {
            return index < _count;
        }

        constexpr auto is_index_in_range_or_end(usize index) const -> bool
        {
            return index <= _count;
        }

        constexpr auto is_iterator_valid(iterator_t it) const -> bool
        {
            return true;
        }

        constexpr auto get_index_for_iterator(iterator_t it) const -> usize
        {
            isize index = it.get_data() - _data;
            return index < 0 ? math::max<usize>() : index;
        }

        constexpr auto is_iterator_in_range(iterator_t it) const -> bool
        {
            return get_index_for_iterator(it) < _count;
        }

        constexpr auto is_iterator_in_range_or_end(iterator_t it) const -> bool
        {
            return get_index_for_iterator(it) <= _count;
        }

    private:
        constexpr auto _release_all_mem()
        {
            _allocator.dealloc(_data);
        }

        template <typename... arg_ts>
        constexpr auto _emplace_at(usize index, arg_ts&&... args) -> usize
        {
            _ensure_space_at(index, 1);
            _construct_at(index, forward<arg_ts>(args)...);
            _count += 1;

            return index;
        }

        template <typename other_iterator_t>
        constexpr auto _insert_range_at_counted(usize index, other_iterator_t it, usize count) -> usize
        {
            if (count == 0)
                return index;

            _ensure_space_at(count);

            for (usize i = 0; i < count; i++)
            {
                _construct_at(index + i, it.value());
                it.next();
            }

            _count += count;
            return index;
        }

        // todo: improve performance.
        constexpr auto _ensure_space_front(usize count) -> void
        {
            _ensure_space_at(0, count);
        }

        // todo: improve performance.
        constexpr auto _ensure_space_at(usize index, usize count) -> void
        {
            _ensure_cap_for(count);

            if (_count != 0)
                _shift_range_back(index, count);
        }

        // todo: improve performance.
        constexpr auto _ensure_space_back(usize count) -> void
        {
            _ensure_cap_for(count);
        }

        // unchecked
        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto _insert_range_at_uncounted(
            usize index, other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            usize rotate_size = _count - index;
            _insert_range_back_uncounted(move(it), move(it_end));
            _rotate_range_back(index, rotate_size);

            return index;
        }

        template <typename other_iterator_t>
        constexpr auto _insert_range_back_counted(other_iterator_t it, usize count)
        {
            if (count == 0)
                return;

            _ensure_space_back(count);

            for (usize i = 0; i < count; i++)
            {
                _construct_at(_count + i, it.value());
                it.next();
            }

            _count += count;
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto _insert_range_back_uncounted(
            other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            usize count = 0;
            while (it != it_end)
            {
                _ensure_space_back(1);
                _construct_at(_count + count, it.value());

                it.next();
                count++;
            }

            _count += count;
            return count;
        }

        // todo: implement this.
        constexpr auto _update_iterator_debug_id() {}

        constexpr auto _calc_cap_growth(usize required) const -> usize
        {
            const usize max = _count < math::max<usize>() / 2 ? _count : math::max<usize>();
            return std::max(max, required);
        }

        constexpr auto _ensure_cap_for(usize count)
        {
            // we have enough capacity.
            if (_capacity - _count >= count)
                return;

            _update_iterator_debug_id();

            usize new_cap = _calc_cap_growth(count);
            value_t* new_data = (value_t*)_allocator.alloc(new_cap);

            if (_count != 0)
            {
                _move_range_to(0, new_data);
            }

            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
            }

            _data = new_data;
            _capacity = new_cap;
        }

        template <typename... arg_ts>
        constexpr auto _construct_at(usize index, arg_ts&&... args) -> void
        {
            value_t* src = _data + index;
            std::construct_at(src, forward<arg_ts>(args)...);
        }

        constexpr auto _destruct_at(usize index) -> void
        {
            value_t* src = _data + index;
            std::destroy_at(src);
        }

        constexpr auto _destruct_range(usize index, usize count) -> void
        {
            value_t* begin = _data + index;
            value_t* end = begin + count;
            std::destroy(begin, end);
        }

        constexpr auto _destruct_all() -> void
        {
            value_t* begin = _data;
            value_t* end = _data + _count;
            std::destroy(begin, end);
        }

        constexpr auto _shift_range_front(usize index, usize steps) -> void
        {
            value_t* begin = _data + index;
            value_t* end = _data + _count;
            value_t* dest = begin - steps;
            std::move(begin, end, dest);
        }

        constexpr auto _shift_range_back(usize index, usize steps) -> void
        {
            value_t* begin = _data + index;
            value_t* end = _data + _count;
            value_t* dest = begin + steps;
            std::move_backward(begin, end, dest);
        }

        constexpr auto _rotate_range_back(usize index, usize count) -> void
        {
            value_t* begin = _data;
            value_t* mid = begin + index;
            value_t* end = begin + _count;
            std::rotate(begin, mid, end);
        }

        constexpr auto _move_range_to(usize index, value_t* dest) -> void
        {
            value_t* begin = _data + index;
            value_t* end = _data + _count;
            std::move(begin, end, dest);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        static constexpr auto _can_get_range_size() -> bool
        {
            return is_unidirectional_iterator_pair<other_iterator_t, other_iterator_end_t>;
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        static constexpr auto _get_range_size(other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            if constexpr (is_random_access_iterator_pair<other_iterator_t, other_iterator_end_t>)
            {
                return it.compare(it_end).abs().template to<usize>();
            }

            usize count = 0;
            for (; it != it_end; it++)
                count++;

            return count;
        }

    private:
        value_t* _data;
        usize _count;
        usize _capacity;
        allocator_t _allocator;
    };
}
