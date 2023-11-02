#pragma once
#include "Atom/Core.h"
#include "Atom/Core/Storage.h"
#include "Atom/Exceptions.h"
#include "Atom/Invokable/InvokablePtr.h"

namespace Atom
{
    template <typename TVal_, bool copy_, bool move_, bool allowNonMove, usize bufSize, typename TAlloc_>
    class _BoxImpl
    {
    public:
        using TVal = TVal_;
        using TAlloc = TAlloc_;

        class CopyTag
        {};

        class MoveTag
        {};

    public:
        constexpr _BoxImpl():
            _val(), _heapMem(nullptr), _heapMemSize(0), _alloc()
        {}

        template <typename TBox>
        constexpr _BoxImpl(CopyTag, const TBox& box)
        {}

        template <typename TBox>
        constexpr _BoxImpl(MoveTag, TBox& box)
        {}

        template <typename TVal>
        constexpr _BoxImpl(TVal&& val)
        {}

        constexpr ~_BoxImpl() {}

    public:
        static consteval auto IsCopyable() -> bool
        {
            return copy_;
        }

        static consteval auto IsMovable() -> bool
        {
            return move_;
        }

        static consteval auto AllowNonMovable() -> bool
        {
            return allowNonMove;
        }

        static consteval auto BufSize() -> usize
        {
            return bufSize;
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies `that` [`Box`] into `this` [`Box`].
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto copyBox(TBox& that)
        {
            _copyVal(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves `that` [`Box`] into `this` [`Box`].
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto moveBox(TBox&& that)
        {
            // When allocator type is different, we cannot handle heap memory. So we only move the
            // object and not the memory.
            if constexpr (not RSameAs<TAlloc, typename TTI::TRemoveCVRef<TBox>::TAlloc>)
            {
                if (that._hasVal())
                {
                    if (_hasVal())
                    {
                        _destroyVal();
                    }

                    _moveVal(that);
                }
                else
                {
                    if (_hasVal())
                    {
                        _destroyVal();
                        _resetValData();
                    }
                }

                return;
            }

            if (that._hasVal())
            {
                if (_hasVal())
                {
                    _destroyVal();
                }

                if (that._isValOnBuf())
                {
                    // If that has enough memory, we prefer that's memory as 
                    // we are moving and user expects the memory to be moved too.
                    if (that._heapMemSize >= that._val.size)
                    {
                        _checkAndReleaseMem();

                        _alloc = mov(that._alloc);
                        _heapMem = that._heapMem;
                        _heapMemSize = that._heapMemSize;

                        that._heapMem = 0;
                        that._heapMemSize = 0;
                    }

                    if (_heapMemSize >= that._val.size)
                    {
                        _moveValData(that);
                        _val.move(_heapMem, _val.val);
                        _val.val = _heapMem;
                    }
                    else
                    {
                        _moveVal(that);
                    }
                }
                else
                {
                    _checkAndReleaseMem();

                    _alloc = mov(that._alloc);
                    _heapMem = that._heapMem;
                    _heapMemSize = that._heapMemSize;

                    that._heapMem = 0;
                    that._heapMemSize = 0;

                    _moveValData(that);
                }
            }
            else
            {
                if (_hasVal())
                {
                    _destroyVal();
                    _resetValData();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto emplaceVal(TArgs&&... args, bool forceHeap = false)
        {
            destroyVal();
            _emplaceVal<T>(fwd(args)..., forceHeap);
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys previous object if any and stores new object.
        ///
        /// @TPARAM T Type of object to store.
        ///
        /// @PARAM[IN] val Valect to store.
        /// @PARAM[IN] forceHeap (default = false) Force store on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto setVal(T&& val, bool forceHeap = false)
        {
            emplaceVal<TTI::TRemoveCVRef<T>>(fwd(val));
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// # Template Parameters
        ///
        /// - `T`: Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getVal() -> const T&
        {
            debug_expects(getMemAs<T>() != nullptr);

            return *getMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// # Template Parameters
        ///
        /// - `T`: Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutVal() -> T&
        {
            debug_expects(getMutMemAs<T>() != nullptr);

            return *getMutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMem() const -> const void*
        {
            return _val.val;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMutMem() -> void*
        {
            return _val.val;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMemAs() const -> const T*
        {
            return static_cast<T*>(_val.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutMemAs() -> T*
        {
            return static_cast<T*>(_val.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `TypeInfo` of stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getValType() const -> const TypeInfo&
        {
            return *_val.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// Size of the object stored.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getValSize() const -> usize
        {
            return _val.size;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if object is not `null`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto hasVal() const -> bool
        {
            return _hasVal();
        }

        /// ----------------------------------------------------------------------------------------
        /// Disposes current object by calling its destructor.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroyVal()
        {
            if (_hasVal())
            {
                _destroyVal();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isValOnBuf() const -> bool
        {
            return _isValOnBuf();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Creates new object of type `T`. Doesn't handles current object.
        ///
        /// # Template Parameters
        ///
        /// - `T`: Type of the object to create.
        /// - `TArgs`: Type of args for object's constructor.
        ///
        /// # Parameters
        ///
        /// - `args...`: Args for object's constructor.
        /// - `forceHeap`: If true, allocates object on heap even if buffer was sufficient, else
        ///    chooses the best fit.
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto _emplaceVal(TArgs&&... args, bool forceHeap = false)
        {
            _val.size = sizeof(T);
            _val.type = &typeid(T);

            // TODO: Check if we can do static_cast instead to preserve constexpr.
            _val.dtor = [](void* val) { reinterpret_cast<T*>(val)->T::~T(); };

            if constexpr (IsCopyable())
            {
                _val.copy = [](void* val, const void* that) {
                    new (val) T(*reinterpret_cast<const T*>(that));
                };
            }

            if constexpr (IsMovable())
            {
                if constexpr (RMoveConstructible<T>)
                {
                    _val.move = [](void* val, void* that) {
                        new (val) T(mov(*reinterpret_cast<T*>(that)));
                    };
                }
                else
                {
                    _val.move = nullptr;
                }
            }

            // If the object is not movable but allowNonMove is allowed, we allocate it on heap to
            // avoid object's move constructor.
            if constexpr (IsMovable() and AllowNonMovable() and not RMoveConstructible<T>)
            {
                forceHeap = true;
            }

            _val.val = _allocMem(_val.size, forceHeap);
            new (_val.val) T(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies the object from `that` [`Box`] into `this` [`Box`].
        ///
        /// @PARAM[IN] that [`Box`] of which to copy object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _copyVal(const TBox& that, bool forceHeap = false)
        {
            _copyValData(that);

            if constexpr (IsMovable() and AllowNonMovable())
            {
                forceHeap = forceHeap || _val.move == nullptr;
            }

            _val.val = _allocMem(_val.size, forceHeap);
            _val.copy(_val.val, that.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves the object from `that` [`Box`] into `this` [`Box`].
        ///
        /// @PARAM[IN] that [`Box`] of which to move object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        ///
        /// @NOTE This doesn't moves the memory from `that` [`Box`].
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _moveVal(TBox&& that, bool forceHeap = false)
        {
            _moveValData(that);

            if constexpr (AllowNonMovable())
            {
                forceHeap = forceHeap || _val.move == nullptr;
            }

            _val.val = _allocMem(_val.size, forceHeap);
            _val.move(_val.val, that._val.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto _hasVal() const -> bool
        {
            return _val.val != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto _destroyVal()
        {
            _val.dtor(_val.val);
            _resetValData();
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {_ValData} from {thatBox}.
        ///
        /// @PARAM[IN] thatBox [`Box`] of which to copy {_ValData}.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _copyValData(const TBox& thatBox)
        {
            const auto& that = thatBox._val;

            _val.val = that.val;
            _val.size = that.size;
            _val.type = that.type;
            _val.dtor = that.dtor;

            if constexpr (IsCopyable())
            {
                if constexpr (IsMovable())
                {
                    _val.copy = that.copy;
                }
                else
                {
                    _val.copy = nullptr;
                }
            }

            if constexpr (IsMovable())
            {
                if constexpr (TBox::IsMovable())
                {
                    _val.move = that.move;
                }
                else
                {
                    _val.move = nullptr;
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _moveValData(TBox& thatBox)
        {
            auto& that = thatBox._val;

            _val.val = that.val;
            _val.type = that.type;
            _val.dtor = that.dtor;
            _val.size = that.size;

            that.val = nullptr;
            that.type = nullptr;
            that.dtor = nullptr;
            that.size = 0;

            if constexpr (IsCopyable())
            {
                if constexpr (TBox::IsCopyable())
                {
                    _val.copy = that.copy;
                    that.copy = nullptr;
                }
                else
                {
                    _val.copy = nullptr;
                }
            }

            if constexpr (IsMovable())
            {
                if constexpr (TBox::IsMovable())
                {
                    _val.move = that.move;
                    that.move = nullptr;
                }
                else
                {
                    _val.move = nullptr;
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto _resetValData()
        {
            _val = _ValData();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _isValOnBuf() const -> bool
        {
            if constexpr (BufSize() == 0)
                return false;

            return _val.val == _buf.mem();
        }

        /// ----------------------------------------------------------------------------------------
        /// Allocates enough memory of size `size`. Uses stack memory if it is big enough.
        ///
        /// @PARAM[IN] size Size of memory to allocate.
        /// @PARAM[IN] forceHeap If `true`, allocates memory from `TAlloc`.
        ///
        /// @RETURNS Pointer to the allocated memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _allocMem(usize size, bool forceHeap = false) -> void*
        {
            if constexpr (bufSize > 0)
            {
                // Check if stack memory is big enough.
                if (not forceHeap and size <= bufSize)
                {
                    return _buf.mutMem();
                }
            }

            // If we have previously allocated memory.
            if (_heapMem != nullptr)
            {
                if (_heapMemSize < size)
                {
                    _heapMem = _alloc.Realloc(_heapMem, size);
                    _heapMemSize = size;
                }
            }
            // We need to allocate heap memory.
            else
            {
                _heapMem = _alloc.Alloc(size);
                _heapMemSize = size;
            }

            return _heapMem;
        }

        /// ----------------------------------------------------------------------------------------
        /// If heap memory is allocated, deallocates it.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _checkAndReleaseMem()
        {
            if (_heapMem != nullptr)
            {
                _releaseMem();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Deallocates allocated heap memory.
        /// 
        /// # Note
        /// 
        /// Doesn't check if memory is even allocated.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _releaseMem()
        {
            _alloc.Dealloc(_heapMem);
            _heapMem = nullptr;
            _heapMemSize = 0;
        }

    private:
        class _ValData
        {
        public:
            TVal* val;
            usize size;
            const TypeInfo* type;
            InvokablePtr<void(void*)> dtor;

            ATOM_CONDITIONAL_FIELD(IsCopyable(), InvokablePtr<void(void*, const void*)>) copy;
            ATOM_CONDITIONAL_FIELD(IsMovable(), InvokablePtr<void(void*, void*)>) move;
        };

    private:
        TAlloc _alloc;
        void* _heapMem;
        usize _heapMemSize;
        StaticStorage<bufSize> _buf;
        _ValData _val;
    };
}
