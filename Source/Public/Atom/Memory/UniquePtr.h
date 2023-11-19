#pragma once
#include "Atom/Memory/Ptr.h"
#include "Atom/TTI.h"

namespace Atom
{
    template <typename T>
    class UniquePtr: public MutPtr<T>
    {
        static_assert(TTI::IsPure<T>, "UniquePtr only supports pure types.");
        static_assert(not TTI::IsVoid<T>, "UniquePtr doesn't support void.");

    private:
        using Base = MutPtr<T>;

    public:
        using TVal = typename Base::TVal;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr():
            Base() {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr(const UniquePtr& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr& operator=(const UniquePtr& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr(UniquePtr&& that);

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr& operator=(UniquePtr&& that);

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr UniquePtr(UniquePtr<TThat>&& that)
            requires RSameOrDerivedFrom<TThat, TVal>;

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operaor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr UniquePtr& operator=(UniquePtr<TThat>&& that)
            requires RSameOrDerivedFrom<TThat, TVal>;

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr(T* ptr);

        /// ----------------------------------------------------------------------------------------
        /// # Value Operator
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr& operator=(T* ptr);

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~UniquePtr();
    };

    template <typename T, typename... TArgs>
    auto MakeUnique(TArgs&&... args) -> UniquePtr<T>
    {
        return std::make_unique<T>(forward<TArgs>(args)...);
    }
}
