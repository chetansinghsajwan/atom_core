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
    /// \todo add doc.
    /// \todo add complexities.
    /// \todo add note for cases, where value or a range of values to be inserted are from this array.
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
        using impl_type = dynamic_array_impl_vector<in_value_type, in_allocator_type>;
        using value_type_info = type_info<in_value_type>;

    public:
        using value_type = in_value_type;
        using allocator_type = in_allocator_type;
        using const_iterator_type = array_iterator<value_type>;
        using const_iterator_end_type = const_iterator_type;
        using iterator_type = mut_array_iterator<value_type>;
        using iterator_end_type = iterator_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// initializes with nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array()
            : _impl{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// initializes by copying each value to a new allocated array.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(const dynamic_array& that)
            : _impl{ typename impl_type::copy_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// destorys current values, and copies each value of `that` to `this` array. reallocation
        /// my happen during this.
        ///
        /// \note all iterators are invalidated after this.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array& operator=(const dynamic_array& that)
        {
            _impl.assign_range(that.get_iterator(), that.get_iterator_end());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// initializes by moving underlying storage from `that` to `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(dynamic_array&& that)
            : _impl{ typename impl_type::move_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// destroys current values, and moves `that` storage into `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array& operator=(dynamic_array&& that)
        {
            _impl.move_this(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// initializes by copying or moving each value from range into a new allocated array.
        /// if the array was passed as an rvalue reference, each value is moved, else copied.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr dynamic_array(range_type&& range)
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    value_type>)
            : _impl{ typename impl_type::range_tag{}, ranges::get_iterator(range),
                ranges::get_iterator_end(range) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// destroyes current values, and copies or moves each value of `that` to `this` array.
        /// reallocation may happen during this.
        ///
        /// \note all iterators are invalidated after this.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr dynamic_array& operator=(range_type&& range)
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    value_type>)
        {
            _impl.assign_range(ranges::get_iterator(range), ranges::get_iterator_end(range));
        }

        /// ----------------------------------------------------------------------------------------
        /// initializes with `count` count default constructed values.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_with_count_type, usize count)
            requires(value_type_info::is_default_constructible())
            : _impl{ create_with_count, count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// initializes with `count` count values, constructed using `value`.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_with_count_type, usize count, const value_type& value)
            : _impl{ create_with_count, count, value }
        {}

        /// ----------------------------------------------------------------------------------------
        /// initializes with `capacity` capacity.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_with_capacity_type, usize capacity)
            : _impl{ create_with_capacity, capacity }
        {}

        /// ----------------------------------------------------------------------------------------
        /// initializes with raw data `arr` and count `count`.
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(create_from_raw_tag, const value_type* arr, usize count)
            : _impl{ create_from_raw, arr, count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// destroys each value and releases allocated memory.
        /// ----------------------------------------------------------------------------------------
        constexpr ~dynamic_array() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// \returns reference to value at index `i`.
        ///
        /// \pre if debug `is_index_in_range(index)`: index is out of bounds.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_at(usize i) -> value_type&
        {
            contract_debug_expects(is_index_in_range(i));

            return _impl.get_at(i);
        }

        /// \copydoc get_at
        constexpr auto get_at(usize i) const -> const value_type&
        {
            contract_debug_expects(is_index_in_range(i));

            return _impl.get_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at index `i` with `args`.
        ///
        /// \pre if debug `is_index_in_range_or_end(i)`: index is out of range.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(usize i, arg_types&&... args) -> void
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            contract_debug_expects(is_index_in_range_or_end(i), "index is out of range.");

            _impl.emplace_at(i, forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at position referenced by `it` with `args`.
        ///
        /// \pre if debug `contract_debug_expects(is_iterator_valid(it)` invalid iterator.
        /// \pre if debug `contract_debug_expects(is_iterator_in_range_or_end(it)`
        ///     iterator is out of range.
        ///
        /// \returns iterator_type to value added.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
            requires(value_type_info::template is_constructible_from<arg_types...>())
        constexpr auto emplace_at(const_iterator_type it, arg_types&&... args) -> iterator_type
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            _impl.emplace_at(index, forward<arg_types>(args)...);
            return _impl.get_iterator_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at first position with `args`.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_first(arg_types&&... args) -> void
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_first(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value at last position with `args`.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_last(arg_types&&... args) -> void
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_last(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs `count` number of values at index `i`, each value is constructed with `args`.
        ///
        /// \pre if debug `is_index_in_range_or_end(i)`: index is out of range.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_many_at(usize i, usize count, const arg_types&... args) -> void
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            contract_debug_expects(is_index_in_range_or_end(i), "index is out of range.");

            _impl.emplace_at(i, forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs `count` number of values at position referenced by `it`, each value is
        /// constructed with `args`.
        ///
        /// \pre if debug `contract_debug_expects(is_iterator_valid(it)` invalid iterator.
        /// \pre if debug `contract_debug_expects(is_iterator_in_range_or_end(it)`
        ///     iterator is out of range.
        ///
        /// \returns iterator_type to the first value added.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_many_at(
            const_iterator_type it, usize count, const arg_types&... args) -> iterator_type
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            _impl.emplace_many_at(index, count, forward<arg_types>(args)...);
            return _impl.get_iterator_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs `count` number of values at first position, each value is constructed with
        /// `args`.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_many_first(usize count, const arg_types&... args) -> void
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_many_first(count, args...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs `count` number of value at last position, each value is constructed with
        /// `args`.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_many_last(usize count, const arg_types&... args) -> void
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_many_last(count, args...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `emplace_last(value)`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+=(const value_type& value) -> void
        {
            _impl.emplace_last(value);
        }

        /// \copydoc operator+=
        constexpr auto operator+=(value_type&& value) -> void
        {
            _impl.emplace_last(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at index `i`. forwards each value returned by `range`'s iterator to the
        /// constructor of `value_type` in the array.
        ///
        /// \returns count of values inserted.
        ///
        /// \pre if debug `is_index_in_range_or_end(i)`: index is out of range.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(usize i, range_type&& range) -> usize
            requires(ranges::const_range_concept<range_type, value_type>
                     and value_type_info::template is_constructible_from<
                         ranges::value_type<range_type>>())
        {
            contract_debug_expects(is_index_in_range_or_end(i), "index is out of range.");

            return _impl.insert_range_at(
                i, ranges::get_iterator(range), ranges::get_iterator_end(range));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at position referenced by `it`. forwards each value returned by
        /// `range`'s iterator to the constructor of value in the array
        ///
        /// \returns `mut_array_range` of values inserted.
        ///
        /// \pre if debug `is_iterator_valid(it)`: invalid iterator.
        /// \pre if debug `is_iterator_in_range_or_end(it)`: iterator is out of range.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(const_iterator_type it, range_type&& range)
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    value_type>
                and value_type_info::template is_constructible_from<
                    ranges::value_type<typename type_info<range_type>::pure_type::value_type>>())
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            usize count = _impl.insert_range_at(
                index, ranges::get_iterator(range), ranges::get_iterator_end(range));
            return ranges::from(_impl.get_iterator_at(index), _impl.get_iterator_at(index + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at first position. forwards each value returned by `range`'s iterator to
        /// the constructor of value in the array.
        ///
        /// \returns `iterator_type` to next of the last inserted value.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_first(range_type&& range) -> iterator_type
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    value_type>
                and value_type_info::template is_constructible_from<
                    ranges::value_type<typename type_info<range_type>::pure_type::value_type>>())
        {
            usize count = _impl.insert_range_first(move(range));
            return _impl.get_iterator(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts values at last. forwards each value returned by `range`'s iterator to
        /// the constructor of value in the array.
        ///
        /// \returns `iterator_type` to the first inserted value.
        ///
        /// \note all iterators are invalidated after this operation.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_last(range_type&& range) -> iterator_type
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    value_type>
                and value_type_info::template is_constructible_from<
                    ranges::value_type<typename type_info<range_type>::pure_type::value_type>>())
        {
            usize count = _impl.insert_range_last(
                ranges::get_iterator(range), ranges::get_iterator_end(range));
            return _impl.get_iterator_at(_impl.get_count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `insert_range_last`
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto operator+=(range_type&& range)
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    value_type>
                and value_type_info::template is_constructible_from<
                    ranges::value_type<typename type_info<range_type>::pure_type::value_type>>())
        {
            _impl.insert_range_last(ranges::get_iterator(range), ranges::get_iterator_end(range));
        }

        /// ----------------------------------------------------------------------------------------
        /// removes value at index `i`.
        ///
        /// \pre if debug `is_index_in_range(i)`: i is out of range.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(usize i) -> void
        {
            contract_debug_expects(is_index_in_range(i), "i is out of range.");

            _impl.remove_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes value at position referenced by `it`.
        ///
        /// \returns `iterator_type` to next value. if `it` was pointing to the last value,
        /// returns `get_iterator_end()`.
        ///
        /// \pre if debug `is_iterator_valid(it)`: invalid iterator.
        /// \pre if debug `is_iterator_in_range(it)`: iterator is out of range.
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
        /// removes values from index `from` to `to`.
        ///
        /// \pre if debug `is_index_in_range(to)`: index was out of range.
        /// \pre if debug `from <= to`: index was out of range.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(usize from, usize to) -> void
        {
            contract_debug_expects(is_index_in_range(to), "index was out of range.");
            contract_debug_expects(from <= to, "index was out of range.");

            _impl.remove_range(from, to);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes values from position referenced by `from` to `to`.
        ///
        /// \returns `iterator_type` to next value of the last removed value. if the last
        /// removed value was also the last value of the array, returns `get_iterator_end()`.
        ///
        /// \pre if debug `is_iterator_valid(from)`: invalid iterator.
        /// \pre if debug `is_iterator_valid(to)`: invalid iterator.
        /// \pre if debug `is_iterator_in_range(from)`: iterator is out range.
        /// \pre if debug `is_iterator_in_range(to)`: iterator is out range.
        /// \pre if debug `(from - to) <= 0`: invalid range.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(
            const_iterator_type from, const_iterator_type to) -> iterator_type
        {
            contract_debug_expects(is_iterator_valid(from), "invalid iterator.");
            contract_debug_expects(is_iterator_valid(to), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range(from), "iterator is out range.");
            contract_debug_expects(is_iterator_in_range(to), "iterator is out range.");
            contract_debug_expects((from - to) <= 0, "invalid range.");

            usize from_index = get_index_for_iterator(from);
            usize to_index = get_index_for_iterator(to);
            _impl.remove_range(from_index, to_index);

            return _impl.get_iterator(from_index);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count` values from first position.
        ///
        /// \pre if debug `count <= get_count()`: `this` doesn't contain enough values.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_first(usize count = 1) -> void
        {
            contract_debug_expects(count <= get_count(), "doesn't contain enough values.");

            _impl.remove_first(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count` values from last position.
        ///
        /// \pre if debug `count <= get_count()`: `this` doesn't contain enough values.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_last(usize count = 1) -> void
        {
            contract_debug_expects(count <= get_count());

            _impl.remove_last(count);
        }

        static constexpr usize npos = nums::get_max_usize();

        /// ----------------------------------------------------------------------------------------
        /// removes first entry compairing equal to `value`.
        ///
        /// \returns index at which the entry was removed, if no entry was removed returns
        /// `npos`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_one_find(const value_type& value) -> usize
        {
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (_impl.get_data()[i] == value)
                {
                    _impl.remove_at(i);
                    return i;
                }
            }

            return npos;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes all entries compairing equal to `value`.
        ///
        /// \returns count of values removed.
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
        /// removes first entry for which `pred` returns `true`.
        ///
        /// \returns index of entry which was removed.
        /// ----------------------------------------------------------------------------------------
        template <typename predicate_type>
        constexpr auto remove_one_if(predicate_type&& pred) -> usize
            requires(type_info<predicate_type>::template is_function<bool(const value_type&)>())
        {
            for (usize i = 0; i < _impl.get_count(); i++)
            {
                if (pred(_impl.get_data()[i]))
                {
                    _impl.remove_at(i);
                    return i;
                }
            }

            return npos;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes all entries for which `pred` returns `true`.
        ///
        /// \returns count of entries removed.
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
        /// \note does not free storage.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_all()
        {
            _impl.remove_all();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns pointer to underlying array.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() -> value_type*
        {
            return _impl.get_data();
        }

        /// \copydoc get_data
        constexpr auto get_data() const -> const value_type*
        {
            return _impl.get_data();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns count of values `this` contains.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _impl.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns iterator to first value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() const -> const_iterator_type
        {
            return _impl.get_iterator();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns iterator to next the last value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return _impl.get_iterator_end();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns mut iterator to the first value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() -> iterator_type
        {
            return _impl.get_iterator();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns mut iterator to next the last value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return _impl.get_iterator_end();
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for `count` values. if there is already enough memory reserved,
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserve(usize count) -> void
        {
            return _impl.reserve(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for `count` count more values, even if there is already space for
        /// `count` value, this still allocates new memory for `count` more values.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserve_more(usize count) -> void
        {
            return _impl.reserve_more(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// releases unused memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto release_mem() -> void
        {
            return _impl.release_unused_mem();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns total number of places this has, filled or empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_capacity() const -> usize
        {
            return _impl.get_capacity();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns count of places reserved for new values.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_reserved_count() const -> usize
        {
            return _impl.get_capacity() - _impl.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns reference to stored allocator.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_allocator() const -> const allocator_type&
        {
            return _impl.get_allocator();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if `get_count() == 0`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_empty() const -> bool
        {
            return _impl.is_empty();
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if index `i` is in range `[0, get_count())`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return _impl.is_index_in_range(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if index `i` is in range `[0, get_count()]`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_index_in_range_or_end(usize i) const -> bool
        {
            return _impl.is_index_in_range_or_end(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns index for iterator `it`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_index_for_iterator(const_iterator_type it) const -> usize
        {
            return _impl.get_index_for_iterator(it);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_valid(const_iterator_type it) const -> bool
        {
            return _impl.is_iterator_valid(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if iterator `it` is in range `[get_iterator(), get_iterator_end())`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_in_range(const_iterator_type it) const -> bool
        {
            return _impl.is_iterator_in_range(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if iterator `it` is in range `[get_iterator(), get_iterator_end()]`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_in_range_or_end(const_iterator_type it) const -> bool
        {
            return _impl.is_iterator_in_range_or_end(it);
        }

    private:
        impl_type _impl;
    };
}
