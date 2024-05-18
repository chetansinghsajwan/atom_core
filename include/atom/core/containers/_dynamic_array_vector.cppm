export module atom.core:containers.dynamic_array_impl_vector;

import std;
import :core;
import :ranges;
import :math;

namespace atom
{
    template <typename in_elem_t, typename in_allocator_t>
    class _dynamic_array_impl_using_std_vector
    {
        using this_t = _dynamic_array_impl_using_std_vector;

    public:
        using value_t = in_elem_t;
        using allocator_t = in_allocator_t;
        using iter_t = array_iter<value_t>;
        using iter_end_t = iter_t;
        using mut_iter_t = mut_array_iter<value_t>;
        using mut_iter_end_t = mut_iter_t;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class range_tag
        {};

    public:
        constexpr _dynamic_array_impl_using_std_vector()
            : _vector()
        {}

        constexpr _dynamic_array_impl_using_std_vector(
            copy_tag, const _dynamic_array_impl_using_std_vector& that)
            : _vector(that._vector)
        {}

        constexpr _dynamic_array_impl_using_std_vector(
            move_tag, _dynamic_array_impl_using_std_vector& that)
            : _vector(std::move(that._vector))
        {}

        template <typename other_iter_t, typename other_iter_end_t>
        constexpr _dynamic_array_impl_using_std_vector(
            range_tag, other_iter_t it, other_iter_end_t it_end)
            : _vector(
                  std_iter_wrap_for_atom_iter(move(it)), std_iter_wrap_for_atom_iter(move(it_end)))
        {}

        constexpr ~_dynamic_array_impl_using_std_vector() {}

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

        constexpr auto get_iter() const -> iter_t
        {
            return iter_t(_vector.data());
        }

        constexpr auto get_iter_at(usize index) const -> iter_t
        {
            return iter_t(_vector.data() + index);
        }

        constexpr auto get_iter_end() const -> iter_end_t
        {
            return iter_end_t(_vector.data() + _vector.size());
        }

        constexpr auto get_mut_iter() -> mut_iter_t
        {
            return mut_iter_t(_vector.data());
        }

        constexpr auto get_mut_iter_at(usize index) -> mut_iter_t
        {
            return mut_iter_t(_vector.data() + index);
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_t
        {
            return mut_iter_end_t(_vector.data() + _vector.size());
        }

        template <typename other_iter_t, typename other_iter_end_t>
        constexpr auto assign_range(other_iter_t in_it, other_iter_end_t in_it_end)
        {
            auto it = std_iter_wrap_for_atom_iter(move(in_it));
            auto it_end = std_iter_wrap_for_atom_iter(move(in_it_end));
            _vector.assign(it, it_end);
        }

        template <typename... arg_ts>
        constexpr auto emplace_at(usize index, arg_ts&&... args)
        {
            _vector.emplace(_vector.begin() + index, forward<arg_ts>(args)...);
        }

        template <typename other_iter_t, typename other_iter_end_t>
        constexpr auto insert_range_at(
            usize index, other_iter_t it, other_iter_end_t it_end) -> usize
        {
            usize old_size = _vector.size();
            _vector.insert(_vector.begin() + index, std_iter_wrap_for_atom_iter(move(it)),
                std_iter_wrap_for_atom_iter(move(it_end)));

            return _vector.size() - old_size;
        }

        template <typename... arg_ts>
        constexpr auto emplace_front(arg_ts&&... args)
        {
            emplace_at(0);
        }

        template <typename other_iter_t, typename other_iter_end_t>
        constexpr auto insert_range_front(other_iter_t it, other_iter_end_t it_end) -> usize
        {
            return insert_range_at(0, move(it), move(it_end));
        }

        template <typename... arg_ts>
        constexpr auto emplace_back(arg_ts&&... args)
        {
            _vector.emplace_back(forward<arg_ts>(args)...);
        }

        template <typename other_iter_t, typename other_iter_end_t>
        constexpr auto insert_range_back(other_iter_t it, other_iter_end_t it_end) -> usize
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

        constexpr auto is_iter_valid(iter_t it) const -> bool
        {
            return true;
        }

        constexpr auto get_index_for_iter(iter_t it) const -> usize
        {
            isize index = it.get_data() - _vector.data();
            return index < 0 ? math::max<usize>() : index;
        }

        constexpr auto is_iter_in_range(iter_t it) const -> bool
        {
            return get_index_for_iter(it) < _vector.size();
        }

        constexpr auto is_iter_in_range_or_end(iter_t it) const -> bool
        {
            return get_index_for_iter(it) <= _vector.size();
        }

    private:
        std::vector<value_t> _vector;
    };
}
