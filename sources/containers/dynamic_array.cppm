export module atom_core:containers.dynamic_array;

import :core;
import :ranges;
import :types;
import :contracts;
import :default_mem_allocator;
import :containers.dynamic_array_impl_vector;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # to do
    /// - write time complexities after writing implementation.
    /// - add note for case, where value or values to be inserted are from this array.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type, typename in_allocator_type = default_mem_allocator>
    class dynamic_array
    {
        static_assert(
            type_info<in_value_type>::is_pure(), "dynamic_array does not non pure types.");
        static_assert(
            not type_info<in_value_type>::is_void(), "dynamic_array does not support void.");

    private:
        using this_type = dynamic_array<in_value_type, in_allocator_type>;
        using _impl_type = dynamic_array_impl_vector<in_value_type, in_allocator_type>;

    public:
        using value_type = in_value_type;
        using allocator_type = in_allocator_type;
        using const_iterator_type = array_iterator<value_type>;
        using const_iterator_end_type = const_iterator_type;
        using iterator_type = mut_array_iterator<value_type>;
        using iterator_end_type = iterator_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array()
            : _impl{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(const dynamic_array& that)
            : _impl{ typename _impl_type::copy_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const dynamic_array& that) -> dynamic_array&
        {
            _impl.assign_range(that.get_iterator(), that.get_iterator_end());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(dynamic_array&& that)
            : _impl{ typename _impl_type::move_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(dynamic_array&& that) -> dynamic_array&
        {
            _impl.move_this(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr dynamic_array(range_type&& range)
            requires ranges::const_range_concept<
                typename type_info<range_type>::pure_type::value_type, value_type>
            : _impl{ typename _impl_type::range_tag{}, range.get_iterator(),
                range.get_iterator_end() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr dynamic_array& operator=(range_type&& range)
            requires ranges::const_range_concept<range_type, value_type>
        {
            _impl.assign_range(range.get_iterator(), range.get_iterator_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr dynamic_array(const value_type (&array)[count])
            : _impl{ typename _impl_type::range_tag{}, const_iterator_type(array),
                const_iterator_type(array + count) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr dynamic_array& operator=(const value_type (&array)[count])
        {
            _impl.assign_range(const_iterator_type(array), const_iterator_type(array + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_with_count_type, usize count)
            : _impl{ create_with_count, count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_with_count_type, usize count, const value_type& value)
            : _impl{ create_with_count, count, value }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_with_capacity_type, usize capacity)
            : _impl{ create_with_capacity, capacity }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_from_raw_tag, const value_type* arr, usize count)
            : _impl{ create_from_raw, arr, count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # destroyor
        /// ----------------------------------------------------------------------------------------
        constexpr ~dynamic_array() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_at(usize index) const -> const value_type&
        {
            contract_debug_expects(is_index_in_range(index));

            return _impl.get_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_at(usize index) -> value_type&
        {
            contract_debug_expects(is_index_in_range(index));

            return _impl.get_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at index `index` with `args`.
        ///
        /// # parameters
        ///
        /// - `index`: index to insert value at.
        /// - `args...`: args to construct value with.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(usize index, arg_types&&... args)
            requires(type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            contract_debug_expects(is_index_in_range_or_end(index), "index is out of range.");

            _impl.emplace_at(index, forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at pos `it` with `args`.
        ///
        /// # parameters
        ///
        /// - `it`: pos to insert value at.
        /// - `args...`: args to construct value with.
        ///
        /// # returns
        ///
        /// [`iterator_type`] to value inserted.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(const_iterator_type it, arg_types&&... args) -> iterator_type
            requires(type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            _impl.emplace_at(index, forward<arg_types>(args)...);
            return _impl.get_iterator_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at index `index`. forwards each value returned by `range.get_iterator()` to
        /// constructor of value in the arr.
        ///
        /// # parameters
        ///
        /// - `index`: index to insert values at.
        /// - `range`: range of values to insert.
        ///
        /// # returns
        ///
        /// count of values inserted.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(usize index, range_type&& range) -> usize
            requires ranges::const_range_concept<range_type, value_type>
                     and (type_info<value_type>::template is_constructible_from<
                         typename range_type::value_type>())
        {
            contract_debug_expects(is_index_in_range_or_end(index), "index is out of range.");

            return _impl.insert_range_at(index, range.get_iterator(), range.get_iterator_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at pos `it`. forwards each value returned by `range.get_iterator()` to
        /// constructor of value in the arr.
        ///
        /// # parameters
        ///
        /// - `it`: pos to insert values at.
        /// - `range`: range of values to insert.
        ///
        /// # returns
        ///
        /// [`mut_array_range`] of values inserted.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(const_iterator_type it, range_type&& range)
            requires ranges::const_range_concept<range_type, value_type>
                     and (type_info<value_type>::template is_constructible_from<
                         typename range_type::value_type>())
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            usize count =
                _impl.insert_range_at(index, range.get_iterator(), range.get_iterator_end());
            return ranges::from(_impl.get_iterator_at(index), _impl.get_iterator_at(index + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at first with `args`.
        ///
        /// # parameters
        ///
        /// - `args...`: args to construct value with.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_first(arg_types&&... args)
            requires(type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_first(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at first. forwards each value returned by `range.get_iterator()` to
        /// constructor of value in the arr.
        ///
        /// # parameters
        ///
        /// - `range`: range of values to insert.
        ///
        /// # returns
        ///
        /// [`iterator_type`] to past the last inserted value.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_first(range_type&& range) -> iterator_type
            requires ranges::const_range_concept<range_type, value_type>
                     and (type_info<value_type>::template is_constructible_from<
                         typename range_type::value_type>())
        {
            usize count = _impl.insert_range_first(forward<range_type&&>(range));
            return _impl.get_iterator(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at last with `args`.
        ///
        /// # parameters
        ///
        /// - `args...`: args to construct value with.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_last(arg_types&&... args)
            requires(type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_last(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename arg_type>
        constexpr auto operator+=(arg_type&& el)
            requires(type_info<value_type>::template is_constructible_from<arg_type>())
        {
            _impl.emplace_last(forward<arg_type>(el));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at last. forwards each value returned by `range.get_iterator()` to
        /// constructor of value in the arr.
        ///
        /// # parameters
        ///
        /// - `range`: range of values to insert.
        ///
        /// # returns
        ///
        /// [`iterator_type`] to the first inserted value.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_last(range_type&& range) -> iterator_type
            requires ranges::const_range_concept<
                         typename type_info<range_type>::pure_type::value_type, value_type>
                     and (type_info<value_type>::template is_constructible_from<
                         typename type_info<range_type>::pure_type::value_type::value_type>())
        {
            usize count = _impl.insert_range_last(range.get_iterator(), range.get_iterator_end());
            return _impl.get_iterator_at(_impl.get_count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # to do:
        /// - what happens when the value type accepts range as parameter?
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto operator+=(range_type&& range)
            requires ranges::const_range_concept<
                         typename type_info<range_type>::pure_type::value_type, value_type>
                     and (type_info<value_type>::template is_constructible_from<
                         typename type_info<range_type>::pure_type::value_type::value_type>())
        {
            _impl.insert_range_last(move(range.get_iterator()), move(range.get_iterator_end()));
        }

        /// ----------------------------------------------------------------------------------------
        /// removes value at index `index`.
        ///
        /// # parameters
        ///
        /// - `index`: index to remove value at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(usize index)
        {
            contract_debug_expects(is_index_in_range(index), "index is out of range.");

            _impl.remove_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes value at pos `it`.
        ///
        /// # parameters
        ///
        /// - `it`: pos to remove value at.
        ///
        /// # returns
        ///
        /// [iterator_type] to next value. if `it` was pointing to the last value, returns
        /// [`get_iterator_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(const_iterator_type it) -> iterator_type
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            _impl.remove_at(index);
            return _impl.get_iterator_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes values in range [[`from`, `to`]].
        ///
        /// # parameters
        ///
        /// - `from`: start of range to remove values at.
        /// - `to`: end of range to remove values at.
        ///
        /// # returns
        ///
        /// `from`.
        ///
        /// ## explanation
        ///
        /// after removing `to - from` values, next values will be shifted last to index `from`.
        /// so the next value will be available at index `from`. if the last value of the arr
        /// was also removed, `from` will be equal to [`get_count()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(usize from, usize to) -> usize
        {
            contract_debug_expects(is_index_in_range(to), "index was out of range.");
            contract_debug_expects(from <= to, "index was out of range.");

            _impl.remove_range(from, to);
            return from;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes values in range represented by `range`.
        ///
        /// # parameters
        ///
        /// - `it`: start of range to remove values at.
        /// - `it_end`: end of range to remove values at.
        ///
        /// # returns
        ///
        /// [`iterator_type`] to next value of the last removed value. if the last removed value
        /// was also the last value of the arr, returns [`get_iterator_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(
            const_iterator_type it, const_iterator_type it_end) -> iterator_type
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_valid(it_end), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range(it), "iterator is out range.");
            contract_debug_expects(is_iterator_in_range(it_end), "iterator is out range.");
            contract_debug_expects((it - it_end) <= 0, "invalid range.");

            usize from = get_index_for_iterator(it);
            usize to = get_index_for_iterator(it_end);
            _impl.remove_range(from, to);
            return _impl.get_iterator(from);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count` values from first.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_first(usize count = 1)
        {
            contract_debug_expects(count <= get_count());

            _impl.remove_first(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count` values from last.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_last(usize count = 1)
        {
            contract_debug_expects(count <= get_count());

            _impl.remove_last(count);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_one_find(const value_type& value) -> bool
        {
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (_impl.get_data()[i] == value)
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
        constexpr auto remove_all_find(const value_type& value) -> usize
        {
            usize removed_count = 0;
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (_impl.get_data()[i] == value)
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
        template <typename predicate_type>
        constexpr auto remove_one_if(predicate_type&& pred) -> bool
            requires(type_info<predicate_type>::template is_function<bool(const value_type&)>())
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
        template <typename predicate_type>
        constexpr auto remove_all_if(predicate_type&& pred) -> usize
            requires(type_info<predicate_type>::template is_function<bool(const value_type&)>())
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
        /// removes all values.
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
        constexpr auto get_data() const -> const value_type*
        {
            return _impl.get_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() -> value_type*
        {
            return _impl.get_data();
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
        constexpr auto get_iterator() const -> const_iterator_type
        {
            return _impl.get_iterator();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return _impl.get_iterator_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() -> iterator_type
        {
            return _impl.get_iterator();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return _impl.get_iterator_end();
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for `count` values. if there is already enough memory reserved,
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserve(usize count)
        {
            return _impl.reserve(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for `count` count more values.
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
        /// total count of places, with values and reserved.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_capacity() const -> usize
        {
            return _impl.get_capacity();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of places reserved for values.
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
        constexpr auto is_empty() const -> bool
        {
            return _impl.is_empty();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `index` is in range [`0`, [`get_count()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_index_in_range(usize index) const -> bool
        {
            return _impl.is_index_in_range(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `index` is in range [`0`, [`get_count()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_index_in_range_or_end(usize index) const -> bool
        {
            return _impl.is_index_in_range_or_end(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// get index for iterator `it`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_index_for_iterator(const_iterator_type it) const -> usize
        {
            return _impl.get_index_for_iterator(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks for iterator has been invalidated. this_type is done through a value which is changed
        /// for the container every time iterators are invalidated.
        ///
        /// # to do
        /// - implement iterator validation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_valid(const_iterator_type it) const -> bool
        {
            return _impl.is_iterator_valid(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iterator `pos` is in range [[`get_iterator()`], [`get_iterator_end()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_in_range(const_iterator_type it) const -> bool
        {
            return _impl.is_iterator_in_range(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iterator `pos` is in range [[`get_iterator()`], [`get_iterator_end()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_in_range_or_end(const_iterator_type it) const -> bool
        {
            return _impl.is_iterator_in_range_or_end(it);
        }

    private:
        _impl_type _impl;
    };
}
