#pragma once
#include "Atom/Memory/ObjectBox.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"
#include "Atom/String.h"

#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        struct InvokableBoxIdentifier { };

        template <typename TResult, typename... TArgs>
        struct Invoker
        {
            template <RInvokable<TResult(TArgs...)> TInvokable>
            void Set()
            {
                m_impl = [](void* obj, TResult& result, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    new (&result) TResult(invokable(FORWARD(args)...));
                };
            }

            TResult Invoke(void* invokable, TArgs&&... args)
            {
                TResult result;
                m_impl(invokable, result, FORWARD(args)...);

                return result;
            }

        protected:
            void (*m_impl) (void* invokable, TResult& result, TArgs&&... args);
        };

        template <typename... TArgs>
        struct Invoker <void, TArgs...>
        {
            template <RInvokable<void(TArgs...)> TInvokable>
            void Set()
            {
                m_impl = [](void* obj, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    invokable(FORWARD(args)...);
                };
            }

            void Invoke(void* invokable, TArgs&&... args)
            {
                m_impl(invokable, FORWARD(args)...);
            }

        protected:
            void (*m_impl) (void* invokable, TArgs&&... args);
        };
    }

    /// InvokableBox declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename>
    class InvokableBox;

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TResult, typename... TArgs>
    class InvokableBox <TResult(TArgs...)> :
        public ObjectBox<true, true, true, 50, DefaultMemAllocator>,
        public Private::InvokableBoxIdentifier
    {
    public:
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr InvokableBox() noexcept { }

        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        InvokableBox(NullType null) noexcept :
            ObjectBox(null) { }

        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        InvokableBox& operator = (NullType null)
        {
            ObjectBox::operator = (null);
            return *this;
        }

        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        bool operator == (NullType null) const noexcept
        {
            return ObjectBox::operator == (null);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
            requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        InvokableBox(TInvokable&& invokable) :
            ObjectBox(FORWARD(invokable))
        {
            m_SetInvoker<TInvokable>();
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
            requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        InvokableBox& operator = (TInvokable&& invokable)
        {
            ObjectBox::operator = (FORWARD(invokable));
            m_SetInvoker<TInvokable>();
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(const InvokableBox& other) :
            ObjectBox(other) { }

        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox& operator = (const InvokableBox& other)
        {
            ObjectBox::operator = (other);
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(InvokableBox&& other) :
            ObjectBox(MOVE(other)) { }

        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox& operator = (InvokableBox&& other)
        {
            ObjectBox::operator = (MOVE(other));
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        ~InvokableBox() { }

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        TResult Invoke(TArgs&&... args)
        {
            ATOM_ASSERT_THROW(ObjectBox::_HasObject(), NullPointerException(
                TEXT("InvokableTarget is null.")));

            return m_invoker.Invoke(ObjectBox::_GetObject(), FORWARD(args)...);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        TResult operator () (TArgs&&... args)
        {
            return Invoke(FORWARD(args)...);
        }

    public:
        template <typename T>
        T* GetInvokable() noexcept
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        const TypeInfo& GetInvokableType() const noexcept
        {
            return ObjectBox::_GetObjectType();
        }

    protected:
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
        void m_SetInvoker()
        {
            m_invoker.template Set<TInvokable>();
        }

        TResult m_InvokeInvokable(TArgs&&... args)
        {
            return m_invoker.Invoke(FORWARD(args)...);
        }

    protected:
        using TInvoker = Private::Invoker<TResult, TArgs...>;

        TInvoker m_invoker;
    };
}