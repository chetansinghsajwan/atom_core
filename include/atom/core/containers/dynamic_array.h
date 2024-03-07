#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/core/int_wrapper.h"
#include "atom/core/tti.h"
#include "atom/core/range.h"
#include "atom/core/invokable/invokable.h"
#include "atom/core/memory/default_mem_allocator.h"
#include "atom/core/contracts.h"
#include "atom/core/range/array_iter.h"
#include <limits>

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_elem_type, typename in_allocator_type>
    class _dynamic_array_impl
    {
        using this_type = _dynamic_array_impl;

    public:
        using elem_type = in_elem_type;
        using allocator_type = in_allocator_type;
        using iter_type = array_iter<elem_type>;
        using iter_end_type = iter_type;
        using mut_iter_type = mut_array_iter<elem_type>;
        using mut_iter_end_type = mut_iter_type;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class range_tag
        {};

    public:
        constexpr _dynamic_array_impl()
            : _data(nullptr)
            , _count(0)
            , _capacity(0)
            , _allocator()
        {}

        constexpr _dynamic_array_impl(copy_tag, const _dynamic_array_impl& that)
            : this_type(range_tag(), that.get_iter(), that.get_iter_end())
        {}

        constexpr _dynamic_array_impl(move_tag, _dynamic_array_impl& that)
            : _data(that._data)
            , _count(that._count)
            , _capacity(that._capacity)
            , _allocator(that._allocator)
        {
            that._data = nullptr;
            that._count = 0;
            that._capacity = 0;
            that._allocator = allocator_type();
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr _dynamic_array_impl(range_tag, other_iter_type it, other_iter_end_type it_end)
            : _dynamic_array_impl()
        {
            insert_range_back(move(it), move(it_end));
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
        constexpr auto move_this(this_type& that) -> void
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

        constexpr auto get_at(usize index) const -> const elem_type&
        {
            return _data[index];
        }

        constexpr auto get_mut_at(usize index) -> elem_type&
        {
            return _data[index];
        }

        constexpr auto get_iter() const -> iter_type
        {
            return iter_type(_data);
        }

        constexpr auto get_iter_at(usize index) const -> iter_type
        {
            return iter_type(_data + index);
        }

        constexpr auto get_iter_end() const -> iter_end_type
        {
            return iter_end_type(_data + _count);
        }

        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return mut_iter_type(_data);
        }

        constexpr auto get_mut_iter_at(usize index) -> mut_iter_type
        {
            return mut_iter_type(_data + index);
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return mut_iter_end_type(_data + _count);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto assign_range(other_iter_type it, other_iter_end_type it_end)
        {
            remove_all();
            insert_range_back(move(it), move(it_end));
        }

        template <typename... arg_types>
        constexpr auto emplace_at(usize index, arg_types&&... args)
        {
            return _emplace_at(index, forward<arg_types>(args)...);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_at(usize index, other_iter_type it, other_iter_end_type it_end)
            -> usize
        {
            if constexpr (_can_get_range_size<other_iter_type, other_iter_end_type>())
            {
                return _insert_range_at_counted(index, it, _get_range_size(move(it), move(it_end)));
            }
            else
            {
                return _insert_range_at_uncounted(index, move(it), move(it_end));
            }
        }

        template <typename... arg_types>
        constexpr auto emplace_front(arg_types&&... args)
        {
            return _emplace_front(forward<arg_types>(args)...);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_front(other_iter_type it, other_iter_end_type it_end) -> usize
        {
            return insert_range_at(0, move(it), move(it_end));
        }

        template <typename... arg_types>
        constexpr auto emplace_back(arg_types&&... args)
        {
            return _emplace_at(_count, forward<arg_types>(args)...);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_back(other_iter_type it, other_iter_end_type it_end) -> usize
        {
            if constexpr (_can_get_range_size<other_iter_type, other_iter_end_type>())
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

        constexpr auto get_data() const -> const elem_type*
        {
            return _data;
        }

        constexpr auto get_mut_data() -> elem_type*
        {
            return _data;
        }

        constexpr auto get_allocator() const -> const allocator_type&
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

        constexpr auto is_iter_valid(iter_type it) const -> bool
        {
            return true;
        }

        constexpr auto get_index_for_iter(iter_type it) const -> usize
        {
            isize index = it.get_data() - _data;
            return index < 0 ? std::numeric_limits<usize>::max() : index;
        }

        constexpr auto is_iter_in_range(iter_type it) const -> bool
        {
            return get_index_for_iter(it) < _count;
        }

        constexpr auto is_iter_in_range_or_end(iter_type it) const -> bool
        {
            return get_index_for_iter(it) <= _count;
        }

    private:
        constexpr auto _release_all_mem()
        {
            _allocator.dealloc(_data);
        }

        template <typename... arg_types>
        constexpr auto _emplace_at(usize index, arg_types&&... args) -> usize
        {
            _ensure_space_at(index, 1);
            _construct_at(index, forward<arg_types>(args)...);
            _count += 1;

            return index;
        }

        template <typename other_iter_type>
        constexpr auto _insert_range_at_counted(usize index, other_iter_type it, usize count)
            -> usize
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
        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto _insert_range_at_uncounted(
            usize index, other_iter_type it, other_iter_end_type it_end) -> usize
        {
            usize rotate_size = _count - index;
            _insert_range_back_uncounted(move(it), move(it_end));
            _rotate_range_back(index, rotate_size);

            return index;
        }

        template <typename other_iter_type>
        constexpr auto _insert_range_back_counted(other_iter_type it, usize count)
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

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto _insert_range_back_uncounted(other_iter_type it, other_iter_end_type it_end)
            -> usize
        {
            usize count = 0;
            while (not it.is_eq(it_end))
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
        constexpr auto _update_iter_debug_id() {}

        constexpr auto _calc_cap_growth(usize required) const -> usize
        {
            const usize max = _count < std::numeric_limits<usize>::max() / 2
                                  ? _count
                                  : std::numeric_limits<usize>::max();
            return std::max(max, required);
        }

        constexpr auto _ensure_cap_for(usize count)
        {
            // we have enough capacity.
            if (_capacity - _count >= count)
                return;

            _update_iter_debug_id();

            usize new_cap = _calc_cap_growth(count);
            elem_type* new_data = (elem_type*)_allocator.alloc(new_cap);

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

        template <typename... arg_types>
        constexpr auto _construct_at(usize index, arg_types&&... args) -> void
        {
            elem_type* src = _data + index;
            std::construct_at(src, forward<arg_types>(args)...);
        }

        constexpr auto _destruct_at(usize index) -> void
        {
            elem_type* src = _data + index;
            std::destroy_at(src);
        }

        constexpr auto _destruct_range(usize index, usize count) -> void
        {
            elem_type* begin = _data + index;
            elem_type* end = begin + count;
            std::destroy(begin, end);
        }

        constexpr auto _destruct_all() -> void
        {
            elem_type* begin = _data;
            elem_type* end = _data + _count;
            std::destroy(begin, end);
        }

        constexpr auto _shift_range_front(usize index, usize steps) -> void
        {
            elem_type* begin = _data + index;
            elem_type* end = _data + _count;
            elem_type* dest = begin - steps;
            std::move(begin, end, dest);
        }

        constexpr auto _shift_range_back(usize index, usize steps) -> void
        {
            elem_type* begin = _data + index;
            elem_type* end = _data + _count;
            elem_type* dest = begin + steps;
            std::move_backward(begin, end, dest);
        }

        constexpr auto _rotate_range_back(usize index, usize count) -> void
        {
            elem_type* begin = _data;
            elem_type* mid = begin + index;
            elem_type* end = begin + _count;
            std::rotate(begin, mid, end);
        }

        constexpr auto _move_range_to(usize index, elem_type* dest) -> void
        {
            elem_type* begin = _data + index;
            elem_type* end = _data + _count;
            std::move(begin, end, dest);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        static constexpr auto _can_get_range_size() -> bool
        {
            return rfwd_iter_pair<other_iter_type, other_iter_end_type>;
        }

        template <typename other_iter_type, typename other_iter_end_type>
        static constexpr auto _get_range_size(other_iter_type it, other_iter_end_type it_end)
            -> usize
        {
            if constexpr (rjump_iter_pair<other_iter_type, other_iter_end_type>)
            {
                return it.compare(it_end).abs().template to<usize>();
            }

            usize count = 0;
            for (; it.is_eq(it_end); it.next())
                count++;

            return count;
        }

    private:
        elem_type* _data;
        usize _count;
        usize _capacity;
        allocator_type _allocator;
    };

    template <typename in_elem_type, typename in_allocator_type>
    class _dynamic_array_impl_using_std_vector
    {
        using this_type = _dynamic_array_impl_using_std_vector;

    public:
        using elem_type = in_elem_type;
        using allocator_type = in_allocator_type;
        using iter_type = array_iter<elem_type>;
        using iter_end_type = iter_type;
        using mut_iter_type = mut_array_iter<elem_type>;
        using mut_iter_end_type = mut_iter_type;

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

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr _dynamic_array_impl_using_std_vector(
            range_tag, other_iter_type it, other_iter_end_type it_end)
            : _vector(
                std_iter_wrap_for_atom_iter(move(it)), std_iter_wrap_for_atom_iter(move(it_end)))
        {}

        constexpr ~_dynamic_array_impl_using_std_vector() {}

    public:
        constexpr auto move_this(this_type& that) -> void
        {
            _vector = std::move(that._vector);
        }

        constexpr auto get_at(usize index) const -> const elem_type&
        {
            return _vector[index];
        }

        constexpr auto get_mut_at(usize index) -> elem_type&
        {
            return _vector[index];
        }

        constexpr auto get_iter() const -> iter_type
        {
            return iter_type(_vector.data());
        }

        constexpr auto get_iter_at(usize index) const -> iter_type
        {
            return iter_type(_vector.data() + index);
        }

        constexpr auto get_iter_end() const -> iter_end_type
        {
            return iter_end_type(_vector.data() + _vector.size());
        }

        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return mut_iter_type(_vector.data());
        }

        constexpr auto get_mut_iter_at(usize index) -> mut_iter_type
        {
            return mut_iter_type(_vector.data() + index);
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return mut_iter_end_type(_vector.data() + _vector.size());
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto assign_range(other_iter_type it, other_iter_end_type it_end)
        {
            _vector = range_from(move(it), move(it_end));
        }

        template <typename... arg_types>
        constexpr auto emplace_at(usize index, arg_types&&... args)
        {
            _vector.emplace(_vector.begin() + index, forward<arg_types>(args)...);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_at(usize index, other_iter_type it, other_iter_end_type it_end)
            -> usize
        {
            usize old_size = _vector.size();
            _vector.insert(_vector.begin() + index, std_iter_wrap_for_atom_iter(move(it)),
                std_iter_wrap_for_atom_iter(move(it_end)));

            return _vector.size() - old_size;
        }

        template <typename... arg_types>
        constexpr auto emplace_front(arg_types&&... args)
        {
            emplace_at(0);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_front(other_iter_type it, other_iter_end_type it_end) -> usize
        {
            return insert_range_at(0, move(it), move(it_end));
        }

        template <typename... arg_types>
        constexpr auto emplace_back(arg_types&&... args)
        {
            _vector.emplace_back(forward<arg_types>(args)...);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_back(other_iter_type it, other_iter_end_type it_end) -> usize
        {
            return insert_range_at(get_count(), move(it), move(it_end));
        }

        constexpr auto remove_at(usize index)
        {
            _vector.erase(_vector.begin() + index);
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

        constexpr auto get_data() const -> const elem_type*
        {
            return _vector.data();
        }

        constexpr auto get_mut_data() -> elem_type*
        {
            return _vector.data();
        }

        constexpr auto get_allocator() const -> const allocator_type&
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

        constexpr auto is_iter_valid(iter_type it) const -> bool
        {
            return true;
        }

        constexpr auto get_index_for_iter(iter_type it) const -> usize
        {
            isize index = it.get_data() - _vector.data();
            return index < 0 ? std::numeric_limits<usize>::max() : index;
        }

        constexpr auto is_iter_in_range(iter_type it) const -> bool
        {
            return get_index_for_iter(it) < _vector.size();
        }

        constexpr auto is_iter_in_range_or_end(iter_type it) const -> bool
        {
            return get_index_for_iter(it) <= _vector.size();
        }

    private:
        std::vector<elem_type> _vector;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # to do
    /// - write time complexities after writing implementation.
    /// - add note for case, where element or elements to be inserted are from this array.
    /// --------------------------------------------------------------------------------------------
    template <typename in_elem_type, typename in_allocator_type>
    class basic_dynamic_array
    {
        ATOM_STATIC_ASSERTS(
            not tti::is_ref<in_elem_type>, "dynamic_array does not supports ref types.");
        ATOM_STATIC_ASSERTS(not tti::is_void<in_elem_type>, "dynamic_array does not support void.");

    private:
        using _impl_type = _dynamic_array_impl_using_std_vector<in_elem_type, in_allocator_type>;

    public:
        using elem_type = in_elem_type;
        using allocator_type = in_allocator_type;
        using iter_type = array_iter<elem_type>;
        using iter_end_type = iter_type;
        using mut_iter_type = mut_array_iter<elem_type>;
        using mut_iter_end_type = mut_iter_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_dynamic_array()
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_dynamic_array(const basic_dynamic_array& that)
            : _impl(typename _impl_type::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const basic_dynamic_array& that) -> basic_dynamic_array&
        {
            _impl.assign_range(that.get_iter(), that.get_iter_end());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_dynamic_array(basic_dynamic_array&& that)
            : _impl(typename _impl_type::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(basic_dynamic_array&& that) -> basic_dynamic_array&
        {
            _impl.move_this(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr basic_dynamic_array(range_type&& range)
            requires(rrange_of<pure_type<range_type>, elem_type>)
            : _impl(typename _impl_type::range_tag(), range.get_iter(), range.get_iter_end())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr basic_dynamic_array& operator=(range_type&& range)
            requires(rrange_of<range_type, elem_type>)
        {
            _impl.assign_range(range.get_iter(), range.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// # destroyor
        /// ----------------------------------------------------------------------------------------
        constexpr ~basic_dynamic_array() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_at(usize index) const -> const elem_type&
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(index));

            return _impl.get_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_at(usize index) -> elem_type&
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(index));

            return _impl.get_mut_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs element at index `index` with `args`.
        ///
        /// # parameters
        ///
        /// - `index`: index to insert element at.
        /// - `args...`: args to construct element with.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(usize index, arg_types&&... args)
            requires(rconstructible<elem_type, arg_types...>)
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range_or_end(index), "index is out of range.");

            _impl.emplace_at(index, forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs element at pos `it` with `args`.
        ///
        /// # parameters
        ///
        /// - `it`: pos to insert element at.
        /// - `args...`: args to construct element with.
        ///
        /// # returns
        ///
        /// [`mut_iter_type`] to element inserted.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(iter_type it, arg_types&&... args) -> mut_iter_type
            requires(rconstructible<elem_type, arg_types...>)
        {
            ATOM_DEBUG_EXPECTS(is_iter_valid(it), "invalid iter.");
            ATOM_DEBUG_EXPECTS(is_iter_in_range_or_end(it), "iter is out of range.");

            usize index = get_index_for_iter(it);
            _impl.emplace_at(index, forward<arg_types>(args)...);
            return _impl.get_mut_iter_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at index `index`. forwards each value returned by `range.get_iter()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `index`: index to insert elements at.
        /// - `range`: range of elements to insert.
        ///
        /// # returns
        ///
        /// count of elements inserted.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(usize index, range_type&& range) -> usize
            requires(rrange_of<range_type, elem_type>)
                    and (rconstructible<elem_type, typename range_type::elem_type>)
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range_or_end(index), "index is out of range.");

            return _impl.insert_range_at(index, range.get_iter(), range.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at pos `it`. forwards each value returned by `range.get_iter()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `it`: pos to insert elements at.
        /// - `range`: range of elements to insert.
        ///
        /// # returns
        ///
        /// [`mut_array_range`] of elements inserted.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(iter_type it, range_type&& range)
            requires(rrange_of<range_type, elem_type>)
                    and (rconstructible<elem_type, typename range_type::elem_type>)
        {
            ATOM_DEBUG_EXPECTS(is_iter_valid(it), "invalid iter.");
            ATOM_DEBUG_EXPECTS(is_iter_in_range_or_end(it), "iter is out of range.");

            usize index = get_index_for_iter(it);
            usize count = _impl.insert_range_at(index, range.get_iter(), range.get_iter_end());
            return range_from(_impl.get_mut_iter_at(index), _impl.get_mut_iter_at(index + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs element at front with `args`.
        ///
        /// # parameters
        ///
        /// - `args...`: args to construct element with.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_front(arg_types&&... args)
            requires(rconstructible<elem_type, arg_types...>)
        {
            _impl.emplace_front(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at front. forwards each value returned by `range.get_iter()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `range`: range of elements to insert.
        ///
        /// # returns
        ///
        /// [`mut_iter_type`] to past the last inserted element.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_front(range_type&& range) -> mut_iter_type
            requires(rrange_of<range_type, elem_type>)
                    and (rconstructible<elem_type, typename range_type::elem_type>)
        {
            usize count = _impl.insert_range_front(forward<range_type&&>(range));
            return _impl.get_mut_iter(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs element at back with `args`.
        ///
        /// # parameters
        ///
        /// - `args...`: args to construct element with.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_back(arg_types&&... args)
            requires(rconstructible<elem_type, arg_types...>)
        {
            _impl.emplace_back(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename arg_type>
        constexpr auto operator+=(arg_type&& el)
            requires(rconstructible<elem_type, arg_type>)
        {
            _impl.emplace_back(forward<arg_type>(el));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at back. forwards each value returned by `range.get_iter()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `range`: range of elements to insert.
        ///
        /// # returns
        ///
        /// [`mut_iter_type`] to the first inserted element.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_back(range_type&& range) -> mut_iter_type
            requires(rrange_of<range_type, elem_type>)
                    and (rconstructible<elem_type, typename range_type::elem_type>)
        {
            usize count = _impl.insert_range_back(range.get_iter(), range.get_iter_end());
            return _impl.get_mut_iter_at(_impl.get_count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # to do:
        /// - what happens when the elem type accepts range as parameter?
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto operator+=(range_type&& range)
            requires(rrange_of<pure_type<range_type>, elem_type>)
                    and (rconstructible<elem_type, typename pure_type<range_type>::elem_type>)
        {
            _impl.insert_range_back(move(range.get_iter()), move(range.get_iter_end()));
        }

        /// ----------------------------------------------------------------------------------------
        /// removes element at index `index`.
        ///
        /// # parameters
        ///
        /// - `index`: index to remove element at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(usize index)
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(index), "index is out of range.");

            _impl.remove_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes element at pos `it`.
        ///
        /// # parameters
        ///
        /// - `it`: pos to remove element at.
        ///
        /// # returns
        ///
        /// [mut_iter_type] to next element. if `it` was pointing to the last element, returns
        /// [`get_iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(iter_type it) -> mut_iter_type
        {
            ATOM_DEBUG_EXPECTS(is_iter_valid(it), "invalid iter.");
            ATOM_DEBUG_EXPECTS(is_iter_in_range(it), "iter is out of range.");

            usize index = get_index_for_iter(it);
            _impl.remove_at(index);
            return _impl.get_mut_iter_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes elements in range [[`from`, `to`]].
        ///
        /// # parameters
        ///
        /// - `from`: start of range to remove elements at.
        /// - `to`: end of range to remove elements at.
        ///
        /// # returns
        ///
        /// `from`.
        ///
        /// ## explanation
        ///
        /// after removing `to - from` elements, next elements will be shifted back to index `from`.
        /// so the next element will be available at index `from`. if the last element of the arr
        /// was also removed, `from` will be equal to [`get_count()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(usize from, usize to) -> usize
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(to), "index was out of range.");
            ATOM_DEBUG_EXPECTS(from <= to, "index was out of range.");

            _impl.remove_range(from, to);
            return from;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes elements in range represented by `range`.
        ///
        /// # parameters
        ///
        /// - `it`: start of range to remove elements at.
        /// - `it_end`: end of range to remove elements at.
        ///
        /// # returns
        ///
        /// [`mut_iter_type`] to next element of the last removed element. if the last removed element
        /// was also the last element of the arr, returns [`get_iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(iter_type it, iter_type it_end) -> mut_iter_type
        {
            ATOM_DEBUG_EXPECTS(is_iter_valid(it), "invalid iter.");
            ATOM_DEBUG_EXPECTS(is_iter_valid(it_end), "invalid iter.");
            ATOM_DEBUG_EXPECTS(is_iter_in_range(it), "iter is out range.");
            ATOM_DEBUG_EXPECTS(is_iter_in_range(it_end), "iter is out range.");
            ATOM_DEBUG_EXPECTS(it.compare(it_end) <= 0, "invalid range.");

            usize from = get_index_for_iter(it);
            usize to = get_index_for_iter(it_end);
            _impl.remove_range(from, to);
            return _impl.get_mut_iter(from);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count_` elements from front.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_front(usize count_ = 1)
        {
            ATOM_DEBUG_EXPECTS(count_ <= get_count());

            _impl.remove_front(count_);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count_` elements from back.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_back(usize count_ = 1)
        {
            ATOM_DEBUG_EXPECTS(count_ <= get_count());

            _impl.remove_back(count_);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_one_find(const elem_type& elem) -> bool
        {
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (_impl.get_data()[i] == elem)
                {
                    _impl.remove_at(i);
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_all_find(const elem_type& elem) -> usize
        {
            usize removed_count = 0;
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (_impl.get_data()[i] == elem)
                {
                    _impl.remove_at(i);
                    i--;
                    removed_count++;
                }
            }

            return removed_count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename pred_type>
        constexpr auto remove_one_if(pred_type&& pred) -> bool
            requires(rinvokable<pred_type, bool(const elem_type&)>)
        {
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (pred(_impl.get_data()[i]))
                {
                    _impl.remove_at(i);
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename pred_type>
        constexpr auto remove_all_if(pred_type&& pred) -> usize
            requires(rinvokable<pred_type, bool(const elem_type&)>)
        {
            usize removed_count = 0;
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (pred(_impl.get_data()[i]))
                {
                    _impl.remove_at(i);
                    i--;
                    removed_count++;
                }
            }

            return removed_count;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes all elements.
        ///
        /// # note
        /// - does not free storage.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_all()
        {
            _impl.remove_all();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() const -> const elem_type*
        {
            return _impl.get_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> elem_type*
        {
            return _impl.get_mut_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _impl.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter() const -> iter_type
        {
            return _impl.get_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_type
        {
            return _impl.get_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return _impl.get_mut_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return _impl.get_mut_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for `count` elements. if there is already enough memory reserved,
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserve(usize count)
        {
            return _impl.reserve(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for `count` count more elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserve_more(usize count)
        {
            return _impl.reserve_more(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// releases unused memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto release_mem()
        {
            return _impl.release_unused_mem();
        }

        /// ----------------------------------------------------------------------------------------
        /// total count of places, with elements and reserved.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_capacity() const -> usize
        {
            return _impl.get_capacity();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of places reserved for elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserved_count() const -> usize
        {
            return _impl.get_capacity() - _impl.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_allocator() const -> const allocator_type&
        {
            return _impl.get_allocator();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `index` is in range [`0`, [`get_count()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_index_in_range(usize index) const
        {
            return _impl.is_index_in_range(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `index` is in range [`0`, [`get_count()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_index_in_range_or_end(usize index) const
        {
            return _impl.is_index_in_range_or_end(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// get index for iter `it`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_index_for_iter(iter_type it) const -> usize
        {
            return _impl.get_index_for_iter(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks for iter has been invalidated. this_type is done through a value which is changed
        /// for the container every time iters are invalidated.
        ///
        /// # to do
        /// - implement iter validation.
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_valid(iter_type it) const
        {
            return _impl.is_iter_valid(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iter `pos` is in range [[`get_iter()`], [`get_iter_end()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_in_range(iter_type it) const
        {
            return _impl.is_iter_in_range(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iter `pos` is in range [[`get_iter()`], [`get_iter_end()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_in_range_or_end(iter_type it) const
        {
            return _impl.is_iter_in_range_or_end(it);
        }

    private:
        _impl_type _impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type, typename allocator_type = default_mem_allocator>
    class dynamic_array
        : public mut_array_range_extensions<basic_dynamic_array<elem_type, allocator_type>>
    {
        using base_type =
            mut_array_range_extensions<basic_dynamic_array<elem_type, allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
