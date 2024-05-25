module;
#include <cerrno>
#include <cstdio>

export module atom.core:filesystem.file;

import std;
import :core;
import :contracts;
import :strings;

namespace atom::filesystem
{
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

    export class filesystem_error: public system_error
    {
    public:
        filesystem_error(i32 error_no)
            : system_error{ error_no }
        {}
    };

    export class file
    {
    public:
        enum class open_flags
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
        /// @feature add more error types, like `file_already_exists`.
        /// ----------------------------------------------------------------------------------------
        static auto open(string_view path, open_flags flags) -> result<file, filesystem_error>
        {
            errno = 0;

            const char* c_flags = _get_flags(flags);
            FILE* c_file = std::fopen(path.get_data(), c_flags);

            if (c_file == nullptr)
            {
                return filesystem_error{ errno };
            }

            return file{ c_file, flags };
        }

        static auto get_max_open_count() -> usize
        {
            return FOPEN_MAX;
        }

        static auto get_max_filename_count() -> usize
        {
            return FILENAME_MAX;
        }

        static constexpr usize eof = EOF;

    private:
        file(FILE* file, open_flags flags)
            : _file{ file }
            , _flags{ flags }
        {}

    public:
        auto read_all() -> string
        {
            std::fseek(_file, 0, SEEK_END);
            usize size = std::ftell(_file);
            string content = string{ _with_count, size };

            std::fseek(_file, 0, SEEK_SET);
            usize read_count = std::fread(content.get_mut_data(), sizeof(char), size, _file);

            contract_asserts(read_count == size);
            return content;
        }

        auto read_remaining() -> string
        {
            usize current_pos = std::ftell(_file);
            std::fseek(_file, 0, SEEK_END);
            usize size = std::ftell(_file) - current_pos;
            string content = string{ _with_count, size };

            std::fseek(_file, current_pos, SEEK_SET);
            usize read_count = std::fread(content.get_mut_data(), sizeof(char), size, _file);

            contract_asserts(read_count == size);
            return content;
        }

        auto write_bytes(const void* data, usize count) -> void
        {
            std::fwrite(data, sizeof(byte), count, _file);
        }

        auto write_str(string_view content) -> void
        {
            std::fwrite(content.get_data(), sizeof(byte), content.get_count(), _file);
        }

        auto write_line(string_view content) -> void
        {
            std::fwrite(content.get_data(), sizeof(byte), content.get_count(), _file);
            std::fputc('\n', _file);
        }

        auto flush() -> void
        {
            contract_debug_expects(_file != nullptr);

            std::fflush(_file);
        }

        auto get_size() -> usize
        {
            long current_pos = std::ftell(_file);
            std::fseek(_file, 0, SEEK_END);

            long size = std::ftell(_file);
            std::fseek(_file, current_pos, SEEK_SET);

            return size;
        }

        auto get_size_remaining() -> usize
        {
            long current_pos = std::ftell(_file);
            std::fseek(_file, 0, SEEK_END);

            long size = std::ftell(_file);
            std::fseek(_file, current_pos, SEEK_SET);

            return size - current_pos;
        }

        auto get_pos() -> usize
        {
            long pos = std::ftell(_file);
            contract_asserts(pos != -1);

            return pos;
        }

        auto set_pos(usize pos) -> void
        {
            std::fseek(_file, pos, SEEK_SET);
        }

        auto set_pos_move(isize pos) -> void
        {
            std::fseek(_file, pos, SEEK_CUR);
        }

        auto set_pos_begin() -> void
        {
            std::fseek(_file, 0, SEEK_SET);
        }

        auto set_pos_end() -> void
        {
            std::fseek(_file, 0, SEEK_END);
        }

        auto as_range() -> decltype(auto) {}

    private:
        static constexpr auto _get_flags(open_flags flags) -> const char*
        {
            return "r";
        }

    private:
        FILE* _file;
        open_flags _flags;
    };
}
