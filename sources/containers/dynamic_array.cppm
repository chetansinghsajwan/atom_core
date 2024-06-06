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
    export template <typename in_elem_type, typename in_allocator_type = default_mem_allocator>
    class dynamic_array
    {
        static_assert(typeinfo<in_elem_type>::is_pure, "dynamic_array does not non pure types.");
        static_assert(not typeinfo<in_elem_type>::is_void, "dynamic_array does not support void.");

    private:
        using this_type = dynamic_array<in_elem_type, in_allocator_type>;
        using _impl_type = _dynamic_array_impl_vector<in_elem_type, in_allocator_type>;

    public:
        using value_type = in_elem_type;
        using allocator_type = in_allocator_type;
        using iterator_type = array_iterator<value_type>;
        using iterator_end_type = iterator_type;
        using mut_iterator_type = mut_array_iterator<value_type>;
        using mut_iterator_end_type = mut_iterator_type;

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
            _impl.move_typehis(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr dynamic_array(range_type&& range)
            requires ranges::is_range_of<typename typeinfo<range_type>::pure_type::value_type, value_type>
            : _impl{ typename _impl_type::range_tag{}, range.get_iterator(), range.get_iterator_end() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr dynamic_array& operator=(range_type&& range)
            requires ranges::is_range_of<range_type, value_type>
        {
            _impl.assign_range(range.get_iterator(), range.get_iterator_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr dynamic_array(const value_type (&array)[count])
            : _impl{ typename _impl_type::range_tag{}, iterator_type(array), iterator_type(array + count) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr dynamic_array& operator=(const value_type (&array)[count])
        {
            _impl.assign_range(iterator_type(array), iterator_type(array + count));
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(_with_count_type, usize count)
            : _impl{ _with_count, count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(_with_count_type, usize count, const value_type& value)
            : _impl{ _with_count, count, value }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr dynamic_array(_with_capacity_type, usize capacity)
            : _impl{ _with_count, capacity }
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
        constexpr auto get_mut_at(usize index) -> value_type&
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
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(usize index, arg_types&&... args)
            requires(typeinfo<value_type>::template is_constructible_from<arg_types...>)
        {
            contract_debug_expects(is_index_in_range_or_end(index), "index is out of range.");

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
        /// [`mut_iterator_type`] to element inserted.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_at(iterator_type it, arg_types&&... args) -> mut_iterator_type
            requires(typeinfo<value_type>::template is_constructible_from<arg_types...>)
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            _impl.emplace_at(index, forward<arg_types>(args)...);
            return _impl.get_mut_iterator_at(index);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at index `index`. forwards each value returned by `range.get_iterator()` to
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
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(usize index, range_type&& range) -> usize
            requires ranges::is_range_of<range_type, value_type>
                     and (typeinfo<value_type>::template is_constructible_from<
                         typename range_type::value_type>)
        {
            contract_debug_expects(is_index_in_range_or_end(index), "index is out of range.");

            return _impl.insert_range_at(index, range.get_iterator(), range.get_iterator_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at pos `it`. forwards each value returned by `range.get_iterator()` to
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
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_at(iterator_type it, range_type&& range)
            requires ranges::is_range_of<range_type, value_type>
                     and (typeinfo<value_type>::template is_constructible_from<
                         typename range_type::value_type>)
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range_or_end(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            usize count =
                _impl.insert_range_at(index, range.get_iterator(), range.get_iterator_end());
            return ranges::from(
                _impl.get_mut_iterator_at(index), _impl.get_mut_iterator_at(index + count));
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
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_front(arg_types&&... args)
            requires(typeinfo<value_type>::template is_constructible_from<arg_types...>)
        {
            _impl.emplace_front(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at front. forwards each value returned by `range.get_iterator()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `range`: range of elements to insert.
        ///
        /// # returns
        ///
        /// [`mut_iterator_type`] to past the last inserted element.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_front(range_type&& range) -> mut_iterator_type
            requires ranges::is_range_of<range_type, value_type>
                     and (typeinfo<value_type>::template is_constructible_from<
                         typename range_type::value_type>)
        {
            usize count = _impl.insert_range_front(forward<range_type&&>(range));
            return _impl.get_mut_iterator(count);
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
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_back(arg_types&&... args)
            requires(typeinfo<value_type>::template is_constructible_from<arg_types...>)
        {
            _impl.emplace_back(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename arg_type>
        constexpr auto operator+=(arg_type&& el)
            requires typeinfo<value_type>::template
        is_constructible_from<arg_type>
        {
            _impl.emplace_back(forward<arg_type>(el));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at back. forwards each value returned by `range.get_iterator()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `range`: range of elements to insert.
        ///
        /// # returns
        ///
        /// [`mut_iterator_type`] to the first inserted element.
        ///
        /// # time complexity
        ///
        /// # iterator invalidation
        ///
        /// all iterators are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto insert_range_back(range_type&& range) -> mut_iterator_type
            requires ranges::is_range_of<typename typeinfo<range_type>::pure_type::value_type, value_type>
                     and (typeinfo<value_type>::template is_constructible_from<
                         typename typeinfo<range_type>::pure_type::value_type::value_type>)
        {
            usize count = _impl.insert_range_back(range.get_iterator(), range.get_iterator_end());
            return _impl.get_mut_iterator_at(_impl.get_count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # to do:
        /// - what happens when the elem type accepts range as parameter?
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto operator+=(range_type&& range)
            requires ranges::is_range_of<typename typeinfo<range_type>::pure_type::value_type, value_type>
                     and (typeinfo<value_type>::template is_constructible_from<
                         typename typeinfo<range_type>::pure_type::value_type::value_type>)
        {
            _impl.insert_range_back(move(range.get_iterator()), move(range.get_iterator_end()));
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
        /// [mut_iterator_type] to next element. if `it` was pointing to the last element, returns
        /// [`get_iterator_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(iterator_type it) -> mut_iterator_type
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range(it), "iterator is out of range.");

            usize index = get_index_for_iterator(it);
            _impl.remove_at(index);
            return _impl.get_mut_iterator_at(index);
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
        /// [`mut_iterator_type`] to next element of the last removed element. if the last removed element
        /// was also the last element of the arr, returns [`get_iterator_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(iterator_type it, iterator_type it_end) -> mut_iterator_type
        {
            contract_debug_expects(is_iterator_valid(it), "invalid iterator.");
            contract_debug_expects(is_iterator_valid(it_end), "invalid iterator.");
            contract_debug_expects(is_iterator_in_range(it), "iterator is out range.");
            contract_debug_expects(is_iterator_in_range(it_end), "iterator is out range.");
            contract_debug_expects(it.compare(it_end) <= 0, "invalid range.");

            usize from = get_index_for_iterator(it);
            usize to = get_index_for_iterator(it_end);
            _impl.remove_range(from, to);
            return _impl.get_mut_iterator(from);
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
        constexpr auto remove_one_find(const value_type& elem) -> bool
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
        constexpr auto remove_all_find(const value_type& elem) -> usize
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
        template <typename predicate_type>
        constexpr auto remove_one_if(predicate_type&& pred) -> bool
            requires typeinfo<predicate_type>::template
        is_function<bool(const value_type&)>
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
            requires typeinfo<predicate_type>::template
        is_function<bool(const value_type&)>
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
        constexpr auto get_data() const -> const value_type*
        {
            return _impl.get_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> value_type*
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
        constexpr auto get_iterator() const -> iterator_type
        {
            return _impl.get_iterator();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() const -> iterator_end_type
        {
            return _impl.get_iterator_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iterator() -> mut_iterator_type
        {
            return _impl.get_mut_iterator();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iterator_end() -> mut_iterator_end_type
        {
            return _impl.get_mut_iterator_end();
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
        constexpr auto get_index_for_iterator(iterator_type it) const -> usize
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
        constexpr auto is_iterator_valid(iterator_type it) const -> bool
        {
            return _impl.is_iterator_valid(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iterator `pos` is in range [[`get_iterator()`], [`get_iterator_end()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_in_range(iterator_type it) const -> bool
        {
            return _impl.is_iterator_in_range(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iterator `pos` is in range [[`get_iterator()`], [`get_iterator_end()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_iterator_in_range_or_end(iterator_type it) const -> bool
        {
            return _impl.is_iterator_in_range_or_end(it);
        }

    private:
        _impl_type _impl;
    };
}
