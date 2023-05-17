#pragma once
#include "Atom/Containers/InitializerList.h"
#include "Atom/Containers/Iterators.h"

namespace Atom
{
    /// ArrayIterator iterates over raw array.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIterator: public ArrayIterator<const T>
    {
        using TBase = ArrayIterator<const T>;
        
    public:
        using TBase::ArrayIterator;

    public:
        constexpr T& operator * () noexcept
        {
            return *(T*)this->_it;
        }

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator + (usize steps) const noexcept
        {
            return ArrayIterator(this->_it + steps);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator - (usize steps) const noexcept
        {
            return ArrayIterator(this->_it - steps);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr usize operator - (const ArrayIterator& that) const noexcept
        {
            return this->_it - that._it;
        }
    };

    /// ArrayIterator iterates over const raw array.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIterator<const T>:
        public ArrayIterTag
    {
    public:
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator() noexcept:
            _it{ nullptr } { }

        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(NullPtr) noexcept:
            _it{ nullptr } { }

        /// Constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(const T* it) noexcept:
            _it{ it } { }

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr const T& operator * () const noexcept
        {
            return *_it;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator == (const ArrayIterator& that) noexcept
        {
            return this->_it == that._it;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator != (const ArrayIterator& that) noexcept
        {
            return this->_it != that._it;
        }

        /// NOTE: We will be shifting to cpp2
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator ++ () noexcept
        {
            _it++;
            return *this;
        }

        /// NOTE: We will be shifting to cpp2
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator ++ (int) noexcept
        {
            _it++;
            return *this;
        }

        /// NOTE: We will be shifting to cpp2
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator -- () noexcept
        {
            _it--;
            return *this;
        }

        /// NOTE: We will be shifting to cpp2
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator -- (int) noexcept
        {
            _it--;
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator + (usize steps) const noexcept
        {
            return ArrayIterator(_it + steps);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator - (usize steps) const noexcept
        {
            return ArrayIterator(_it - steps);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr usize operator - (const ArrayIterator& that) const noexcept
        {
            return this->_it - that._it;
        }

    protected:
        /// 
        /// ----------------------------------------------------------------------------------------
        const T* _it;
    };

    static_assert(RArrayIter<ArrayIterator<int>, ArrayIterator<int>, int>,
        "ArrayIterator<int> does not satisfy RArrayIterator<int> requirements.");

    static_assert(RArrayIter<ArrayIterator<const int>, ArrayIterator<const int>, const int>,
        "ArrayIterator<const int> does not satisfy RArrayIterator<const int> requirements.");
}