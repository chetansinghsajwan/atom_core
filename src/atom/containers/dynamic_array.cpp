export module atom.core:containers.dynamic_array;
import :std;
import :core;
import :tti;
import :range;
import :mem_ptr;
import :invokable;
import :memory.default_mem_allocator;
import :contracts_decl;
import :array_iter;

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
            insert_range_back(it, it_end);
        }

        constexpr ~_dynamic_array_impl()
        {
            _destruct_all();
            _release_all_mem();
        }

    public:
        // checked
        constexpr auto get_at(usize i) const -> const elem_type&
        {
            return _data[i];
        }

        // checked
        constexpr auto get_mut_at(usize i) -> elem_type&
        {
            return _data[i];
        }

        // checked
        constexpr auto get_iter() const -> iter_type
        {
            return iter_type(_data);
        }

        // checked
        constexpr auto get_iter_at(usize i) const -> iter_type
        {
            return iter_type(_data + i);
        }

        // checked
        constexpr auto get_iter_end() const -> iter_end_type
        {
            return iter_end_type(_data + _count);
        }

        // checked
        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return mut_iter_type(_data);
        }

        // checked
        constexpr auto get_mut_iter_at(usize i) -> mut_iter_type
        {
            return mut_iter_type(_data + i);
        }

        // checked
        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return mut_iter_end_type(_data + _count);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto assign_range(other_iter_type it, other_iter_end_type it_end)
        {
            remove_all();
            insert_range_back(it, it_end);
        }

        template <typename... arg_types>
        constexpr auto emplace_at(usize i, arg_types&&... args)
        {
            return _emplace_at(i, forward<arg_types>(args)...);
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto insert_range_at(usize i, other_iter_type it, other_iter_end_type it_end)
            -> usize
        {
            if constexpr (_can_get_range_size<other_iter_type, other_iter_end_type>())
            {
                return _insert_range_at_counted(i, it, _get_range_size(it, it_end));
            }
            else
            {
                return _insert_range_at_uncounted(i, move(it), move(it_end));
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
            return insert_range_at(0, it, it_end);
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
                usize count = _get_range_size(it, it_end);
                _insert_range_back_counted(it, count);
                return count;
            }
            else
            {
                return _insert_range_back_uncounted(move(it), move(it_end));
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
            _move_range_front(begin + count, count);
        }

        // checked
        constexpr auto remove_all()
        {
            if (_count != 0)
            {
                _destruct_all();
                _count = 0;
            }
        }

        // checked
        constexpr auto reserve(usize count)
        {
            _ensure_cap_for(count);
        }

        constexpr auto reserve_more(usize count)
        {
            _ensure_cap_for(_count + count);
        }

        constexpr auto release_unused_mem() {}

        // checked
        constexpr auto get_capacity() const -> usize
        {
            return _capacity;
        }

        // checked
        constexpr auto get_count() const -> usize
        {
            return _count;
        }

        // checked
        constexpr auto get_data() const -> mem_ptr<elem_type>
        {
            return _data;
        }

        // checked
        constexpr auto get_mut_data() -> mut_mem_ptr<elem_type>
        {
            return _data;
        }

        // checked
        constexpr auto get_allocator() const -> const allocator_type&
        {
            return _allocator;
        }

        // checked
        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return i < _count;
        }

        // checked
        constexpr auto is_index_in_range_or_end(usize i) const -> bool
        {
            return i <= _count;
        }

        constexpr auto is_iter_valid(iter_type it) const -> bool
        {
            return true;
        }

        // checked
        constexpr auto get_index_for_iter(iter_type it) const -> usize
        {
            isize i = it.get_data() - _data;
            return i < 0 ? usize::max() : usize(i);
        }

        // checked
        constexpr auto is_iter_in_range(iter_type it) const -> bool
        {
            return get_index_for_iter(it) < _count;
        }

        // checked
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
        constexpr auto _emplace_at(usize i, arg_types&&... args) -> usize
        {
            _ensure_cap_for(1);

            if (_count != 0)
                _move_range_back(i, 1);

            _construct_at(i, forward<arg_types>(args)...);
            _count += 1;

            return i;
        }

        template <typename other_iter_type>
        constexpr auto _insert_range_at_counted(usize i, other_iter_type it, usize count) -> usize
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

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto _insert_range_at_uncounted(
            usize i, other_iter_type it, other_iter_end_type it_end) -> usize
        {
            usize rotate_size = _count - i;
            _insert_range_back_uncounted(move(it), move(it_end));
            _rotate_range_back(i, rotate_size);

            return i;
        }

        template <typename u>
        constexpr auto _insert_back(u&& el)
        {
            _ensure_cap_for(1);
            _construct_at(_count, forward<u>(el));
            _count += 1;
        }

        template <typename other_iter_type>
        constexpr auto _insert_range_back_counted(other_iter_type it, usize count)
        {
            if (count == 0)
                return;

            _ensure_cap_for(count);

            for (usize j = 0; j < count; j++)
            {
                _construct_at(_count + j, forward<decltype(it.value())>(it.value()));
                it.next();
            }

            _count = count + 1;
        }

        template <typename other_iter_type, typename other_iter_end_type>
        constexpr auto _insert_range_back_uncounted(other_iter_type it, other_iter_end_type it_end)
            -> usize
        {
            usize count = 0;
            while (not it.is_eq(it_end))
            {
                _ensure_cap_for(1);
                _construct_at(_count + count, it.value());

                it.next();
                count++;
            }

            _count += count;
            return count;
        }

        constexpr auto _update_iter_debug_id() {}

        constexpr auto _calc_cap_growth(usize required) const -> usize
        {
            return _count.mul(2).max_of(_count + required);
        }

        constexpr auto _ensure_cap_for(usize count)
        {
            // we have enough capacity.
            if (_capacity - _count >= count)
                return;

            _update_iter_debug_id();

            usize new_cap = _calc_cap_growth(count);
            mut_mem_ptr<elem_type> new_data = _allocator.alloc(new_cap);

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
        constexpr auto _construct_at(usize i, arg_types&&... args) -> void
        {
            mut_mem_ptr<elem_type> src = _data + i;
            std::construct_at(src.to_unwrapped(), forward<arg_types>(args)...);
        }

        constexpr auto _destruct_at(usize i) -> void
        {
            mut_mem_ptr<elem_type> src = _data + i;
            std::destroy_at(src.to_unwrapped());
        }

        constexpr auto _destruct_range(usize i, usize count) -> void
        {
            mut_mem_ptr<elem_type> begin = _data + i;
            mut_mem_ptr<elem_type> end = begin + count;
            std::destroy(begin.to_unwrapped(), end.to_unwrapped());
        }

        constexpr auto _destruct_all() -> void
        {
            mut_mem_ptr<elem_type> begin = _data;
            mut_mem_ptr<elem_type> end = _data + _count;
            std::destroy(begin.to_unwrapped(), end.to_unwrapped());
        }

        constexpr auto _move_range_front(usize i, usize steps) -> void
        {
            mut_mem_ptr<elem_type> begin = _data + i;
            mut_mem_ptr<elem_type> end = _data + _count;
            mut_mem_ptr<elem_type> dest = begin - steps;
            std::move(begin.to_unwrapped(), end.to_unwrapped(), dest.to_unwrapped());
        }

        constexpr auto _move_range_back(usize i, usize steps) -> void
        {
            mut_mem_ptr<elem_type> begin = _data + i;
            mut_mem_ptr<elem_type> end = _data + _count;
            mut_mem_ptr<elem_type> dest = begin + steps;
            std::move_backward(begin.to_unwrapped(), end.to_unwrapped(), dest.to_unwrapped());
        }

        constexpr auto _move_range_to(usize i, mut_mem_ptr<elem_type> dest) -> void
        {
            mut_mem_ptr<elem_type> begin = _data + i;
            mut_mem_ptr<elem_type> end = _data + _count;
            std::move(begin.to_unwrapped(), end.to_unwrapped(), dest.to_unwrapped());
        }

        constexpr auto _rotate_range_back(usize i, usize count) -> void
        {
            mut_mem_ptr<elem_type> begin = _data;
            mut_mem_ptr<elem_type> mid = begin + i;
            mut_mem_ptr<elem_type> end = begin + _count;
            std::rotate(begin.to_unwrapped(), mid.to_unwrapped(), end.to_unwrapped());
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
        mut_mem_ptr<elem_type> _data;
        usize _count;
        usize _capacity;
        allocator_type _allocator;
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
        static_assert(not tti::is_ref<in_elem_type>, "dynamic_array does not supports ref types.");
        static_assert(not tti::is_void<in_elem_type>, "dynamic_array does not support void.");

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
            _impl.storage().move(move(that));
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
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~basic_dynamic_array() {}

    public:
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
        template <typename... arg_types>
        constexpr auto emplace_at(usize i, arg_types&&... args)
            requires(rconstructible<elem_type, arg_types...>)
        {
            contracts::debug_expects(is_index_in_range_or_end(i), "index is out of range.");

            _impl.emplace_at(i, forward<arg_types>(args)...);
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
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_in_range_or_end(it), "iter is out of range.");

            usize i = get_index_for_iter(it);
            _impl.emplace_at(i, forward<arg_types>(args)...);
            return _impl.get_mut_iter_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// inserts elements at index `i`. forwards each value returned by `range.get_iter()` to
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

            return _impl.insert_range_at(i, range.get_iter(), range.get_iter_end());
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
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_in_range_or_end(it), "iter is out of range.");

            usize i = get_index_for_iter(it);
            usize count = _impl.insert_range_at(i, range.get_iter(), range.get_iter_end());
            return make_range(_impl.get_mut_iter_at(i), _impl.get_mut_iter_at(i + count));
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
        /// [`get_iter_end()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_at(iter_type it) -> mut_iter_type
        {
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_in_range(it), "iter is out of range.");

            usize i = get_index_for_iter(it);
            _impl.remove_at(i);
            return _impl.get_mut_iter_at(i);
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
            contracts::debug_expects(is_index_in_range(to), "index was out of range.");
            contracts::debug_expects(from <= to, "index was out of range.");
            // todo: what should we do about fnret?
            // contracts::debug_ensures(fnret <= get_count(), "invalid return value.");

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
            contracts::debug_expects(is_iter_valid(it), "invalid iter.");
            contracts::debug_expects(is_iter_valid(it_end), "invalid iter.");
            contracts::debug_expects(is_iter_in_range(it), "iter is out range.");
            contracts::debug_expects(is_iter_in_range(it_end), "iter is out range.");
            contracts::debug_expects(it.compare(it_end) <= 0, "invalid range.");

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
            contracts::debug_expects(count_ <= get_count());

            _impl.remove_front(count_);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes `count_` elements from back.
        /// ----------------------------------------------------------------------------------------
        constexpr auto remove_back(usize count_ = 1)
        {
            contracts::debug_expects(count_ <= get_count());

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
        constexpr auto get_data() const -> mem_ptr<elem_type>
        {
            return _impl.get_data();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> mut_mem_ptr<elem_type>
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
        constexpr auto get_iter(usize i = 0) const -> iter_type
        {
            contracts::debug_expects(is_index_in_range_or_end(i));

            return _impl.get_iter_at(i);
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
        constexpr auto get_mut_iter(usize i = 0) -> mut_iter_type
        {
            contracts::debug_expects(is_index_in_range_or_end(i));

            return _impl.get_mut_iter_at(i);
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
        /// checks if index `i` is in range [`0`, [`get_count()`]).
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_index_in_range(usize i) const
        {
            return _impl.is_index_in_range(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if index `i` is in range [`0`, [`get_count()`]].
        /// ----------------------------------------------------------------------------------------
        constexpr bool is_index_in_range_or_end(usize i) const
        {
            return _impl.is_index_in_range_or_end(i);
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
    export template <typename elem_type, typename allocator_type = default_mem_allocator>
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
