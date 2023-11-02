#pragma once
#include "Atom/Core.h"
#include "Atom/Core/Storage.h"
#include "Atom/Exceptions.h"
#include "Atom/Invokable/InvokablePtr.h"

namespace Atom
{
    template <bool copy_, bool move_, bool allowNonMove, usize bufSize, typename TAlloc_>
    class _BoxImpl
    {
    public:
        using TAlloc = TAlloc_;

        class CopyTag
        {};

        class MoveTag
        {};

    public:
        constexpr _BoxImpl():
            _obj(), _heapMem(nullptr), _heapMemSize(0), _alloc()
        {}

        template <typename TBox>
        constexpr _BoxImpl(CopyTag, const TBox& box)
        {}

        template <typename TBox>
        constexpr _BoxImpl(MoveTag, TBox& box)
        {}

        template <typename TObj>
        constexpr _BoxImpl(TObj&& obj)
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
            _copyObj(that);
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
                if (that._hasObj())
                {
                    if (_hasObj())
                    {
                        _destroyObj();
                    }

                    _moveObj(that);
                }
                else
                {
                    if (_hasObj())
                    {
                        _destroyObj();
                        _resetObjData();
                    }
                }

                return;
            }

            if (that._hasObj())
            {
                if (_hasObj())
                {
                    _destroyObj();
                }

                if (that._isObjOnBuf())
                {
                    // If that has enough memory, we prefer that's memory as 
                    // we are moving and user expects the memory to be moved too.
                    if (that._heapMemSize >= that._obj.size)
                    {
                        _checkAndReleaseMem();

                        _alloc = mov(that._alloc);
                        _heapMem = that._heapMem;
                        _heapMemSize = that._heapMemSize;

                        that._heapMem = 0;
                        that._heapMemSize = 0;
                    }

                    if (_heapMemSize >= that._obj.size)
                    {
                        _moveObjData(that);
                        _obj.move(_heapMem, _obj.obj);
                        _obj.obj = _heapMem;
                    }
                    else
                    {
                        _moveObj(that);
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

                    _moveObjData(that);
                }
            }
            else
            {
                if (_hasObj())
                {
                    _destroyObj();
                    _resetObjData();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto emplaceObj(TArgs&&... args, bool forceHeap = false)
        {
            destroyObj();
            _emplaceObj<T>(fwd(args)..., forceHeap);
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys previous object if any and stores new object.
        ///
        /// @TPARAM T Type of object to store.
        ///
        /// @PARAM[IN] obj Object to store.
        /// @PARAM[IN] forceHeap (default = false) Force store on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto setObj(T&& obj, bool forceHeap = false)
        {
            emplaceObj<TTI::TRemoveCVRef<T>>(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// # Template Parameters
        ///
        /// - `T`: Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getObj() -> const T&
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
        constexpr auto getMutObj() -> T&
        {
            debug_expects(getMutMemAs<T>() != nullptr);

            return *getMutMemAs<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMem() const -> const void*
        {
            return _obj.obj;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMutMem() -> void*
        {
            return _obj.obj;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMemAs() const -> const T*
        {
            return static_cast<T*>(_obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutMemAs() -> T*
        {
            return static_cast<T*>(_obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `TypeInfo` of stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getObjType() const -> const TypeInfo&
        {
            return *_obj.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// Size of the object stored.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getObjSize() const -> usize
        {
            return _obj.size;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if object is not `null`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto hasObj() const -> bool
        {
            return _hasObj();
        }

        /// ----------------------------------------------------------------------------------------
        /// Disposes current object by calling its destructor.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroyObj()
        {
            if (_hasObj())
            {
                _destroyObj();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isObjOnBuf() const -> bool
        {
            return _isObjOnBuf();
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
        constexpr auto _emplaceObj(TArgs&&... args, bool forceHeap = false)
        {
            _obj.size = sizeof(T);
            _obj.type = &typeid(T);

            // TODO: Check if we can do static_cast instead to preserve constexpr.
            _obj.dtor = [](void* obj) { reinterpret_cast<T*>(obj)->T::~T(); };

            if constexpr (IsCopyable())
            {
                _obj.copy = [](void* obj, const void* that) {
                    new (obj) T(*reinterpret_cast<const T*>(that));
                };
            }

            if constexpr (IsMovable())
            {
                if constexpr (RMoveConstructible<T>)
                {
                    _obj.move = [](void* obj, void* that) {
                        new (obj) T(mov(*reinterpret_cast<T*>(that)));
                    };
                }
                else
                {
                    _obj.move = nullptr;
                }
            }

            // If the object is not movable but allowNonMove is allowed, we allocate it on heap to
            // avoid object's move constructor.
            if constexpr (IsMovable() and AllowNonMovable() and not RMoveConstructible<T>)
            {
                forceHeap = true;
            }

            _obj.obj = _allocMem(_obj.size, forceHeap);
            new (_obj.obj) T(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies the object from `that` [`Box`] into `this` [`Box`].
        ///
        /// @PARAM[IN] that [`Box`] of which to copy object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _copyObj(const TBox& that, bool forceHeap = false)
        {
            _copyObjData(that);

            if constexpr (IsMovable() and AllowNonMovable())
            {
                forceHeap = forceHeap || _obj.move == nullptr;
            }

            _obj.obj = _allocMem(_obj.size, forceHeap);
            _obj.copy(_obj.obj, that.obj);
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
        constexpr auto _moveObj(TBox&& that, bool forceHeap = false)
        {
            _moveObjData(that);

            if constexpr (AllowNonMovable())
            {
                forceHeap = forceHeap || _obj.move == nullptr;
            }

            _obj.obj = _allocMem(_obj.size, forceHeap);
            _obj.move(_obj.obj, that._obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto _hasObj() const -> bool
        {
            return _obj.obj != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto _destroyObj()
        {
            _obj.dtor(_obj.obj);
            _resetObjData();
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {_ObjData} from {thatBox}.
        ///
        /// @PARAM[IN] thatBox [`Box`] of which to copy {_ObjData}.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _copyObjData(const TBox& thatBox)
        {
            const auto& that = thatBox._obj;

            _obj.obj = that.obj;
            _obj.size = that.size;
            _obj.type = that.type;
            _obj.dtor = that.dtor;

            if constexpr (IsCopyable())
            {
                if constexpr (IsMovable())
                {
                    _obj.copy = that.copy;
                }
                else
                {
                    _obj.copy = nullptr;
                }
            }

            if constexpr (IsMovable())
            {
                if constexpr (TBox::IsMovable())
                {
                    _obj.move = that.move;
                }
                else
                {
                    _obj.move = nullptr;
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _moveObjData(TBox& thatBox)
        {
            auto& that = thatBox._obj;

            _obj.obj = that.obj;
            _obj.type = that.type;
            _obj.dtor = that.dtor;
            _obj.size = that.size;

            that.obj = nullptr;
            that.type = nullptr;
            that.dtor = nullptr;
            that.size = 0;

            if constexpr (IsCopyable())
            {
                if constexpr (TBox::IsCopyable())
                {
                    _obj.copy = that.copy;
                    that.copy = nullptr;
                }
                else
                {
                    _obj.copy = nullptr;
                }
            }

            if constexpr (IsMovable())
            {
                if constexpr (TBox::IsMovable())
                {
                    _obj.move = that.move;
                    that.move = nullptr;
                }
                else
                {
                    _obj.move = nullptr;
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto _resetObjData()
        {
            _obj = _ObjData();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _isObjOnBuf() const -> bool
        {
            if constexpr (BufSize() == 0)
                return false;

            return _obj.obj == _buf.mem();
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
        class _ObjData
        {
        public:
            InvokablePtr<void(void* obj)> dtor;
            const TypeInfo* type;
            void* obj;
            usize size;

            ATOM_CONDITIONAL_FIELD(IsCopyable(), InvokablePtr<void(void*, const void*)>) copy;
            ATOM_CONDITIONAL_FIELD(IsMovable(), InvokablePtr<void(void*, void*)>) move;
        };

    private:
        TAlloc _alloc;
        void* _heapMem;
        usize _heapMemSize;
        StaticStorage<bufSize> _buf;
        _ObjData _obj;

        // void* obj;
        // usize objSize;
        // const TypeInfo* objType;
        // InvokablePtr<void(void* obj)> objDtor;
        // ATOM_CONDITIONAL_FIELD(IsCopyable(), InvokablePtr<void(void*, const void*)>) objCopy;
        // ATOM_CONDITIONAL_FIELD(IsMovable(), InvokablePtr<void(void*, void*)>) objMove;
    };
}
