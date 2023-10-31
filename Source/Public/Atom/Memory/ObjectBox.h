#pragma once
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/TTI.h"
#include "_BoxImpl.h"

namespace Atom
{
    // template <typename _TImpl>
    // class BoxFunctions
    // {
    //     using T = typename _TImpl::T;

    // public:
    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     template <typename T>
    //     constexpr auto set(T&& obj) -> T&
    //         requires(IsObjAssignable<T>())
    //     {
    //         _impl._set(fwd(obj));
    //         return _impl._get();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     template <typename T, typename... TArgs>
    //     constexpr auto emplace(TArgs&&... args) -> T&
    //         requires(IsObjAssignable<T>())
    //     {
    //         return *this;
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     constexpr auto destroy()
    //     {
    //         _impl.destroy();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     template <typename T>
    //     constexpr auto getAs() const -> const T&
    //     {
    //         debug_expects(not isNull(), "Obj is null.");

    //         return _impl.getAs<T>();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     template <typename T>
    //     constexpr auto getAsMut() -> T&
    //     {
    //         debug_expects(not isNull(), "Obj is null.");

    //         return _impl.getAsMut<T>();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     constexpr auto mem() const -> const memptr
    //     {
    //         debug_expects(not isNull(), "Obj is null.");

    //         return _impl.mem();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     constexpr auto mutMem() -> memptr
    //     {
    //         debug_expects(not isNull(), "Obj is null.");

    //         return _impl.mutMem();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     template <typename T>
    //     constexpr auto memAs() const -> const Ptr<T>
    //     {
    //         debug_expects(not isNull(), "Obj is null.");

    //         return _impl.memAs<T>();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     template <typename T>
    //     constexpr auto mutMemAs() -> Ptr<T>
    //     {
    //         debug_expects(not isNull(), "Obj is null.");

    //         return _impl.mutMemAs<T>();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     constexpr auto size() -> usize
    //     {
    //         return _impl.size();
    //     }

    //     /// ----------------------------------------------------------------------------------------
    //     ///
    //     /// ----------------------------------------------------------------------------------------
    //     constexpr auto isNull() const -> bool
    //     {
    //         return _impl.has();
    //     }


    // private:
    //     _TImpl _impl;
    // };

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

    template <typename T, usize bufSize, typename TAlloc>
    class Box
    {
        using This = Box<T, bufSize, TAlloc>;
        using _TImpl = _BoxImpl<false, false, false, bufSize, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Box():
            _impl()
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
            _impl{ typename _TImpl::CopyTag(), that._impl }
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
            _impl{ typename _TImpl::CopyTag(), that._impl }
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
        /// Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr Box(MoveBox<TThat, true, thatBufSize, TThatAlloc>&& that):
            _impl{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr This& operator=(MoveBox<TThat, true, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr Box(CopyMoveBox<TThat, true, thatBufSize, TThatAlloc>&& that):
            _impl{ typename _TImpl::MoveTag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// Template Move Operator
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
        template <typename T1>
        constexpr Box(T1&& obj):
            _impl{ fwd(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr This& operator=(T1&& obj)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Box()
        {
            _impl.destroyBox();
        }

    private:
        _TImpl _impl;
    };

    template <typename T, usize bufSize, typename TAlloc>
    class CopyBox
    {
        using This = CopyBox<T, bufSize, TAlloc>;
        using _TImpl = _BoxImpl<true, false, false, bufSize, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyBox():
            _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyBox(const This& that):
            _impl(typename _TImpl::CopyTag(), that._impl)
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
            _impl(typename _TImpl::CopyTag(), that._impl)
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
        template <typename T1>
        constexpr CopyBox(T1&& obj):
            _impl(fwd(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr This& operator=(T1&& obj)
            requires(RCopyable<T1>)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~CopyBox() {}

    protected:
        _TImpl _impl;
    };

    template <typename T, bool allowNonMove, usize bufSize, typename TAlloc>
    class MoveBox
    {
        using This = MoveBox<T, allowNonMove, bufSize, TAlloc>;
        using _TImpl = _BoxImpl<false, true, allowNonMove, bufSize, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox():
            _impl()
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
            _impl(typename _TImpl::CopyTag(), that._impl)
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
            _impl(typename _TImpl::CopyTag(), that._impl)
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
            _impl(typename _TImpl::MoveTag(), that._impl)
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
            _impl(typename _TImpl::MoveTag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat, usize thatBufSize, typename TThatAlloc>
        constexpr MoveBox& operator=(CopyMoveBox<TThat, allowNonMove, thatBufSize, TThatAlloc>&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr MoveBox(T1&& obj): _impl(fwd(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr MoveBox& operator=(T1&& obj)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~MoveBox()
        {
            _impl.destroyBox();
        }

    protected:
        _TImpl _impl;
    };

    template <typename T, bool allowNonMove, usize bufSize, typename TAlloc>
    class CopyMoveBox
    {
        using This = CopyMoveBox<T, allowNonMove, bufSize, TAlloc>;
        using _TImpl = _BoxImpl<true, true, allowNonMove, bufSize, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox():
            _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CopyMoveBox(const This& that):
            _impl(typename _TImpl::CopyTag(), that._impl)
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
            _impl(typename _TImpl::CopyTag(), that._impl)
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
            _impl(typename _TImpl::CopyTag(), that._impl)
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
            _impl(typename _TImpl::MoveTag(), that._impl)
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
            _impl(typename _TImpl::MoveTag(), that._impl)
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
            _impl(typename _TImpl::MoveTag(), that._impl)
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
        template <typename T1>
        constexpr CopyMoveBox(T1&& obj):
            _impl(fwd(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Operator
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr CopyMoveBox& operator=(T1&& obj)
        {
            _impl.setObj(fwd(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~CopyMoveBox()
        {
            _impl.destroyBox();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto has() const -> bool
        {
            return _impl.hasObj();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutMem() -> T*
        {
            return _impl.template getMutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr auto mutMemAs() -> T1*
        {
            return _impl.template getMutMemAs<T1>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto type() const -> const TypeInfo&
        {
            return _impl.getObjType();
        }

    protected:
        _TImpl _impl;
    };
}
