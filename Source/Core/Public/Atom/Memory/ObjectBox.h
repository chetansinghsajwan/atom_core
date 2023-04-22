#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

#include "MemAllocator.h"

namespace Atom
{
    namespace Internal
    {
        struct ObjectBoxIdentifier { };
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <usize StackSize = 50, typename TMemAllocator = DefaultMemAllocator>
    class ObjectBox: public Internal::ObjectBoxIdentifier
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        struct _ObjectData
        {
            void (*copy) (void* obj, const void* other);
            void (*move) (void* obj, void* other);
            void (*dtor) (void* obj);

            usize size;
            void* obj;
            const TypeInfo* type;
        };

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TObject>
        static constexpr bool _RObject =
            TTI::IsNotBaseOf<Internal::ObjectBoxIdentifier, TObject>&&
            TTI::IsCopyConstructible<TObject>&&
            TTI::IsMoveConstructible<TObject>;

    //// -------------------------------------------------------------------------------------------
    //// Constructors and Operators.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ObjectBox() noexcept:
            _object(), _heapMem(nullptr), _heapMemSize(0) { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ObjectBox(NullType null) noexcept:
            ObjectBox() { }

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        ObjectBox& operator = (NullType null)
        {
            _DisposeObject();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        bool operator == (NullType null) const noexcept
        {
            return _HasObject();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructor. Assigns object.
        /// ----------------------------------------------------------------------------------------
        template <typename TObject>
        requires _RObject<TObject>
        ObjectBox(TObject&& object): ObjectBox()
        {
            _SetObject(FORWARD(object));
        }

        /// ----------------------------------------------------------------------------------------
        /// Operator. Assigns object.
        /// ----------------------------------------------------------------------------------------
        template <typename TObject>
        requires _RObject<TObject>
        ObjectBox& operator = (TObject&& object)
        {
            _SetObject(FORWARD(object));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructor.
        /// ----------------------------------------------------------------------------------------
        ObjectBox(const ObjectBox& other): ObjectBox()
        {
            _CopyObject(other._object);
        }

        /// ----------------------------------------------------------------------------------------
        /// TemplatedCopyConstructor.
        /// ----------------------------------------------------------------------------------------
        template <usize OtherStackSize, typename TOtherMemAllocator>
        ObjectBox(const ObjectBox<OtherStackSize, TOtherMemAllocator>& other): ObjectBox()
        {
            _CopyObject(other._object);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        ObjectBox& operator = (const ObjectBox& other)
        {
            _CopyObject(other._object);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// TemplatedCopyAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        template <usize OtherStackSize, typename TOtherMemAllocator>
        ObjectBox& operator = (const ObjectBox<OtherStackSize, TOtherMemAllocator>& other)
        {
            _CopyObject(other._object);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructor.
        /// ----------------------------------------------------------------------------------------
        ObjectBox(ObjectBox&& other): ObjectBox()
        {
            _MoveBox(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// TemplatedMoveConstructor.
        /// ----------------------------------------------------------------------------------------
        template <usize OtherStackSize, typename TOtherMemAllocator>
        ObjectBox(ObjectBox<OtherStackSize, TOtherMemAllocator>&& other): ObjectBox()
        {
            _MoveBox(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        ObjectBox& operator = (ObjectBox&& other)
        {
            _MoveBox(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// TemplatedMoveAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        template <usize OtherStackSize, typename TOtherMemAllocator>
        ObjectBox& operator = (ObjectBox<OtherStackSize, TOtherMemAllocator>&& other)
        {
            _MoveBox(other);
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
        template <typename T>
        T& GetObject() noexcept
        {
            return *_GetObject<T>();
        }

    //// -------------------------------------------------------------------------------------------
    //// Box Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize OtherStackSize>
        void _MoveBox(ObjectBox<OtherStackSize, TMemAllocator>& otherBox)
        {
            _DisposeBox();

            bool otherBoxIsUsingStackMem = otherBox._IsUsingStackMem();

            SWAP(_object, otherBox._object);
            SWAP(_heapMem, otherBox._heapMem);
            SWAP(_heapMemSize, otherBox._heapMemSize);
            SWAP(_memAllocator, otherBox._memAllocator);

            if (otherBoxIsUsingStackMem)
            {
                _MoveObject(otherBox._object);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// When allocator type is different, we cannot handle heap memory.
        /// ----------------------------------------------------------------------------------------
        template <usize OtherStackSize, typename TOtherMemAllocator>
        requires RSameAs<TMemAllocator, TOtherMemAllocator>
        void _MoveBox(ObjectBox<OtherStackSize, TOtherMemAllocator>& otherBox)
        {
            _MoveObject(otherBox._object);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        void _DisposeBox()
        {
            _DisposeObject();
            _DeallocMem();
        }

    //// -------------------------------------------------------------------------------------------
    //// Object Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        template <typename TObject>
        requires _RObject<TObject>
        void _SetObject(TObject&& object)
        {
            // TODO: Add static_assert for requirements.

            _DisposeObject();

            _object.size = sizeof(TObject);
            _object.obj = _AllocMem(_object.size);
            _object.type = &typeid(TObject);

            _object.copy = [](void* obj, const void* other)
            {
                new (obj) TObject(*reinterpret_cast<const TObject*>(other));
            };

            _object.move = [](void* obj, void* other)
            {
                new (obj) TObject(MOVE(*reinterpret_cast<TObject*>(other)));
            };

            _object.dtor = [](void* obj)
            {
                reinterpret_cast<TObject*>(obj)->TObject::~TObject();
            };

            new (_object.obj) TObject(FORWARD(object));
        }

        template <typename T = void>
        T* _GetObject() noexcept
        {
            return reinterpret_cast<T*>(_object.obj);
        }

        const TypeInfo& _GetObjectType() const noexcept
        {
            return *_object.type;
        }

        bool _HasObject() const noexcept
        {
            return _object.obj != nullptr;
        }

        void _CopyObject(const _ObjectData& otherObject)
        {
            _DisposeObject();

            _object = otherObject;
            _object.obj = _AllocMem(_object.size);
            _object.copy(_object.obj, otherObject.obj);
        }

        void _MoveObject(_ObjectData& otherObject)
        {
            _DisposeObject();

            _object = otherObject;
            _object.obj = _AllocMem(_object.size);
            _object.move(_object.obj, otherObject.obj);
            otherObject.obj = nullptr;
        }

        void _DisposeObject()
        {
            if (_object.obj != nullptr)
            {
                _object.dtor(_object.obj);
                _object.obj = nullptr;
            }
        }

    //// -------------------------------------------------------------------------------------------
    //// Memory Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        void* _AllocMem(usize size)
        {
            // Check if stack memory is big enough.
            if (size <= StackSize)
            {
                return _stackMem;
            }

            // Check for already allocated heap memory.
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

        void _DeallocMem()
        {
            if (_heapMem != nullptr)
            {
                _memAllocator.Dealloc(_heapMem);
                _heapMem = nullptr;
                _heapMemSize = 0;
            }
        }

        bool _IsUsingStackMem() const noexcept
        {
            return _object.obj == _stackMem;
        }

    //// -------------------------------------------------------------------------------------------
    //// Variables
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Stack Memory Management
        /// ----------------------------------------------------------------------------------------
        byte _stackMem[StackSize];

        /// ----------------------------------------------------------------------------------------
        /// Heap Memory management
        /// ----------------------------------------------------------------------------------------
        TMemAllocator _memAllocator;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        usize _heapMemSize;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        void* _heapMem;

        /// ----------------------------------------------------------------------------------------
        /// Object
        /// ----------------------------------------------------------------------------------------
        _ObjectData _object;
    };
}