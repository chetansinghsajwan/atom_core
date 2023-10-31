#pragma once
#include "Atom/Core.h"
#include "Atom/Core/Storage.h"
#include "Atom/Exceptions.h"
#include "Atom/Invokable/InvokablePtr.h"

namespace Atom
{
    /// ----------------------------------------------------------------------------------------
    /// [`_BoxId`] is used as an identifier for [`Box`] where a template cannot be used.
    /// ----------------------------------------------------------------------------------------
    class _BoxId
    {};

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <bool copy, bool move>
    class _BoxObjData;

    template <>
    class _BoxObjData<false, false>
    {
    public:
        InvokablePtr<void(void* obj)> dtor;
        const TypeInfo* type;
        void* obj;
        usize size;
    };

    template <>
    class _BoxObjData<true, false>: public _BoxObjData<false, false>
    {
    public:
        InvokablePtr<void(void*, const void*)> copy;
    };

    template <>
    class _BoxObjData<false, true>: public _BoxObjData<false, false>
    {
    public:
        InvokablePtr<void(void*, void*)> move;
    };

    template <>
    class _BoxObjData<true, true>: public _BoxObjData<false, false>
    {
    public:
        InvokablePtr<void(void*, const void*)> copy;
        InvokablePtr<void(void*, void*)> move;
    };

    template <bool copy, bool move, bool allowNonMove, usize bufSize, typename TAlloc_>
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
            _obj(), _mem(nullptr), _memSize(0), _alloc()
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
        static constexpr auto IsCopyable() -> bool
        {
            return copy;
        }

        static constexpr auto IsMovable() -> bool
        {
            return move;
        }

        static constexpr auto AllowNonMovable() -> bool
        {
            return allowNonMove;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Box Manipulation Functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// Copies `that` `Box` into `this` `Box`.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto copyBox(TBox& that)
        {
            copyObj(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves `that` `Box` into `this` `Box`.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto moveBox(TBox&& that)
        {
            // When allocator type is different, we cannot handle heap memory.
            // So we only move the object.
            if constexpr (not RSameAs<TAlloc, typename TTI::TRemoveCVRef<TBox>::TAlloc>)
            {
                moveObj(that);
                that.destroyBox();
                return;
            }

            destroyObj();

            const usize thatObjSize = that._obj.size;
            const bool thatIsUsingStackMem = that.isUsingStackMem();
            if (thatIsUsingStackMem && thatObjSize > bufSize && _memSize >= thatObjSize
                && that._memSize < thatObjSize)
            {
                // We cannot deallocate our memory in the above scenario.
                that._releaseMem();
            }
            else
            {
                _releaseMem();

                _mem = mov(that._mem);
                _memSize = mov(that._memSize);
                _alloc = mov(that._alloc);
            }

            if (thatIsUsingStackMem)
            {
                moveObj(mov(that));
            }
            else
            {
                _copyObjData(that);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroy stored object and releases any allocated memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroyBox()
        {
            destroyObj();
            _releaseMem();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Object Manipulation Functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto initObj(T&& obj, bool forceHeap = false)
        {
            _obj.size = sizeof(T);
            _obj.type = &typeid(T);

            // TODO: Check if we can do static_cast instead to preserve constexpr.
            _obj.dtor = [](void* obj) { reinterpret_cast<T*>(obj)->T::~T(); };

            if constexpr (copy)
            {
                _obj.copy = [](void* obj, const void* that) {
                    new (obj) T(*reinterpret_cast<const T*>(that));
                };
            }

            if constexpr (move)
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

            // If the object is not movable but allowNonMove is allowed,
            // we allocate it on heap to avoid object's move constructor.
            if constexpr (move && allowNonMove && !RMoveConstructible<T>)
            {
                forceHeap = true;
            }

            _obj.obj = _allocMem(_obj.size, forceHeap);
            new (_obj.obj) T(fwd(obj));
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
            destroyObj();
            initObj(fwd(obj), forceHeap);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getObj() -> const T&
        {
            return *reinterpret_cast<const T*>(_obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutObj() -> T&
        {
            return *reinterpret_cast<T*>(_obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMemAs() const -> const T*
        {
            return reinterpret_cast<T*>(_obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto getMutMemAs() -> T*
        {
            return reinterpret_cast<T*>(_obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get {TypeInfo} or stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getObjType() const -> const TypeInfo&
        {
            return *_obj.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if object is not {null}.
        /// ----------------------------------------------------------------------------------------
        constexpr auto hasObj() const -> bool
        {
            return _obj.obj != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies the object from `that` `Box` into `this` `Box`.
        ///
        /// @PARAM[IN] that `Box` of which to copy object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename TBoxObjData>
        constexpr auto copyObj(const TBoxObjData& that, bool forceHeap = false)
        {
            destroyObj();

            _copyObjData(that);

            if constexpr (move)
            {
                forceHeap = forceHeap || _obj.move == nullptr;
            }
            else
            {
                forceHeap = true;
            }

            _obj.obj = _allocMem(_obj.size, forceHeap);
            _obj.copy(_obj.obj, that.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves the object from `that` `Box` into `this` `Box`.
        ///
        /// @PARAM[IN] that `Box` of which to move object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        ///
        /// @NOTE This doesn't moves the memory from `that` `Box`.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto moveObj(TBox&& that, bool forceHeap = false)
            requires(move)
        {
            destroyObj();

            _copyObjData(that);
            forceHeap = forceHeap || _obj.move == nullptr;

            _obj.obj = _allocMem(_obj.size, forceHeap);
            _obj.move(_obj.obj, that._obj.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Disposes current object by calling its destructor.
        ///
        /// @NOTE This does'n deallocates memory.
        ///
        /// @SEE _releaseMem().
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroyObj()
        {
            if (_obj.obj != nullptr)
            {
                _obj.dtor(_obj.obj);
                _obj = {};
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isUsingStackMem() const -> bool
        {
            return _obj.obj == _buf.mem();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Copies {_ObjData} from {thatBox}.
        ///
        /// @PARAM[IN] thatBox `Box` of which to copy {_ObjData}.
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _copyObjData(const TBox& thatBox)
        {
            const auto& that = thatBox._obj;

            _obj.obj = that.obj;
            _obj.size = that.size;
            _obj.type = that.type;
            _obj.dtor = that.dtor;

            if constexpr (copy)
            {
                if constexpr (move)
                {
                    _obj.copy = that.copy;
                }
                else
                {
                    _obj.copy = nullptr;
                }
            }

            if constexpr (move)
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
                if (!forceHeap && size <= bufSize)
                {
                    return reinterpret_cast<void*>(_buf.mutMem());
                }
            }

            // If we have previously allocated memory.
            if (_mem != nullptr)
            {
                if (_memSize < size)
                {
                    _mem = _alloc.Realloc(_mem, size);
                    _memSize = size;
                }
            }
            // We need to allocate heap memory.
            else
            {
                _mem = _alloc.Alloc(size);
                _memSize = size;
            }

            return _mem;
        }

        /// ----------------------------------------------------------------------------------------
        /// Deallocates any allocated memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _releaseMem()
        {
            if (_mem != nullptr)
            {
                _alloc.Dealloc(_mem);
                _mem = nullptr;
                _memSize = 0;
            }
        }

    protected:
        TAlloc _alloc;
        void* _mem;
        usize _memSize;
        _BoxObjData<copy, move> _obj;
        StaticStorage<bufSize> _buf;
    };
}
