module;

#include <chrono>
#include <source_location>
#include <string_view>
#include <iostream>
#include <type_traits>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <typeinfo>
#include <string_view>
#include <string>
#include <concepts>
#include <cstdint>
#include <cmath>
#include <numeric>
#include <atomic>
#include <memory>
#include <mutex>
#include <initializer_list>
#include <cstring>
#include <functional>

export module atom.core:std;

export namespace std
{
    using std::operator==;
    using std::operator!=;
    using std::operator>;
    using std::operator>=;
    using std::operator<;
    using std::operator<=;
    using std::operator+;
    using std::operator-;
    using std::operator&;
    using std::operator&=;
    using std::operator<<=;
    using std::operator>>=;
    using std::operator~;
    using std::operator^;
    using std::operator^=;
    using std::operator|;
    using std::operator|=;
    using std::operator>>;
    using std::operator<<;
    using std::operator<=>;

    using std::basic_string;
    using std::basic_string_view;
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::forward;
    using std::move;
    using std::source_location;
    using std::string;
    using std::string_view;
    using std::vector;

    using std::conditional_t;
    using std::exception;
    using std::is_constant_evaluated;
    using std::mt19937;
    using std::random_device;
    using std::terminate;
    using std::uniform_int_distribution;

    using std::decay_t;
    using std::enable_if_t;
    using std::is_assignable_v;
    using std::is_base_of_v;
    using std::is_const_v;
    using std::is_constructible_v;
    using std::is_copy_assignable_v;
    using std::is_copy_constructible_v;
    using std::is_default_constructible_v;
    using std::is_destructible_v;
    using std::is_empty_v;
    using std::is_floating_point_v;
    using std::is_integral_v;
    using std::is_invocable_r_v;
    using std::is_invocable_v;
    using std::is_lvalue_reference_v;
    using std::is_move_assignable_v;
    using std::is_move_constructible_v;
    using std::is_polymorphic_v;
    using std::is_reference_v;
    using std::is_rvalue_reference_v;
    using std::is_same_v;
    using std::is_signed_v;
    using std::is_trivially_assignable_v;
    using std::is_trivially_constructible_v;
    using std::is_trivially_copy_assignable_v;
    using std::is_trivially_copy_constructible_v;
    using std::is_trivially_default_constructible_v;
    using std::is_trivially_destructible_v;
    using std::is_trivially_move_assignable_v;
    using std::is_trivially_move_constructible_v;
    using std::is_unsigned_v;
    using std::is_void_v;
    using std::is_volatile_v;
    using std::remove_const_t;
    using std::remove_cv_t;
    using std::remove_cvref_t;
    using std::remove_reference_t;

    using std::construct_at;
    using std::destroy;
    using std::destroy_at;
    using std::move;
    using std::move_backward;
    using std::nullptr_t;
    using std::rotate;
    using std::type_info;
    using std::unordered_map;

    using std::derived_from;
    using std::destructible;
    using std::same_as;

    using std::abs;
    using std::ceil;
    using std::floor;
    using std::round;

    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::int8_t;
    using std::int_fast16_t;
    using std::int_fast32_t;
    using std::int_fast64_t;
    using std::int_fast8_t;
    using std::int_least16_t;
    using std::int_least32_t;
    using std::int_least64_t;
    using std::int_least8_t;
    using std::intmax_t;
    using std::numeric_limits;
    using std::ptrdiff_t;
    using std::size_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::uint8_t;
    using std::uint_fast16_t;
    using std::uint_fast32_t;
    using std::uint_fast64_t;
    using std::uint_fast8_t;
    using std::uint_least16_t;
    using std::uint_least32_t;
    using std::uint_least64_t;
    using std::uint_least8_t;
    using std::uintmax_t;

    using std::atomic;
    using std::copy;
    using std::copy_backward;
    using std::equal;
    using std::fill;
    using std::find;
    using std::free;
    using std::initializer_list;
    using std::lock;
    using std::make_unique;
    using std::malloc;
    using std::realloc;
    using std::rotate;
    using std::shift_left;
    using std::shift_right;
    using std::strlen;

    using std::bidirectional_iterator;
    using std::bidirectional_iterator_tag;
    using std::contiguous_iterator;
    using std::contiguous_iterator_tag;
    using std::forward_iterator;
    using std::forward_iterator_tag;
    using std::input_iterator;
    using std::input_iterator_tag;
    using std::random_access_iterator;
    using std::random_access_iterator_tag;

    using std::function;
    using std::hash;
    using std::mutex;

    namespace chrono
    {
        using std::chrono::system_clock;
    }
}
