export module atom_core:dynamic_buffer;

import std;
import :core;
import :default_mem_allocator;
import :memory_utils;
import :ranges;

namespace atom
{
    export class dynamic_buffer
    {
        using this_type = dynamic_buffer;
        using allocator_type = default_mem_allocator;

    public:
        constexpr dynamic_buffer()
            : _data{ nullptr }
            , _size{ 0 }
            , _capacity{ 0 }
            , _allocator{}
        {}

        constexpr dynamic_buffer(const this_type& that)
            : _data{ nullptr }
            , _size{ that._size }
            , _capacity{ that._size }
            , _allocator{ that._allocator }
        {
            _data = static_cast<byte*>(_allocator.alloc(_capacity));

            memory_utils::copy_to(that._data, _size, _data);
        }

        constexpr dynamic_buffer& operator=(const this_type& that)
        {
            _set_data(that._data, that._size);
            return *this;
        }

        constexpr dynamic_buffer(this_type&& that)
            : _data{ that._data }
            , _size{ that._size }
            , _capacity{ that._capacity }
            , _allocator{ move(that._allocator) }
        {
            that._data = nullptr;
            that._size = 0;
            that._capacity = 0;
        }

        constexpr dynamic_buffer& operator=(this_type&& that)
        {
            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
            }

            _data = that._data;
            _size = that._size;
            _allocator = move(that._allocator);

            that._data = nullptr;
            that._size = 0;
            that._capacity = 0;

            return *this;
        }

        constexpr dynamic_buffer(create_with_size_tag, usize size)
            : _data{ nullptr }
            , _size{ size }
            , _capacity{ size }
            , _allocator{}
        {
            _data = static_cast<byte*>(_allocator.alloc(_capacity));
        }

        template <typename range_type>
        constexpr dynamic_buffer(create_from_range_tag, const range_type& range)
            requires(ranges::const_array_range_concept<range_type>)
            : _data{ nullptr }
            , _size{ ranges::get_count(range) * sizeof(ranges::value_type<range_type>) }
            , _capacity{ ranges::get_count(range) * sizeof(ranges::value_type<range_type>) }
            , _allocator{}
        {
            _data = static_cast<byte*>(_allocator.alloc(_capacity));

            memory_utils::copy_to(ranges::get_data(range), _size, _data);
        }

        constexpr ~dynamic_buffer()
        {
            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
            }
        }

    public:
        constexpr auto get_data() -> byte*
        {
            return _data;
        }

        constexpr auto get_data() const -> const byte*
        {
            return _data;
        }

        constexpr auto get_size() const -> usize
        {
            return _size;
        }

        constexpr auto get_capacity() const -> usize
        {
            return _capacity;
        }

        constexpr auto release() -> void
        {
            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
                _data = nullptr;
                _size = 0;
                _capacity = 0;
            }
        }

        constexpr auto resize(usize size) -> void
        {
            if (_size == size)
            {
                return;
            }

            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
            }

            if (size == 0)
            {
                _size = 0;
                _capacity = 0;
                _data = nullptr;
                return;
            }

            _size = size;
            _capacity = 0;
            _data = static_cast<byte*>(_allocator.alloc(_capacity));
        }

        template <typename value_type>
        constexpr auto set_to_std_vector(const std::vector<value_type>& vector) -> void
        {
            _set_data(vector.data(), vector.size() * sizeof(value_type));
        }

    private:
        constexpr auto _set_data(const void* data, usize size) -> void
        {
            if (_data == nullptr)
            {
                _size = size;
                _capacity = size;
                _data = static_cast<byte*>(_allocator.alloc(_capacity));

                memory_utils::copy_to(data, _size, _data);
                return;
            }

            // if we don't have enough storage
            if (_capacity < size)
            {
                _allocator.dealloc(_data);

                _capacity = size;
                _data = static_cast<byte*>(_allocator.alloc(_capacity));
            }

            _size = size;
            memory_utils::copy_to(data, _size, _data);
        }

    private:
        byte* _data;
        usize _size;
        usize _capacity;
        allocator_type _allocator;
    };

    export class memory_view
    {
    public:
        constexpr memory_view()
            : _data{ nullptr }
            , _size{ 0 }
        {}

        constexpr memory_view(const void* data, usize size)
            : _data{ static_cast<const byte*>(data) }
            , _size{ size }
        {}

    public:
        constexpr auto get_data() const -> const byte*
        {
            return _data;
        }

        constexpr auto get_size() const -> usize
        {
            return _size;
        }

    private:
        const byte* _data;
        usize _size;
    };
}
