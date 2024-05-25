module;
#include <cerrno>
#include <cstdio>

export module atom.core:filesystem.file;

import std;
import fmt;
import :core;
import :contracts;
import :strings;

namespace atom::filesystem
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export class system_error: public error
    {
    public:
        system_error(i32 error_no)
            : error_no{ error_no }
            , error{ string_view::from_cstr(std::strerror(error_no)) }
        {}

    public:
        i32 error_no;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export class filesystem_error: public system_error
    {
    public:
        filesystem_error(i32 error_no)
            : system_error{ error_no }
        {}
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export class invalid_options_error: public error
    {
    public:
        invalid_options_error(string_view msg)
            : error{ msg }
        {}
    };

    export class file
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        enum class open_flags : byte
        {
            create = 1 << 0,    // allow creating file if doesn't exists
            overwrite = 1 << 1, // allow overwriting file if it exists
            read = 1 << 2,      // allow reading from the file
            write = 1 << 3,     // allow writing to the file
            append = 1 << 4,    // write to the file from the last
            binary = 1 << 5,    // open file in binary mode.
        };

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static auto get_max_open_count() -> usize
        {
            return FOPEN_MAX;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static auto get_max_filename_count() -> usize
        {
            return FILENAME_MAX;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize eof = EOF;

    private:
        file(FILE* file, open_flags flags)
            : _file{ file }
            , _flags{ flags }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// @feature add more error types, like `file_already_exists`.
        /// ----------------------------------------------------------------------------------------
        static auto open(string_view path,
            open_flags flags) -> result<file, filesystem_error, invalid_options_error>
        {
            errno = 0;

            const char* c_mode = _get_mode(flags);
            if (c_mode == nullptr)
            {
                return invalid_options_error{ "invalid flags combination." };
            }

            FILE* c_file = std::fopen(path.get_data(), c_mode);

            if (c_file == nullptr)
            {
                return filesystem_error{ errno };
            }

            return file{ c_file, flags };
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto reopen(open_flags flags) -> result<void, filesystem_error, invalid_options_error>
        {
            contract_expects(not is_closed(), "the file is closed.");

            errno = 0;

            const char* c_mode = _get_mode(flags);
            if (c_mode == nullptr)
            {
                return invalid_options_error{ "invalid flags combination." };
            }

            _file = std::freopen(nullptr, c_mode, _file);
            _flags = flags;

            if (_file == nullptr)
            {
                return filesystem_error{ errno };
            }

            return result_void();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto close() -> void
        {
            contract_expects(not is_closed(), "the file is closed.");

            std::fclose(_file);
            _file = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto is_closed() const -> bool
        {
            return _file == nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto read_all() -> string
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fseek(_file, 0, SEEK_END);
            usize size = std::ftell(_file);
            string content = string{ _with_count, size };

            std::fseek(_file, 0, SEEK_SET);
            usize read_count = std::fread(content.get_mut_data(), sizeof(char), size, _file);

            contract_asserts(read_count == size);
            return content;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto read_remaining() -> string
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            usize current_pos = std::ftell(_file);
            std::fseek(_file, 0, SEEK_END);
            usize size = std::ftell(_file) - current_pos;
            string content = string{ _with_count, size };

            std::fseek(_file, current_pos, SEEK_SET);
            usize read_count = std::fread(content.get_mut_data(), sizeof(char), size, _file);

            contract_asserts(read_count == size);
            return content;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto write_bytes(const void* data, usize count) -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fwrite(data, sizeof(byte), count, _file);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto write_str(string_view content) -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fwrite(content.get_data(), sizeof(byte), content.get_count(), _file);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto write_fmt(format_string<arg_types...> fmt, arg_types&&... args) -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            fmt::print(_file, _convert_format_string_atom_to_fmt(fmt), fowrard<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto write_line(string_view content) -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fwrite(content.get_data(), sizeof(byte), content.get_count(), _file);
            std::fputc('\n', _file);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto flush() -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            contract_debug_expects(_file != nullptr);

            std::fflush(_file);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_size() -> usize
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            long current_pos = std::ftell(_file);
            std::fseek(_file, 0, SEEK_END);

            long size = std::ftell(_file);
            std::fseek(_file, current_pos, SEEK_SET);

            return size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_size_remaining() -> usize
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            long current_pos = std::ftell(_file);
            std::fseek(_file, 0, SEEK_END);

            long size = std::ftell(_file);
            std::fseek(_file, current_pos, SEEK_SET);

            return size - current_pos;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_pos() -> usize
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            long pos = std::ftell(_file);
            contract_asserts(pos != -1);

            return pos;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto set_pos(usize pos) -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fseek(_file, pos, SEEK_SET);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto set_pos_move(isize pos) -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fseek(_file, pos, SEEK_CUR);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto set_pos_begin() -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fseek(_file, 0, SEEK_SET);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto set_pos_end() -> void
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            std::fseek(_file, 0, SEEK_END);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_flags() -> open_flags
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            return _flags;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto can_read() const -> bool
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            return (_flags & open_flags::read) == open_flags::read;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto can_write() const -> bool
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            return (_flags & open_flags::write) == open_flags::write;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto is_binary() const -> bool
        {
            contract_debug_expects(not is_closed(), "the file is closed.");

            return (_flags & open_flags::binary) == open_flags::binary;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto as_range() -> decltype(auto)
        {
            contract_debug_expects(not is_closed(), "the file is closed.");
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// @todo fix clang warning `case value not in enumerated type 'open_flags' [-Wswitch]`
        /// ----------------------------------------------------------------------------------------
        static constexpr auto _get_mode(open_flags flags) -> const char*
        {
            switch (flags)
            {
                case open_flags::read:                                               return "r";
                case open_flags::write:
                case open_flags::write | open_flags::create:
                case open_flags::write | open_flags::overwrite:                     return "w";
                case open_flags::append:                                             return "a";
                case open_flags::read | open_flags::binary:                          return "rb";
                case open_flags::write | open_flags::binary:
                case open_flags::write | open_flags::create | open_flags::binary:
                case open_flags::write | open_flags::overwrite | open_flags::binary: return "wb";
                case open_flags::append | open_flags::binary:                        return "ab";
                case open_flags::read | open_flags::write:                           return "r+";
                case open_flags::read | open_flags::write | open_flags::binary:      return "r+b";
                case open_flags::read | open_flags::write | open_flags::overwrite:
                case open_flags::read | open_flags::write | open_flags::create:      return "w+";
                case open_flags::read | open_flags::write | open_flags::overwrite
                    | open_flags::binary:
                case open_flags::read | open_flags::write | open_flags::create | open_flags::binary:
                    return "w+b";
                case open_flags::read | open_flags::write | open_flags::append: return "a+";
                case open_flags::read | open_flags::write | open_flags::append | open_flags::binary:
                    return "a+b";
                default: return nullptr;
            }
        }

    private:
        FILE* _file;
        open_flags _flags;
    };
}
