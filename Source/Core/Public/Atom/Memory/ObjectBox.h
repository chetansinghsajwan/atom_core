#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

namespace Atom
{
    struct DefaultMemAllocator
    {
        void* Alloc(SizeT size)
        {
            return std::malloc(size);
        }

        void* Realloc(void* mem, SizeT size)
        {
            return std::realloc(mem, size);
        }

        void Dealloc(void* mem)
        {
            std::free(mem);
        }
    };

    namespace Internal
    {
        struct ObjectBoxIdentifier { };
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <SizeT StackSize = 50, typename MemAllocatorT = DefaultMemAllocator>
    class ObjectBox: public Internal::ObjectBoxIdentifier
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        struct ObjectData
        {
            void (*copy) (void* obj, const void* other);
            void (*move) (void* obj, void* other);
            void (*dtor) (void* obj);

            SizeT size;
            void* obj;
        };

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TObject>
        static constexpr bool RObject =
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
        constexpr ObjectBox(NullType null) noexcept: ObjectBox() { }

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        ObjectBox& operator = (NullType null)
        {
            _DisposeObject();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullCheckOperator
        /// ----------------------------------------------------------------------------------------
        bool operator == (NullType null) const noexcept
        {
            return _HasObject();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructor. Assigns object.
        /// ----------------------------------------------------------------------------------------
        template <typename TObject>
            requires RObject<TObject>
        ObjectBox(TObject&& object): ObjectBox()
        {
            _SetObject(FORWARD(object));
        }

        /// ----------------------------------------------------------------------------------------
        /// Operator. Assigns object.
        /// ----------------------------------------------------------------------------------------
        template <typename TObject>
            requires RObject<TObject>
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
        template <SizeT OtherStackSize, typename OtherMemAllocator>
        ObjectBox(const ObjectBox<OtherStackSize, OtherMemAllocator>& other): ObjectBox()
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
        template <SizeT OtherStackSize, typename OtherMemAllocator>
        ObjectBox& operator = (const ObjectBox<OtherStackSize, OtherMemAllocator>& other)
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
        template <SizeT OtherStackSize, typename OtherMemAllocator>
        ObjectBox(ObjectBox<OtherStackSize, OtherMemAllocator>&& other): ObjectBox()
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
        template <SizeT OtherStackSize, typename OtherMemAllocator>
        ObjectBox& operator = (ObjectBox<OtherStackSize, OtherMemAllocator>&& other)
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
        template <SizeT OtherStackSize>
        void _MoveBox(ObjectBox<OtherStackSize, MemAllocatorT>& otherBox)
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
        template <SizeT OtherStackSize, typename OtherMemAllocator,
            typename = TTI::EnableIf<TTI::IsNotSame<MemAllocatorT, OtherMemAllocator>>>
        void _MoveBox(ObjectBox<OtherStackSize, OtherMemAllocator>& otherBox)
        {
            _MoveObject(otherBox._object);
        }

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
            requires RObject<TObject>
        void _SetObject(TObject&& object)
        {
            // TODO: Add static_assert for requirements.

            _DisposeObject();

            _object.size = sizeof(TObject);
            _object.obj = _AllocMem(_object.size);

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

        bool _HasObject() const noexcept
        {
            return _object.obj != nullptr;
        }

        void _CopyObject(const ObjectData& otherObject)
        {
            _DisposeObject();

            _object = otherObject;
            _object.obj = _AllocMem(_object.size);
            _object.copy(_object.obj, otherObject.obj);
        }

        void _MoveObject(ObjectData& otherObject)
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
        void* _AllocMem(SizeT size)
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
        /// Stack Memory Management
        byte _stackMem[StackSize];

        /// Heap Memory management
        MemAllocatorT _memAllocator;
        SizeT _heapMemSize;
        void* _heapMem;

        /// Object
        ObjectData _object;
    };
}