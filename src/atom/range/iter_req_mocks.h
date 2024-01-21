#pragma once
#include "atom/core.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// mock object used as `iter_end`.
    /// --------------------------------------------------------------------------------------------
    class iter_end_req_mock
    {};

    /// --------------------------------------------------------------------------------------------
    /// `riter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        iter_req_mock() = delete;
        iter_req_mock(const iter_req_mock& that) = delete;
        iter_req_mock& operator=(const iter_req_mock& that) = delete;

        iter_req_mock(iter_req_mock&& that) = default;
        iter_req_mock& operator=(iter_req_mock&& that) = default;
        ~iter_req_mock() = default;

    public:
        auto value() const -> const type&;
        auto is_eq(iter_end_req_mock end) const -> bool;
        auto next() -> iter_req_mock&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        mut_iter_req_mock() = delete;
        mut_iter_req_mock(const mut_iter_req_mock& that) = delete;
        mut_iter_req_mock& operator=(const mut_iter_req_mock& that) = delete;

        mut_iter_req_mock(mut_iter_req_mock&& that) = default;
        mut_iter_req_mock& operator=(mut_iter_req_mock&& that) = default;
        ~mut_iter_req_mock() = default;

    public:
        auto value() const -> const type&;
        auto mut_value() -> type&;
        auto is_eq(iter_end_req_mock end) const -> bool;
        auto next() -> mut_iter_req_mock&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rfwd_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class fwd_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto mut_value() -> type&;
        auto next() -> fwd_iter_req_mock&;
        auto is_eq(iter_end_req_mock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_fwd_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_fwd_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto mut_value() -> type&;
        auto next() -> mut_fwd_iter_req_mock&;
        auto is_eq(iter_end_req_mock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rbidi_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class bidi_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto next() -> bidi_iter_req_mock&;
        auto prev() -> bidi_iter_req_mock&;
        auto is_eq(iter_end_req_mock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_bidi_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_bidi_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto mut_value() -> type&;
        auto next() -> mut_bidi_iter_req_mock&;
        auto prev() -> mut_bidi_iter_req_mock&;
        auto is_eq(iter_end_req_mock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rjump_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class jump_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto next(usize steps = 1) -> jump_iter_req_mock&;
        auto prev(usize steps = 1) -> jump_iter_req_mock&;
        auto is_eq(iter_end_req_mock end) const -> bool;
        auto compare(const jump_iter_req_mock& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_jump_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_jump_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto mut_value() -> type&;
        auto next(usize steps = 1) -> mut_jump_iter_req_mock&;
        auto prev(usize steps = 1) -> mut_jump_iter_req_mock&;
        auto is_eq(iter_end_req_mock end) const -> bool;
        auto compare(const mut_jump_iter_req_mock& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rarray_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class array_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto data() const -> mem_ptr<type>;
        auto next(usize steps = 1) -> array_iter_req_mock&;
        auto prev(usize steps = 1) -> array_iter_req_mock&;
        auto is_eq(array_iter_req_mock end) const -> bool;
        auto compare(const array_iter_req_mock& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_array_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_array_iter_req_mock
    {
    public:
        using elem_type = type;

    public:
        auto value() const -> const type&;
        auto mut_value() -> type&;
        auto data() const -> mem_ptr<type>;
        auto mut_data() -> mut_mem_ptr<type>;
        auto next(usize steps = 1) -> mut_array_iter_req_mock&;
        auto prev(usize steps = 1) -> mut_array_iter_req_mock&;
        auto is_eq(mut_array_iter_req_mock end) const -> bool;
        auto compare(const mut_array_iter_req_mock& that) const -> isize;
    };
}
