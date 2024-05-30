export module atom.core:dynamic_buffer;

import std;
import :core;
import :default_mem_allocator;
import :mem_helper;

namespace atom
{
    export template <typename allocator_type = default_mem_allocator>
    class dynamic_buffer
    {
        using this_type = dynamic_buffer;

    public:
        constexpr dynamic_buffer(_with_size_type, usize size)
            : _data{ nullptr }
            , _size{ size }
            , _allocator{}
        {
            _data = _allocator.alloc(size);
        }

        template <typename value_type>
        constexpr dynamic_buffer(create_from_std_vector_tag, const std::vector<value_type>& vec)
            : _data{ nullptr }
            , _size{ vec.size() * sizeof(value_type) }
            , _allocator{}
        {
            _data = static_cast<byte*>(_allocator.alloc(_size));

            mem_helper::copy_to(vec.data(), _size, _data);
        }

        constexpr dynamic_buffer(const this_type& that)
            : _data{ nullptr }
            , _size{ that._size }
            , _allocator{ that._allocator }
        {
            _data = static_cast<byte*>(_allocator.alloc(_size));
        }

        constexpr dynamic_buffer& operator=(const this_type& that)
        {
            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
            }

            _allocator = that._allocator;
            _size = that._size;
            _data = static_cast<byte*>(_allocator.alloc(_size));
        }

        constexpr dynamic_buffer(this_type&& that)
            : _data{ that._data }
            , _size{ that._size }
            , _allocator{ move(that._allocator) }
        {
            that._data = nullptr;
            that._size = 0;
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

        constexpr auto release() -> void
        {
            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
                _data = nullptr;
                _size = 0;
            }
        }

        constexpr auto resize(usize size) -> void
        {
            if (_data != nullptr)
            {
                _allocator.dealloc(_data);
            }

            if (size == 0)
            {
                _data = nullptr;
                _size = 0;
                return;
            }

            _data = static_cast<byte*>(_allocator.alloc(size));
            _size = size;
        }

    private:
        byte* _data;
        usize _size;
        allocator_type _allocator;
    };
}
