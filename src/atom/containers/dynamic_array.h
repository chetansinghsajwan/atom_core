#pragma once
#include "atom/contracts.h"
#include "atom/invokable/invokable.h"
#include "atom/memory/default_mem_allocator.h"
#include "atom/range.h"
#include "_dynamic_array_impl.h"

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
        static_assert(not tti::is_ref<in_elem_type>, "dynamic_array doesn't supports ref types.");
        static_assert(not tti::is_void<in_elem_type>, "dynamic_array doesn't support void.");

    private:
        using _impl_type = _dynamic_array_impl<in_elem_type, in_allocator_type>;

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
            : _impl{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_dynamic_array(const basic_dynamic_array& that)
            : _impl{ that.iter(), that.iter_end() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const basic_dynamic_array& that) -> basic_dynamic_array&
        {
            _impl.assign_range(that.iter(), that.iter_end());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_dynamic_array(basic_dynamic_array&& that)
            : _impl{ mov(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(basic_dynamic_array&& that) -> basic_dynamic_array&
        {
            _impl.storage().move(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type,
            typename trange_unqualified = tti::tremove_quailfiers_ref<range_type>>
        constexpr basic_dynamic_array(range_type&& range)
            requires(rrange_of<trange_unqualified, elem_type>)
            : _impl{ range.iter(), range.iter_end() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr basic_dynamic_array& operator=(range_type&& range)
            requires(rrange_of<range_type, elem_type>)
        {
            _impl.assign_range(range.iter(), range.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~basic_dynamic_array()
        {
            _impl.on_destruct();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> mem_ptr<elem_type>
        {
            return _impl.data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_data() -> mut_mem_ptr<elem_type>
        {
            return _impl.mut_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _impl.count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter(usize i = 0) const -> iter_type
        {
            return _impl.iter(i);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter_end() const -> iter_end_type
        {
            return _impl.iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_iter(usize i = 0) -> mut_iter_type
        {
            return _impl.mut_iter(i);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_iter_end() -> mut_iter_end_type
        {
            return _impl.mut_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs element at index `i` with `args`.
        ///
        /// # parameters
        ///
        /// - `i`: index to insert element at.
        /// - `args...`: args to construct element with.
        ///
        /// # time complexity
        ///
        /// # iter invalidation
        ///
        /// all iters are invalidated.
        /// ----------------------------------------------------------------------------------------
        template <typename... args_type>
        constexpr auto emplace_at(usize i, args_type&&... args)
            requires(rconstructible<elem_type, args_type...>)
        {
            contracts::debug_expects(is_index_in_range_or_end(i), "index is out of range.");

            _impl.emplace_at(i, forward<args_type>(args)...);
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
        template <typename... args_type>
        constexpr auto emplace_at(iter_type it, args_type&&... args) -> mut_iter_type
            requires(rconstructible<elem_type, args_type...>)
        {
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_in_range_or_end(it), "iter is out of range.");

            usize i = index_for_iter(it);
            _impl.emplace_at(i, forward<args_type>(args)...);
            return _impl.mut_iter(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at index `i`. forwards each value returned by `range.iter()` to
        /// constructor of element in the arr.
        ///
        /// # parameters
        ///
        /// - `i`: index to insert elements at.
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
        constexpr auto insert_range_at(usize i, range_type&& range) -> usize
            requires(rrange_of<range_type, elem_type>)
                    and (rconstructible<elem_type, typename range_type::elem_type>)
        {
            contracts::debug_expects(is_index_in_range_or_end(i), "index is out of range.");

            return _impl.insert_range_at(i, range.iter(), range.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at pos `it`. forwards each value returned by `range.iter()` to
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
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_in_range_or_end(it), "iter is out of range.");

            usize i = index_for_iter(it);
            usize count = _impl.insert_range_at(i, range.iter(), range.iter_end());
            return make_range(_impl.mut_iter(i), _impl.mut_iter(i + count));
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
        template <typename... args_type>
        constexpr auto emplace_front(args_type&&... args)
            requires(rconstructible<elem_type, args_type...>)
        {
            _impl.emplace_front(forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at front. forwards each value returned by `range.iter()` to
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
            return _impl.mut_iter(count);
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
        template <typename... args_type>
        constexpr auto emplace_back(args_type&&... args)
            requires(rconstructible<elem_type, args_type...>)
        {
            _impl.emplace_back(forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename targ>
        constexpr auto operator+=(targ&& el)
            requires(rconstructible<elem_type, targ>)
        {
            _impl.emplace_back(forward<targ>(el));
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at back. forwards each value returned by `range.iter()` to
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
            usize count = _impl.insert_range_back(range.iter(), range.iter_end());
            return _impl.mut_iter(_impl.count() - count);
        }

        /// ----------------------------------------------------------------------------------------
        /// # to do:
        /// - what happens when the elem type accepts range as parameter?
        /// ----------------------------------------------------------------------------------------
        template <typename range_type,
            typename trange_unqualified = tti::tremove_quailfiers_ref<range_type>>
        constexpr auto operator+=(range_type&& range)
            requires(rrange_of<trange_unqualified, elem_type>)
                    and (rconstructible<elem_type, typename trange_unqualified::elem_type>)
        {
            _impl.insert_range_back(mov(range.iter()), mov(range.iter_end()));
        }

        /// ----------------------------------------------------------------------------------------
        /// removes element at index `i`.
        ///
        /// # parameters
        ///
        /// - `i`: index to remove element at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(usize i)
        {
            contracts::debug_expects(is_index_in_range(i), "index is out of range.");

            _impl.remove_at(i);
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
        /// [`iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(iter_type it) -> mut_iter_type
        {
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_in_range(it), "iter is out of range.");

            usize i = index_for_iter(it);
            _impl.remove_at(i);
            return _impl.mut_iter(i);
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
        /// was also removed, `from` will be equal to [`count()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(usize from, usize to) -> usize
        {
            contracts::debug_expects(from <= to, "invalid range.");
            contracts::debug_expects(is_index_in_range(to), "index was out of range.");
            // todo: what should we do about fnret?
            // contracts::debug_ensures(fnret <= count(), "invalid return value.");

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
        /// was also the last element of the arr, returns [`iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_range(iter_type it, iter_type it_end) -> mut_iter_type
        {
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_valid(it_end), "invalid iter.");
            contracts::debug_expects(is_iter_in_range(it), "iter is out range.");
            contracts::debug_expects(is_iter_in_range(it_end), "iter is out range.");
            contracts::debug_expects(it.compare(it_end) <= 0, "invalid range.");

            usize from = index_for_iter(it);
            usize to = index_for_iter(it_end);
            _impl.remove_range(from, to);
            return _impl.mut_iter(from);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count_` elements from front.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_front(usize count_ = 1)
        {
            contracts::debug_expects(count_ <= count());

            _impl.remove_front(count_);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count_` elements from back.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_back(usize count_ = 1)
        {
            contracts::debug_expects(count_ <= count());

            _impl.remove_back(count_);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tpred>
        constexpr auto remove_if(tpred&& pred) -> usize
            requires(rinvokable<tpred, bool(const elem_type&)>)
        {
            usize removed_count = 0;
            for (usize i = 0; i < _impl.count(); i++)
            {
                if (pred(_impl.data()[i]))
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
        /// - doesn't free storage.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_all()
        {
            _impl.remove_all();
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
        constexpr auto capacity() const -> usize
        {
            return _impl.capacity();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of places reserved for elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reserved_count() const -> usize
        {
            return _impl.capacity() - _impl.count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto alloc() const -> const allocator_type&
        {
            return _impl.alloc();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `i` is in range [`0`, [`count()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_index_in_range(usize i) const
        {
            return _impl.is_index_in_range(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `i` is in range [`0`, [`count()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_index_in_range_or_end(usize i) const
        {
            return _impl.is_index_in_range_or_end(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// get index for iter `it`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index_for_iter(iter_type it) const -> usize
        {
            return _impl.index_for_iter(it);
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
        /// checks if iter `pos` is in range [[`iter()`], [`iter_end()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_iter_in_range(iter_type it) const
        {
            return _impl.is_iter_in_range(it);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if iter `pos` is in range [[`iter()`], [`iter_end()`]].
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
