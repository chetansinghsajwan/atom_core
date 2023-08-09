#pragma once
#include "IterReqChecks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// ArrIter iterates over const raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrIter: public ArrIterTag
    {
    public:
        using TElem = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ArrIter() noex:
            _it{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ArrIter(NullPtr) noex:
            _it{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ArrIter(const T* it) noex:
            _it{ it } { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op*() const noex -> const T&
        {
            return *_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op==(const ArrIter& that) const noex -> bool
        {
            return self._it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op!=(const ArrIter& that) const noex -> bool
        {
            return self._it != that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op++(i32) noex -> ArrIter&
        {
            _it++;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// @TODO[Cpp2RemoveOper].
        /// ----------------------------------------------------------------------------------------
        cexpr fn op++() noex -> ArrIter&
        {
            _it++;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op--(i32) noex -> ArrIter&
        {
            _it--;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op+=(usize steps) noex -> ArrIter&
        {
            _it =+ steps;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op-=(usize steps) noex -> ArrIter&
        {
            _it =- steps;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op+(usize steps) const noex -> ArrIter
        {
            return ArrIter(_it + steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op-(usize steps) const noex -> ArrIter
        {
            return ArrIter(_it - steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op-(const ArrIter& that) const noex -> isize
        {
            return self._it - that._it;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        const T* _it;
    };

    ATOM_SATISFIES_ARR_ITER_TEMP(ArrIter);

    /// --------------------------------------------------------------------------------------------
    /// MutArrIter iterates over raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrIter: public ArrIter<T>
    {
    public:
        using ArrIter<T>::ArrIter;

    public:
        using ArrIter<T>::op*;

        cexpr fn op*() noex -> T&
        {
            return *(T*)self._it;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op+(usize steps) const noex -> MutArrIter
        {
            return MutArrIter(self._it + steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op-(usize steps) const noex -> MutArrIter
        {
            return MutArrIter(self._it - steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr fn op-(const MutArrIter& that) const noex -> isize
        {
            return self._it - that._it;
        }
    };

    ATOM_SATISFIES_MUT_ARR_ITER_TEMP(ArrIter);
}