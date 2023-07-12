#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        template <tname... TSign>
        struct InvokablePtrImpl;

        template <tname TResult, tname... TArgs>
        struct InvokablePtrImpl<TResult(TArgs...)>
        {
            using Type = TResult(*) (TArgs...);
        };
    }

    template <tname... TSign>
    using InvokablePtr = tname Private::InvokablePtrImpl<TSign...>::Type;
}