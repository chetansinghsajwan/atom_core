export module atom.core:containers.dynamic_array;

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
    /// - add note for case, where element or elements to be inserted are from this array.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_elem_t, typename in_allocator_t = default_mem_allocator>
    class dynamic_array: public range_extensions
    {
        static_assert(
            not typeinfo<in_elem_t>::is_ref, "dynamic_array does not supports ref types.");
        static_assert(
            not typeinfo<in_elem_t>::is_void, "dynamic_array does not support void.");

    private:
        using this_t = dynamic_array<in_elem_t, in_allocator_t>;
        using _impl_t = _dynamic_array_impl_using_std_vector<in_elem_t, in_allocator_t>;

    public:
        using value_t = in_elem_t;
        using allocator_t = in_allocator_t;
        using iter_t = array_iter<value_t>;
        using iter_end_t = iter_t;
        using mut_iter_t = mut_array_iter<value_t>;
        using mut_iter_end_t = mut_iter_t;

    public:
        static constexpr auto with_count(usize count, const value_t& value) -> this_t&&
        {
            this_t arr;
            for (usize i = 0; i < count; i++)
                arr._impl.emplace_back(value);

            return move(arr);
        }

        static constexpr auto with_capacity(usize capacity) -> this_t&&
        {
            this_t arr;
            arr.reserve(capacity);
            return move(arr);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array()
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(const dynamic_array& that)
            : _impl(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const dynamic_array& that) -> dynamic_array&
        {
            _impl.assign_range(that.get_iter(), that.get_iter_end());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(dynamic_array&& that)
            : _impl(typename _impl_t::move_tag(), that._impl)
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
        template <typename range_t>
        constexpr dynamic_array(range_t&& range)
            requires is_range_of<typename typeinfo<range_t>::pure_t::value_t, value_t>
            : _impl(typename _impl_t::range_tag(), range.get_iter(), range.get_iter_end())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr dynamic_array& operator=(range_t&& range)
            requires is_range_of<range_t, value_t>
        {
            _impl.assign_range(range.get_iter(), range.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr dynamic_array(const value_t (&array)[count])
            : _impl(typename _impl_t::range_tag(), iter_t(array), iter_t(array + count))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr dynamic_array& operator=(const value_t (&array)[count])
        {
            _impl.assign_range(iter_t(array), iter_t(array + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// # destroyor
        /// ----------------------------------------------------------------------------------------
        constexpr ~dynamic_array() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_at(usize index) const -> const value_t&
        {
            contract_debug_expects(is_index_in_range(index));

            return _impl.get_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_at(usize index) -> value_t&
        {
            contract_debug_expects(is_index_in_range(index));

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
        template <typename... arg_ts>
        constexpr auto emplace_at(usize index, arg_ts&&... args)
            requires(typeinfo<value_t>::template is_constructible_from<arg_ts...>)
        {
            contract_debug_expects(is_index_in_range_or_end(index), "index is out of range.");

            _impl.emplace_at(index, forward<arg_ts>(args)...);
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
        /// [`mut_iter_t`] to element inserted.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_ts>
        constexpr auto emplace_at(iter_t it, arg_ts&&... args) -> mut_iter_t
            requires(typeinfo<value_t>::template is_constructible_from<arg_ts...>)
        {
            contract_debug_expects(is_iter_valid(it), "invalid iter.");
            contract_debug_expects(is_iter_in_range_or_end(it), "iter is out of range.");

            usize index = get_index_for_iter(it);
            _impl.emplace_at(index, forward<arg_ts>(args)...);
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
        template <typename range_t>
        constexpr auto insert_range_at(usize index, range_t&& range) -> usize
            requires is_range_of<range_t, value_t>
                     and (typeinfo<value_t>::template is_constructible_from<
                         typename range_t::value_t>)
        {
            contract_debug_expects(is_index_in_range_or_end(index), "index is out of range.");

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
        template <typename range_t>
        constexpr auto insert_range_at(iter_t it, range_t&& range)
            requires is_range_of<range_t, value_t>
                     and (typeinfo<value_t>::template is_constructible_from<
                         typename range_t::value_t>)
        {
            contract_debug_expects(is_iter_valid(it), "invalid iter.");
            contract_debug_expects(is_iter_in_range_or_end(it), "iter is out of range.");

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
        template <typename... arg_ts>
        constexpr auto emplace_front(arg_ts&&... args)
            requires(typeinfo<value_t>::template is_constructible_from<arg_ts...>)
        {
            _impl.emplace_front(forward<arg_ts>(args)...);
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
        /// [`mut_iter_t`] to past the last inserted element.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr auto insert_range_front(range_t&& range) -> mut_iter_t
            requires is_range_of<range_t, value_t>
                     and (typeinfo<value_t>::template is_constructible_from<
                         typename range_t::value_t>)
        {
            usize count = _impl.insert_range_front(forward<range_t&&>(range));
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
        template <typename... arg_ts>
        constexpr auto emplace_back(arg_ts&&... args)
            requires(typeinfo<value_t>::template is_constructible_from<arg_ts...>)
        {
            _impl.emplace_back(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename arg_t>
        constexpr auto operator+=(arg_t&& el)
            requires typeinfo<value_t>::template
        is_constructible_from<arg_t>
        {
            _impl.emplace_back(forward<arg_t>(el));
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
        /// [`mut_iter_t`] to the first inserted element.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr auto insert_range_back(range_t&& range) -> mut_iter_t
            requires is_range_of<typename typeinfo<range_t>::pure_t::value_t, value_t>
                     and (typeinfo<value_t>::template is_constructible_from<
                         typename typeinfo<range_t>::pure_t::value_t::value_t>)
        {
            usize count = _impl.insert_range_back(range.get_iter(), range.get_iter_end());
            return _impl.get_mut_iter_at(_impl.get_count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # to do:
        /// - what happens when the elem type accepts range as parameter?
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr auto operator+=(range_t&& range)
            requires is_range_of<typename typeinfo<range_t>::pure_t::value_t, value_t>
                     and (typeinfo<value_t>::template is_constructible_from<
                         typename typeinfo<range_t>::pure_t::value_t::value_t>)
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
            contract_debug_expects(is_index_in_range(index), "index is out of range.");

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
        /// [mut_iter_t] to next element. if `it` was pointing to the last element, returns
        /// [`get_iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(iter_t it) -> mut_iter_t
        {
            contract_debug_expects(is_iter_valid(it), "invalid iter.");
            contract_debug_expects(is_iter_in_range(it), "iter is out of range.");

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
            contract_debug_expects(is_index_in_range(to), "index was out of range.");
            contract_debug_expects(from <= to, "index was out of range.");

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
        /// [`mut_iter_t`] to next element of the last removed element. if the last removed element
        /// was also the last element of the arr, returns [`get_iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(iter_t it, iter_t it_end) -> mut_iter_t
        {
            contract_debug_expects(is_iter_valid(it), "invalid iter.");
            contract_debug_expects(is_iter_valid(it_end), "invalid iter.");
            contract_debug_expects(is_iter_in_range(it), "iter is out range.");
            contract_debug_expects(is_iter_in_range(it_end), "iter is out range.");
            contract_debug_expects(it.compare(it_end) <= 0, "invalid range.");

            usize from = get_index_for_iter(it);
            usize to = get_index_for_iter(it_end);
            _impl.remove_range(from, to);
            return _impl.get_mut_iter(from);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count` elements from front.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_front(usize count = 1)
        {
            contract_debug_expects(count <= get_count());

            _impl.remove_front(count);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count` elements from back.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_back(usize count = 1)
        {
            contract_debug_expects(count <= get_count());

            _impl.remove_back(count);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_one_find(const value_t& elem) -> bool
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
        constexpr auto remove_all_find(const value_t& elem) -> usize
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
        template <typename predicate_t>
        constexpr auto remove_one_if(predicate_t&& pred) -> bool
            requires typeinfo<predicate_t>::template
        is_invokable<bool(const value_t&)>
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
        template <typename predicate_t>
        constexpr auto remove_all_if(predicate_t&& pred) -> usize
            requires typeinfo<predicate_t>::template
        is_invokable<bool(const value_t&)>
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
        constexpr auto get_data() const -> const value_t*
        {
            return _impl.get_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> value_t*
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
        constexpr auto get_iter() const -> iter_t
        {
            return _impl.get_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_t
        {
            return _impl.get_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter() -> mut_iter_t
        {
            return _impl.get_mut_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter_end() -> mut_iter_end_t
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
        constexpr auto get_allocator() const -> const allocator_t&
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
        constexpr auto get_index_for_iter(iter_t it) const -> usize
        {
            return _impl.get_index_for_iter(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks for iter has been invalidated. this_t is done through a value which is changed
        /// for the container every time iters are invalidated.
        ///
        /// # to do
        /// - implement iter validation.
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_valid(iter_t it) const
        {
            return _impl.is_iter_valid(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iter `pos` is in range [[`get_iter()`], [`get_iter_end()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_in_range(iter_t it) const
        {
            return _impl.is_iter_in_range(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iter `pos` is in range [[`get_iter()`], [`get_iter_end()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_in_range_or_end(iter_t it) const
        {
            return _impl.is_iter_in_range_or_end(it);
        }

    private:
        _impl_t _impl;
    };
}
