#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        template <typename... TSign>
        class InvokablePtrImpl;

        template <typename TResult, typename... TArgs>
        class InvokablePtrImpl<TResult(TArgs...)>
        {
        public:
            using Type = TResult(*) (TArgs...);
        };
    }

    template <typename... TSign>
    using InvokablePtr = typename Private::InvokablePtrImpl<TSign...>::Type;
}