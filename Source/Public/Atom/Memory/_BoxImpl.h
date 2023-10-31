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
            return copy_;
        }

        static constexpr auto IsMovable() -> bool
        {
            return move_;
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
            // When allocator type is different, we cannot handle heap memory.
            // So we only move the object.
            if constexpr (not RSameAs<TAlloc, typename TTI::TRemoveCVRef<TBox>::TAlloc>)
            {
                _moveObj(that);
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
                _moveObj(mov(that));
            }
            else
            {
                _copyObjData(that);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Object Manipulation Functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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
            emplaceObj<T>(fwd(obj));
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
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto _emplaceObj(T&& obj, bool forceHeap = false)
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

            // If the object is not movable but allowNonMove is allowed,
            // we allocate it on heap to avoid object's move constructor.
            if constexpr (IsCopyable() && AllowNonMovable() && !RMoveConstructible<T>)
            {
                forceHeap = true;
            }

            _obj.obj = _allocMem(_obj.size, forceHeap);
            new (_obj.obj) T(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies the object from `that` [`Box`] into `this` [`Box`].
        ///
        /// @PARAM[IN] that [`Box`] of which to copy object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename TBoxObjData>
        constexpr auto _copyObj(const TBoxObjData& that, bool forceHeap = false)
        {
            destroyObj();

            _copyObjData(that);

            if constexpr (IsMovable())
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
        /// Moves the object from `that` [`Box`] into `this` [`Box`].
        ///
        /// @PARAM[IN] that [`Box`] of which to move object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        ///
        /// @NOTE This doesn't moves the memory from `that` [`Box`].
        /// ----------------------------------------------------------------------------------------
        template <typename TBox>
        constexpr auto _moveObj(TBox&& that, bool forceHeap = false)
            requires(IsMovable())
        {
            destroyObj();

            _copyObjData(that);
            forceHeap = forceHeap || _obj.move == nullptr;

            _obj.obj = _allocMem(_obj.size, forceHeap);
            _obj.move(_obj.obj, that._obj.obj);
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
        void* _mem;
        usize _memSize;
        _ObjData _obj;
        StaticStorage<bufSize> _buf;
    };
}
