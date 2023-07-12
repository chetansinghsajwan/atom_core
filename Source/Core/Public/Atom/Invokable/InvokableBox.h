#pragma once
#include "Atom/Memory/ObjectBox.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"
#include "Atom/Str.h"

#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        struct InvokableBoxIdentifier { };

        template <tname TResult, tname... TArgs>
        struct Invoker
        {
            template <RInvokable<TResult(TArgs...)> TInvokable>
            void Set()
            {
                m_impl = [](void* obj, TResult& result, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    new (&result) TResult(invokable(fwd(args)...));
                };
            }

            TResult Invoke(void* invokable, TArgs&&... args)
            {
                TResult result;
                m_impl(invokable, result, fwd(args)...);

                return result;
            }

        protected:
            void (*m_impl) (void* invokable, TResult& result, TArgs&&... args);
        };

        template <tname... TArgs>
        struct Invoker <void, TArgs...>
        {
            template <RInvokable<void(TArgs...)> TInvokable>
            void Set()
            {
                m_impl = [](void* obj, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    invokable(fwd(args)...);
                };
            }

            void Invoke(void* invokable, TArgs&&... args)
            {
                m_impl(invokable, fwd(args)...);
            }

        protected:
            void (*m_impl) (void* invokable, TArgs&&... args);
        };
    }

    /// InvokableBox declaration.
    /// --------------------------------------------------------------------------------------------
    template <tname>
    class InvokableBox;

    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TResult, tname... TArgs>
    class InvokableBox <TResult(TArgs...)> :
        public ObjectBox<true, true, true, 50, DefaultMemAllocator>,
        public Private::InvokableBoxIdentifier
    {
    public:
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr InvokableBox() noex { }

        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        InvokableBox(NullType null) noex :
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
        bool operator == (NullType null) const noex
        {
            return ObjectBox::operator == (null);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
            requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        InvokableBox(TInvokable&& invokable) :
            ObjectBox(fwd(invokable))
        {
            m_SetInvoker<TInvokable>();
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
            requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        InvokableBox& operator = (TInvokable&& invokable)
        {
            ObjectBox::operator = (fwd(invokable));
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
            ATOM_ASSERT(ObjectBox::_HasObject()) << NullPointerException(
                TEXT("InvokableTarget is null."));

            return m_invoker.Invoke(ObjectBox::_GetObject(), fwd(args)...);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        TResult operator () (TArgs&&... args)
        {
            return Invoke(fwd(args)...);
        }

    public:
        template <tname T>
        T* GetInvokable() noex
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        const TypeInfo& GetInvokableType() const noex
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
            return m_invoker.Invoke(fwd(args)...);
        }

    protected:
        using TInvoker = Private::Invoker<TResult, TArgs...>;

        TInvoker m_invoker;
    };
}