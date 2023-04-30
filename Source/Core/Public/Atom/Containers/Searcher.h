// #include "Atom/Containers/Iterator.h"
// 
// namespace Atom::Collections
// {
//     template <typename T, RInputIterable<T> TInput>
//     class Searcher
//     {
//         using TIterator = TInput::TIterator;
// 
//     public:
//         constexpr Searcher(const TInput& input) noexcept:
//             in(in) { }
// 
//     protected:
//         const TInput& in;
//     };
// }