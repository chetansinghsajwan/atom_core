export module atom.core:core.char_wrapper;
import :core.byte;

namespace atom
{
    export using _char = char;

    export class uchar
    {
        using this_type = uchar;

    public:
        using unwrapped_type = char;

    public:
        constexpr uchar()
            : _val(0)
        {}

        constexpr uchar(const this_type& that) = default;
        constexpr uchar& operator=(const this_type& that) = default;

        constexpr uchar(this_type&& that) = default;
        constexpr uchar& operator=(this_type&& that) = default;

        constexpr uchar(_char ch)
            : _val(ch)
        {}

        constexpr uchar& operator=(_char ch)
        {
            _val = ch;
            return *this;
        }

        constexpr operator byte() const
        {
            return byte(_val);
        }

        constexpr ~uchar() = default;

    public:
        constexpr auto is_eq(uchar ch) const -> bool
        {
            return _val == ch._val;
        }

        constexpr auto is_lt(uchar ch) const -> bool
        {
            return _val < ch._val;
        }

        constexpr auto is_gt(uchar ch) const -> bool
        {
            return _val > ch._val;
        }

        constexpr auto is_le(uchar ch) const -> bool
        {
            return _val <= ch._val;
        }

        constexpr auto is_ge(uchar ch) const -> bool
        {
            return _val >= ch._val;
        }

        constexpr auto operator+(uchar ch) const -> uchar
        {
            return uchar(_val + ch._val);
        }

        constexpr auto operator-(uchar ch) const -> uchar
        {
            return uchar(_val - ch);
        }

        constexpr auto operator+(int n) const -> uchar
        {
            return uchar(_val + n);
        }

        constexpr auto to_unwrapped() const -> char
        {
            return _val;
        }

    public:
        _char _val;
    };

    constexpr auto _to_std_char_ptr(const uchar* ptr) -> const char*
    {
        return static_cast<const char*>(static_cast<const void*>(ptr));
    }

    constexpr auto _to_std_char_ptr(uchar* ptr) -> char*
    {
        return static_cast<char*>(static_cast<void*>(ptr));
    }

    constexpr auto _from_std_char_ptr(const char* ptr) -> const uchar*
    {
        return static_cast<const uchar*>(static_cast<const void*>(ptr));
    }

    constexpr auto _from_std_char_ptr(char* ptr) -> uchar*
    {
        return static_cast<uchar*>(static_cast<void*>(ptr));
    }
}
