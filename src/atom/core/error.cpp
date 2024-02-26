export module atom.core:error;
import :std;

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class error
    {
    public:
        template <typename string_view_type>
        constexpr error(string_view_type msg)
            : msg(msg.to_std_char_ptr(), msg.get_count().to_unwrapped())
        {}

    public:
        std::string_view msg;
    };
}
