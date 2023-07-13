#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        template <tname... TSign>
        class InvokablePtrImpl;

        template <tname TResult, tname... TArgs>
        class InvokablePtrImpl<TResult(TArgs...)>
        {
            pub using Type = TResult(*) (TArgs...);
        };
    }

    template <tname... TSign>
    using InvokablePtr = tname Private::InvokablePtrImpl<TSign...>::Type;
}