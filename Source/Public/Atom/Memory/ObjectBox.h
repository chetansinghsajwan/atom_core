#pragma once
#include "Atom/Core/Requirements.h"
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/TTI.h"
#include "_BoxImpl.h"

namespace Atom
{
    template <typename T>
    using Ptr = T*;

    using memptr = void*;
}

namespace Atom
{
    template <typename _TImpl_>
    class BoxFunctions
    {
    protected:
        using _TImpl = _TImpl_;

    public:
        using TVal = typename _TImpl::TVal;

    public:
        constexpr BoxFunctions(auto&&... args):
            _impl{ fwd(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto emplace(TArgs&&... args) -> T&
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            _impl.template emplaceObj<T>(fwd(args)...);
            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto set(T&& obj) -> T&
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            _impl._setObj(fwd(obj));
            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() const -> const TVal&
        {
            debug_expects(hasVal(), "Value is null.");

            return _impl.getObj();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMut() -> TVal&
        {
            debug_expects(hasVal(), "Value is null.");

            return _impl.getObjMut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkGet() const -> const TVal&
        {
            expects(hasVal(), "Value is null.");

            return _impl.getObj();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkGetMut() -> TVal&
        {
            expects(hasVal(), "Value is null.");

            return _impl.getObjMut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getAs() const -> const T&
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            debug_expects(hasVal(), "Value is null.");

            return _impl.template getObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutAs() -> T&
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            debug_expects(hasVal(), "Value is null.");

            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkGetAs() const -> const T&
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            expects(hasVal(), "Value is null.");

            return _impl.template getObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkGetMutAs() -> T&
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            expects(hasVal(), "Value is null.");

            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const Ptr<TVal>
        {
            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutMem() -> Ptr<TVal>
        {
            return _impl.mutMem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkMem() const -> const Ptr<TVal>
        {
            expects(hasVal(), "Value is null.");

            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkMutMem() -> Ptr<TVal>
        {
            expects(hasVal(), "Value is null.");

            return _impl.mutMem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto memAs() const -> const Ptr<T>
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            return _impl.template memAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto mutMemAs() -> Ptr<T>
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            return _impl.template mutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkMemAs() const -> const Ptr<T>
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            expects(hasVal(), "Value is null.");

            return _impl.template memAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkMutMemAs() -> Ptr<T>
            requires RSameOrDerivedFrom<TPure<T>, TVal>
        {
            expects(hasVal(), "Value is null.");

            return _impl.template mutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto valType() const -> const TypeInfo&
        {
            return _impl.objType();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto valSize() const -> usize
        {
            return _impl.objSize();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto hasVal() const -> bool
        {
            return _impl.hasObj();
        }

    protected:
        _TImpl _impl;
    };

    template <typename _TImpl_>
        requires(RIsVoid<typename _TImpl_::TVal>)
    class BoxFunctions<_TImpl_>
    {
    protected:
        using _TImpl = _TImpl_;

    public:
        using TVal = void;

    public:
        constexpr BoxFunctions(auto&&... args):
            _impl{ fwd(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto emplace(TArgs&&... args) -> T&
            requires(not RIsVoid<T>)
        {
            _impl.template emplaceObj<T>(fwd(args)...);
            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto set(T&& obj) -> T&
        {
            _impl._setObj(fwd(obj));
            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getAs() const -> const T&
        {
            debug_expects(hasVal(), "Value is null.");

            return _impl.template getObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutAs() -> T&
        {
            debug_expects(hasVal(), "Value is null.");

            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkGetAs() const -> const T&
        {
            expects(hasVal(), "Value is null.");

            return _impl.template getObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkGetMutAs() -> T&
        {
            expects(hasVal(), "Value is null.");

            return _impl.template getMutObjAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const memptr
        {
            return _impl.getMem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutMem() -> memptr
        {
            return _impl.getMutMem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkMem() const -> const memptr
        {
            expects(hasVal(), "Value is null.");

            return _impl.getMem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkMutMem() -> memptr
        {
            expects(hasVal(), "Value is null.");

            return _impl.getMutMem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto memAs() const -> const Ptr<T>
            requires(not RIsVoid<T>)
        {
            return _impl.template memAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto mutMemAs() -> Ptr<T>
            requires(not RIsVoid<T>)
        {
            return _impl.template mutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkMemAs() const -> const memptr
            requires(not RIsVoid<T>)
        {
            expects(hasVal(), "Value is null.");

            return _impl.template getMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto checkMutMemAs() -> memptr
            requires(not RIsVoid<T>)
        {
            expects(hasVal(), "Value is null.");

            return _impl.template getMutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto valType() const -> const TypeInfo&
        {
            return _impl.getObjType();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto valSize() const -> usize
        {
            return _impl.objSize();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto hasVal() const -> bool
        {
            return _impl.hasObj();
        }

    protected:
        _TImpl _impl;
    };

    template <typename T, usize bufSize = 50, typename TAlloc = DefaultMemAllocator>
    class Box;

    template <typename T, usize bufSize = 50, typename TAlloc = DefaultMemAllocator>
    class CopyBox;

    template <typename T, bool allowNonMove = true, usize bufSize = 50,
        typename TAlloc = DefaultMemAllocator>
    class MoveBox;

    template <typename T, bool allowNonMove = true, usize bufSize = 50,
        typename TAlloc = DefaultMemAllocator>
    class CopyMoveBox;

    template <typename TVal, usize bufSize, typename TAlloc>
    class Box: public BoxFunctions<_BoxImpl<TVal, false, false, false, bufSize, TAlloc>>
    {
        using This = Box<TVal, bufSize, TAlloc>;
        using Base = BoxFunctions<_BoxImpl<TVal, false, false, false, bufSize, TAlloc>>;
        using _TImpl = typename Base::_TImpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Box():
            Base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Box(const This& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr This& operator=(const This& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Box(This&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr This& operator=(This&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr Box(const CopyBox<TThat, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr This& operator=(const CopyBox<TThat, thatBufSize, TThatAlloc>& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr Box(const CopyMoveBox<TThat, true, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr This& operator=(const CopyMoveBox<TThat, true, thatBufSize, TThatAlloc>& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr Box(MoveBox<TThat, true, thatBufSize, TThatAlloc>&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr This& operator=(MoveBox<TThat, true, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr Box(CopyMoveBox<TThat, true, thatBufSize, TThatAlloc>&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr This& operator=(CopyMoveBox<TThat, true, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr Box(T&& obj):
            Base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr This& operator=(T&& obj)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Box() {}

    private:
        using Base::_impl;
    };

    template <typename TVal, usize bufSize, typename TAlloc>
    class CopyBox: public BoxFunctions<_BoxImpl<TVal, true, false, false, bufSize, TAlloc>>
    {
        using This = CopyBox<TVal, bufSize, TAlloc>;
        using Base = BoxFunctions<_BoxImpl<TVal, true, false, false, bufSize, TAlloc>>;
        using _TImpl = typename Base::_TImpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyBox():
            Base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyBox(const This& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr This& operator=(const This& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyBox(This&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr This& operator=(This&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, bool allowNonMove, usize thatBufSize, typename TThatAlloc>
        constexpr CopyBox(const CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, bool allowNonMove, usize thatBufSize, typename TThatAlloc>
        constexpr This& operator=(
            const CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr CopyBox(T&& obj):
            Base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr This& operator=(T&& obj)
            requires(RCopyable<T>)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~CopyBox() {}

    private:
        using Base::_impl;
    };

    template <typename TVal, bool allowNonMove, usize bufSize, typename TAlloc>
    class MoveBox: public BoxFunctions<_BoxImpl<TVal, false, true, allowNonMove, bufSize, TAlloc>>
    {
        using This = MoveBox<TVal, allowNonMove, bufSize, TAlloc>;
        using Base = BoxFunctions<_BoxImpl<TVal, false, true, allowNonMove, bufSize, TAlloc>>;
        using _TImpl = typename Base::_TImpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox():
            Base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox(const This& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox& operator=(const This& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr MoveBox(const CopyBox<TThat, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr MoveBox& operator=(const CopyBox<TThat, thatBufSize, TThatAlloc>& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr MoveBox(const CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr MoveBox& operator=(
            const CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox(This&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox& operator=(This&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr MoveBox(CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr MoveBox& operator=(
            CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr MoveBox(T&& obj):
            Base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr MoveBox& operator=(T&& obj)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~MoveBox() {}

    private:
        using Base::_impl;
    };

    template <typename TVal, bool allowNonMove, usize bufSize, typename TAlloc>
    class CopyMoveBox:
        public BoxFunctions<_BoxImpl<TVal, true, true, allowNonMove, bufSize, TAlloc>>
    {
        using This = CopyMoveBox<TVal, allowNonMove, bufSize, TAlloc>;
        using Base = BoxFunctions<_BoxImpl<TVal, true, true, allowNonMove, bufSize, TAlloc>>;
        using _TImpl = typename Base::_TImpl;

    private:
        using Base::_impl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox():
            Base{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox(const This& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox& operator=(const This& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox(const CopyBox<TThat, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox& operator=(const CopyBox<TThat, thatBufSize, TThatAlloc>& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox(
            const CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>& that):
            Base{ typename _TImpl::CopyTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox& operator=(
            const CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>& that)
        {
            _impl.copyBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox(This&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox& operator=(This&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, bool thatAllowNonMove, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox(MoveBox<TThat, thatAllowNonMove, thatBufSize, TThatAlloc>&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, bool thatAllowNonMove, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox& operator=(
            MoveBox<TThat, thatAllowNonMove, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, bool thatAllowNonMove, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox(CopyMoveBox<TThat, thatAllowNonMove, thatBufSize, TThatAlloc>&& that):
            Base{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, bool thatAllowNonMove, usize thatBufSize, typename TThatAlloc>
            requires allowNonMove
        constexpr CopyMoveBox& operator=(
            CopyMoveBox<TThat, thatAllowNonMove, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr CopyMoveBox(T&& obj):
            Base{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr CopyMoveBox& operator=(T&& obj)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~CopyMoveBox() {}
    };
}
