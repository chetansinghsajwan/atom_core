#pragma once
#include "Atom/Memory/Ptr.h"
#include "Atom/TTI.h"

namespace Atom
{
    template <typename T>
    class SharedPtr: public MutPtr<T>
    {
        static_assert(TTI::IsPure<T>, "SharedPtr only supports pure types.");
        static_assert(not TTI::IsVoid<T>, "SharedPtr doesn't support void.");

    private:
        using Base = MutPtr<T>;

    public:
        using TVal = typename Base::TVal;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr():
            Base() {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr(const SharedPtr& that);

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(const SharedPtr& that);

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr(const SharedPtr<TThat>& that)
            requires RSameOrDerivedFrom<TThat, TVal>;

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operaor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr& operator=(const SharedPtr<TThat>& that)
            requires RSameOrDerivedFrom<TThat, TVal>;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr(SharedPtr&& that);

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(SharedPtr&& that);

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr(SharedPtr<TThat>&& that)
            requires RSameOrDerivedFrom<TThat, TVal>;

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operaor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr& operator=(SharedPtr<TThat>&& that)
            requires RSameOrDerivedFrom<TThat, TVal>;

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr(T* ptr);

        /// ----------------------------------------------------------------------------------------
        /// # Value Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(T* ptr);

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~SharedPtr();
    };

    template <typename T, typename... TArgs>
    auto MakeShared(TArgs&&... args) -> SharedPtr<T>
    {
        return std::make_shared<T>(forward<TArgs>(args)...);
    }
}
