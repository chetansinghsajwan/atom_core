#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/Invokable/InvokablePtr.h"
#include "Atom/TTI.h"

#include "DefaultMemAllocator.h"

namespace Atom
{
    namespace Internal
    {
        /// ----------------------------------------------------------------------------------------
        /// {ObjectBoxIdentifier} is used to check if the type is same as or derived from
        /// {ObjectBox} template.
        /// ----------------------------------------------------------------------------------------
        class ObjectBoxIdentifier
        {};
    }

    /// --------------------------------------------------------------------------------------------
    /// Stores object inside using type erasure.
    ///
    /// @TPARAM Copyable Should the {ObjectBox} be {CopyConstructible} and {CopyAssignable}.
    /// @TPARAM Movable Should the {ObjectBox} be {MoveConstructible} and {MoveAssignable}.
    /// @TPARAM AllowNonMovableObject Should the {Object} be {MoveConstructible} when {Movable} is
    /// true.
    ///     {Object} can be allocated on heap if it's not movable.
    /// @TPARAM StackSize Size of stack to store object, to avoid heap allocation.
    /// @TPARAM TMemAllocator {MemAllocator} to allocate memory to store object.
    /// --------------------------------------------------------------------------------------------
    template <bool Copyable, bool Movable, bool AllowNonMovableObject, usize StackSize,
        typename TMemAllocator = DefaultMemAllocator>
    class ObjectBox: public Internal::ObjectBoxIdentifier
    {
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, typename TOtherMemAllocator>
        friend class ObjectBox;

        /// --------------------------------------------------------------------------------------------
        /// Stores data for object. Like, {CopyConstructor}, {MoveConstructor} and {ObjectSize}.
        /// --------------------------------------------------------------------------------------------
        class ObjectData
        {
        public:
            ATOM_CONDITIONAL_FIELD(Copyable, InvokablePtr<void(void*, const void*)>) copy;

            ATOM_CONDITIONAL_FIELD(Movable, InvokablePtr<void(void*, void*)>) move;

            InvokablePtr<void(void* obj)> dtor;

            usize size;
            void* obj;
            const TypeInfo* type;
        };

        /// ----------------------------------------------------------------------------------------
        /// Requirements for {Object} accepted by this {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        static constexpr bool RObject = requires {
            // {ObjectBox} variants are not stored inside {ObjectBox} variants.
            requires RNotDerivedFrom<T, Internal::ObjectBoxIdentifier>;

            // If Box is {Copyable} then the object should also be {CopyConstructible}.
            requires !Copyable || RCopyConstructible<T>;

            // If Box is {Movable} then the object should also be {MoveConstructible} unless
            // {NonMovableObject} is allowed.
            requires !Movable || (RMoveConstructible<T> || AllowNonMovableObject);
        };

        /// ----------------------------------------------------------------------------------------
        /// Requirements for other {ObjectBox} accepted by this {ObjectBox}.
        /// For example, {CopyConstructor} and {MoveConstructor}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject>
        static constexpr bool ROtherBox = requires {
            // If this {Box} is {Copyable} the {OtherBox} should also be {Copyable}.
            requires !Copyable || OtherCopyable;

            // If this {Box} is {Movable} the {OtherBox} should also be {Movable} unless
            // {NonMovableObject} is allowed.
            requires !Movable || (OtherMovable || AllowNonMovableObject);

            // If this {Box} does not allow {NonMovableObject} then {OtherBox} should also not
            // allow {NonMovableObject}.
            requires !Movable || (AllowNonMovableObject || !OtherAllowNonMovableObject);
        };

    //// -------------------------------------------------------------------------------------------
    //// Constructors and Operators.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ObjectBox()
            : _object()
            , _heapMem(nullptr)
            , _heapMemSize(0)
            , _memAllocator()
        {}

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ObjectBox(NullType null)
            : ObjectBox()
        {}

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(NullType null) -> ObjectBox&
        {
            _DisposeObject();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator==(NullType null) const -> bool
        {
            return _HasObject();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructor. Initializes with object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
            requires RObject<T>
        ObjectBox(T&& obj)
            : ObjectBox()
        {
            _InitObject(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// AssignmentOperator. Assigns new object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
            requires RObject<T>
        auto operator=(T&& object) -> ObjectBox&
        {
            _SetObject(fwd(object));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructor.
        /// ----------------------------------------------------------------------------------------
        ObjectBox(const ObjectBox& other)
            requires Copyable
            : ObjectBox()
        {
            _CopyBox(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject, usize OtherStackSize,
            typename TOtherMemAllocator>
            requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        ObjectBox(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>& other)
            : ObjectBox()
        {
            _CopyBox(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(const ObjectBox& other) -> ObjectBox&
            requires Copyable
        {
            _CopyBox(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyAssignmentOperatorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject, usize OtherStackSize,
            typename TOtherMemAllocator>
            requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        auto operator=(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>& other) -> ObjectBox&
        {
            _CopyBox(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructor.
        /// ----------------------------------------------------------------------------------------
        ObjectBox(ObjectBox&& other)
            requires Movable
            : ObjectBox()
        {
            _MoveBox(mov(other));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, typename TOtherMemAllocator>
            requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject>
        ObjectBox(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject, OtherStackSize,
            TOtherMemAllocator>&& other)
            : ObjectBox()
        {
            _MoveBox(mov(other));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(ObjectBox&& other) -> ObjectBox&
            requires Movable
        {
            _MoveBox(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveAssignmentOperatorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, typename TOtherMemAllocator>
            requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject>
        auto operator=(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>&& other) -> ObjectBox&
        {
            _MoveBox(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Destructor.
        /// ----------------------------------------------------------------------------------------
        ~ObjectBox()
        {
            _DisposeBox();
        }

    //// -------------------------------------------------------------------------------------------
    //// Functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Sets the new object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
            requires RObject<T>
        auto SetObject(T&& obj)
        {
            _SetObject(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        auto GetObject() -> T&
        {
            return *_GetObject<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the const object.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        auto GetObject() const -> const T&
        {
            return *_GetObject<T>();
        }

    //// -------------------------------------------------------------------------------------------
    //// Box Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Copies {other} {ObjectBox} into {this} {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject, usize OtherStackSize,
            typename TOtherMemAllocator>
            requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        auto _CopyBox(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>& other)
        {
            _CopyObject(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves {other} {ObjectBox} into {this} {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, typename TOtherMemAllocator>
            requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject>
        auto _MoveBox(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>&& other)
        {
            // When allocator type is different, we cannot handle heap memory.
            // So we only move the object.
            if constexpr (!RSameAs<TMemAllocator, TOtherMemAllocator>)
            {
                _MoveObject(other);
                other._DisposeBox();
                return;
            }

            _DisposeObject();

            const usize otherObjSize = other._object.size;
            const bool otherIsUsingStackMem = other._IsUsingStackMem();
            if (otherIsUsingStackMem && otherObjSize > StackSize && _heapMemSize >= otherObjSize
                && other._heapMemSize < otherObjSize)
            {
                // We cannot deallocate our memory in the above scenario.
                other._ReleaseMem();
            }
            else
            {
                _ReleaseMem();

                _heapMem = mov(other._heapMem);
                _heapMemSize = mov(other._heapMemSize);
                _memAllocator = mov(other._memAllocator);
            }

            if (otherIsUsingStackMem)
            {
                _MoveObject(mov(other));
            }
            else
            {
                _CopyObjectData(other);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroy stored object and releases any allocated memory.
        /// ----------------------------------------------------------------------------------------
        auto _DisposeBox()
        {
            _DisposeObject();
            _ReleaseMem();
        }

    //// -------------------------------------------------------------------------------------------
    //// Object Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Stores the object.
        ///
        /// @TPARAM T Type of object to store.
        ///
        /// @PARAM[IN] obj Object to store.
        /// @PARAM[IN] forceHeap (default = false) Force store on heap.
        ///
        /// @EXPECTS Previous object is not set.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
            requires RObject<T>
        auto _InitObject(T&& obj, bool forceHeap = false)
        {
            _object.size = sizeof(T);
            _object.type = &typeid(T);

            _object.dtor = [](void* obj) { reinterpret_cast<T*>(obj)->T::~T(); };

            if constexpr (Copyable)
            {
                _object.copy = [](void* obj, const void* other) {
                    new (obj) T(*reinterpret_cast<const T*>(other));
                };
            }

            if constexpr (Movable)
            {
                if constexpr (RMoveConstructible<T>)
                {
                    _object.move = [](void* obj, void* other) {
                        new (obj) T(mov(*reinterpret_cast<T*>(other)));
                    };
                }
                else
                {
                    _object.move = nullptr;
                }
            }

            // If the object is not movable but AllowNonMovableObject is allowed,
            // we allocate it on heap to avoid object's move constructor.
            if constexpr (Movable && AllowNonMovableObject && !RMoveConstructible<T>)
            {
                forceHeap = true;
            }

            _object.obj = _AllocMem(_object.size, forceHeap);
            new (_object.obj) T(fwd(obj));
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
            requires RObject<T>
        auto _SetObject(T&& obj, bool forceHeap = false)
        {
            _DisposeObject();
            _InitObject(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        ///
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename T = void>
        auto _GetObject() -> T*
        {
            return reinterpret_cast<T*>(_object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get {TypeInfo} or stored object.
        /// ----------------------------------------------------------------------------------------
        auto _GetObjectType() const -> const TypeInfo&
        {
            return *_object.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if object is not {null}.
        /// ----------------------------------------------------------------------------------------
        auto _HasObject() const -> bool
        {
            return _object.obj != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies the object from {other} {ObjectBox} into {this} {ObjectBox}.
        ///
        /// @PARAM[IN] other {ObjectBox} of which to copy object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject, usize OtherStackSize,
            typename TOtherMemAllocator>
            requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        auto _CopyObject(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,
                             OtherStackSize, TOtherMemAllocator>& other,
            bool forceHeap = false)
        {
            _DisposeObject();

            _CopyObjectData(other);

            if constexpr (Movable)
            {
                forceHeap = forceHeap || _object.move == nullptr;
            }
            else
            {
                forceHeap = true;
            }

            _object.obj = _AllocMem(_object.size, forceHeap);
            _object.copy(_object.obj, other._object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves the object from {other} {ObjectBox} into {this} {ObjectBox}.
        ///
        /// @PARAM[IN] other {ObjectBox} of which to move object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        ///
        /// @NOTE This doesn't moves the memory from {other} {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNoneMovableObject,
            usize OtherStackSize, typename TOtherMemAllocator>
            requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNoneMovableObject>
        auto _MoveObject(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNoneMovableObject,
                             OtherStackSize, TOtherMemAllocator>&& other,
            bool forceHeap = false)
        {
            _DisposeObject();

            _CopyObjectData(other);
            forceHeap = forceHeap || _object.move == nullptr;

            _object.obj = _AllocMem(_object.size, forceHeap);
            _object.move(_object.obj, other._object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Disposes current object by calling its destructor.
        ///
        /// @NOTE This does'n deallocates memory.
        ///
        /// @SEE _ReleaseMem().
        /// ----------------------------------------------------------------------------------------
        auto _DisposeObject()
        {
            if (_object.obj != nullptr)
            {
                _object.dtor(_object.obj);
                _object = {};
            }
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Copies {ObjectData} from {otherBox}.
        ///
        /// @PARAM[IN] otherBox {ObjectBox} of which to copy {ObjectData}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, typename TOtherMemAllocator>
        auto _CopyObjectData(const ObjectBox<OtherCopyable, OtherMovable,
            OtherAllowNonMovableObject, OtherStackSize, TOtherMemAllocator>& otherBox)
        {
            auto& other = otherBox._object;

            _object.obj = other.obj;
            _object.size = other.size;
            _object.type = other.type;
            _object.dtor = other.dtor;

            if constexpr (Copyable)
            {
                if constexpr (Movable)
                {
                    _object.copy = other.copy;
                }
                else
                {
                    _object.copy = nullptr;
                }
            }

            if constexpr (Movable)
            {
                if constexpr (OtherMovable)
                {
                    _object.move = other.move;
                }
                else
                {
                    _object.move = nullptr;
                }
            }
        }

    //// -------------------------------------------------------------------------------------------
    //// Memory Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Allocates enough memory of size {size}. Uses stack memory if it is big enough.
        ///
        /// @PARAM[IN] size Size of memory to allocate.
        /// @PARAM[IN] forceHeap If {true}, allocates memory from {TMemAllocator}.
        ///
        /// @RETURNS Pointer to the allocated memory.
        /// ----------------------------------------------------------------------------------------
        auto _AllocMem(usize size, bool forceHeap = false) -> void*
        {
            if constexpr (StackSize > 0)
            {
                // Check if stack memory is big enough.
                if (!forceHeap && size <= StackSize)
                {
                    return _stackMem;
                }
            }

            // If we have previously allocated memory.
            if (_heapMem != nullptr)
            {
                if (_heapMemSize < size)
                {
                    _heapMem = _memAllocator.Realloc(_heapMem, size);
                    _heapMemSize = size;
                }
            }
            // We need to allocate heap memory.
            else
            {
                _heapMem = _memAllocator.Alloc(size);
                _heapMemSize = size;
            }

            return _heapMem;
        }

        /// ----------------------------------------------------------------------------------------
        /// Deallocates any allocated memory.
        /// ----------------------------------------------------------------------------------------
        auto _ReleaseMem()
        {
            if (_heapMem != nullptr)
            {
                _memAllocator.Dealloc(_heapMem);
                _heapMem = nullptr;
                _heapMemSize = 0;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        auto _IsUsingStackMem() const -> bool
        {
            if constexpr (StackSize > 0)
            {
                return _object.obj == _stackMem;
            }

            return false;
        }

    //// -------------------------------------------------------------------------------------------
    //// Fields
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Stack Memory.
        /// ----------------------------------------------------------------------------------------
        ATOM_CONDITIONAL_FIELD(StackSize > 0, byte[StackSize.val()]) _stackMem;

        /// ----------------------------------------------------------------------------------------
        /// Memory Allocator.
        /// ----------------------------------------------------------------------------------------
        TMemAllocator _memAllocator;

        /// ----------------------------------------------------------------------------------------
        /// Heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        void* _heapMem;

        /// ----------------------------------------------------------------------------------------
        /// Size of heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        usize _heapMemSize;

        /// ----------------------------------------------------------------------------------------
        /// Object data.
        /// ----------------------------------------------------------------------------------------
        ObjectData _object;
    };
}
