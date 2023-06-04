#pragma once
#include "Atom/Containers/InitializerList.h"
#include "Atom/Containers/IterReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// ArrayIterator iterates over raw array.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIterator: public ArrayIterator<const T>
    {
    public:
        using ArrayIterator<const T>::ArrayIterator;

    public:
        using ArrayIterator<const T>::operator*;

        constexpr T& operator *() noexcept
        {
            return *(T*)this->_it;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator +(usize steps) const noexcept
        {
            return ArrayIterator(this->_it + steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator -(usize steps) const noexcept
        {
            return ArrayIterator(this->_it - steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr isize operator -(const ArrayIterator& that) const noexcept
        {
            return this->_it - that._it;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// ArrayIterator iterates over const raw array.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIterator<const T>:
        public ArrIterTag
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator() noexcept:
            _it{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(NullPtr) noexcept:
            _it{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// Constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(const T* it) noexcept:
            _it{ it } { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr const T& operator *() const noexcept
        {
            return *_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator ==(const ArrayIterator& that) const noexcept
        {
            return this->_it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator !=(const ArrayIterator& that) const noexcept
        {
            return this->_it != that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator ++(int) noexcept
        {
            _it++;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// @TODO[Cpp2RemoveOper].
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator ++() noexcept
        {
            _it++;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator --(int) noexcept
        {
            _it--;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator +=(usize steps) noexcept
        {
            _it =+ steps;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator& operator -=(usize steps) noexcept
        {
            _it =- steps;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator +(usize steps) const noexcept
        {
            return ArrayIterator(_it + steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator operator -(usize steps) const noexcept
        {
            return ArrayIterator(_it - steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr isize operator -(const ArrayIterator& that) const noexcept
        {
            return this->_it - that._it;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        const T* _it;
    };

    static_assert(RArrIter<ArrayIterator<const int>, ArrayIterator<const int>, int>,
        "ArrayIterator<const int> does not satisfy RArrayIterator<const int> requirements.");

    static_assert(RMutArrIter<ArrayIterator<int>, ArrayIterator<int>, int>,
        "ArrayIterator<int> does not satisfy RArrayIterator<int> requirements.");
}