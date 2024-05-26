export module atom.core:containers.dynamic_array_impl_vector;

import std;
import :core;
import :ranges;
import :math;

namespace atom
{
    template <typename in_elem_t, typename in_allocator_t>
    class _dynamic_array_impl_vector
    {
        using this_t = _dynamic_array_impl_vector;

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
        constexpr _dynamic_array_impl_vector()
            : _vector{}
        {}

        constexpr _dynamic_array_impl_vector(copy_tag, const _dynamic_array_impl_vector& that)
            : _vector{ that._vector }
        {}

        constexpr _dynamic_array_impl_vector(move_tag, _dynamic_array_impl_vector& that)
            : _vector{ std::move(that._vector) }
        {}

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr _dynamic_array_impl_vector(range_tag, other_iterator_t it, other_iterator_end_t it_end)
            : _vector{ move(it), move(it_end) }
        {}

        constexpr _dynamic_array_impl_vector(_with_count_type, usize count)
            : _vector(count)
        {}

        constexpr _dynamic_array_impl_vector(_with_count_type, usize count, const value_t& value)
            : _vector{ count, value }
        {}

        constexpr _dynamic_array_impl_vector(_with_capacity_type, usize capacity)
            : _vector{}
        {
            _vector.reserve(capacity);
        }

        constexpr ~_dynamic_array_impl_vector() {}

    public:
        constexpr auto move_this(this_t& that) -> void
        {
            _vector = std::move(that._vector);
        }

        constexpr auto get_at(usize index) const -> const value_t&
        {
            return _vector[index];
        }

        constexpr auto get_mut_at(usize index) -> value_t&
        {
            return _vector[index];
        }

        constexpr auto get_iterator() const -> iterator_t
        {
            return iterator_t(_vector.data());
        }

        constexpr auto get_iterator_at(usize index) const -> iterator_t
        {
            return iterator_t(_vector.data() + index);
        }

        constexpr auto get_iterator_end() const -> iterator_end_t
        {
            return iterator_end_t(_vector.data() + _vector.size());
        }

        constexpr auto get_mut_iterator() -> mut_iterator_t
        {
            return mut_iterator_t(_vector.data());
        }

        constexpr auto get_mut_iterator_at(usize index) -> mut_iterator_t
        {
            return mut_iterator_t(_vector.data() + index);
        }

        constexpr auto get_mut_iterator_end() -> mut_iterator_end_t
        {
            return mut_iterator_end_t(_vector.data() + _vector.size());
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto assign_range(other_iterator_t in_it, other_iterator_end_t in_it_end)
        {
            auto it = move(in_it);
            auto it_end = move(in_it_end);
            _vector.assign(it, it_end);
        }

        template <typename... arg_ts>
        constexpr auto emplace_at(usize index, arg_ts&&... args)
        {
            _vector.emplace(_vector.begin() + index, forward<arg_ts>(args)...);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto insert_range_at(
            usize index, other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            usize old_size = _vector.size();
            _vector.insert(_vector.begin() + index, move(it), move(it_end));

            return _vector.size() - old_size;
        }

        template <typename... arg_ts>
        constexpr auto emplace_front(arg_ts&&... args)
        {
            emplace_at(0);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto insert_range_front(other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            return insert_range_at(0, move(it), move(it_end));
        }

        template <typename... arg_ts>
        constexpr auto emplace_back(arg_ts&&... args)
        {
            _vector.emplace_back(forward<arg_ts>(args)...);
        }

        template <typename other_iterator_t, typename other_iterator_end_t>
        constexpr auto insert_range_back(other_iterator_t it, other_iterator_end_t it_end) -> usize
        {
            return insert_range_at(get_count(), move(it), move(it_end));
        }

        constexpr auto remove_at(usize index)
        {
            _vector.erase(_vector.begin() + index);
        }

        constexpr auto remove_front(usize index)
        {
            _vector.erase(_vector.begin());
        }

        constexpr auto remove_back(usize index)
        {
            if (_vector.size() > 0)
                _vector.erase(_vector.begin() + _vector.size() - 1);
        }

        constexpr auto remove_range(usize begin, usize count)
        {
            auto vector_begin = _vector.begin() + begin;
            _vector.erase(vector_begin, vector_begin + count);
        }

        constexpr auto remove_all()
        {
            _vector.clear();
        }

        constexpr auto reserve(usize count)
        {
            _vector.reserve(count);
        }

        constexpr auto reserve_more(usize count)
        {
            _vector.reserve(_vector.size() + count);
        }

        // todo: implement this.
        constexpr auto release_unused_mem() {}

        constexpr auto get_capacity() const -> usize
        {
            return _vector.capacity();
        }

        constexpr auto get_count() const -> usize
        {
            return _vector.size();
        }

        constexpr auto get_data() const -> const value_t*
        {
            return _vector.data();
        }

        constexpr auto get_mut_data() -> value_t*
        {
            return _vector.data();
        }

        constexpr auto get_allocator() const -> const allocator_t&
        {
            return _vector.alloc();
        }

        constexpr auto is_index_in_range(usize index) const -> bool
        {
            return index < _vector.size();
        }

        constexpr auto is_index_in_range_or_end(usize index) const -> bool
        {
            return index <= _vector.size();
        }

        constexpr auto is_iterator_valid(iterator_t it) const -> bool
        {
            return true;
        }

        constexpr auto get_index_for_iterator(iterator_t it) const -> usize
        {
            isize index = it.get_data() - _vector.data();
            return index < 0 ? math::max<usize>() : index;
        }

        constexpr auto is_iterator_in_range(iterator_t it) const -> bool
        {
            return get_index_for_iterator(it) < _vector.size();
        }

        constexpr auto is_iterator_in_range_or_end(iterator_t it) const -> bool
        {
            return get_index_for_iterator(it) <= _vector.size();
        }

    private:
        std::vector<value_t> _vector;
    };
}
