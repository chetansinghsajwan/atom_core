#include "atom/invokable/invokable.h"

namespace atom
{
    template <typename... tsign>
    class _invokable_ptr_impl;

    template <typename result_type, typename... args_type>
    class _invokable_ptr_impl<result_type(args_type...)>
    {
    public:
        using type = result_type (*)(args_type...);
    };

    template <typename... tsign>
    using invokable_ptr = typename _invokable_ptr_impl<tsign...>::type;
}
