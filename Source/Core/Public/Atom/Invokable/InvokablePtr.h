#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        template <typename... TSign>
        struct InvokablePtrImpl;

        template <typename TResult, typename... TArgs>
        struct InvokablePtrImpl<TResult(TArgs...)>
        {
            using Type = TResult(*) (TArgs...);
        };
    }

    template <typename... TSign>
    using InvokablePtr = Private::InvokablePtrImpl<TSign...>::Type;
}