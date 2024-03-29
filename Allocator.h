// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument
#include <cmath>

#include "gtest/gtest.h"

using namespace std;

// ---------
// Allocator
// ---------

template <typename T, std::size_t N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

    private:
        // ----
        // data
        // ----

        char a[N];

        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * Checks the heap, makes sure sentinel values are accurate.
         * Returns false if inaccurate, true if accurate
         */
        FRIEND_TEST(TestAllocator2, valid_1);
        FRIEND_TEST(TestAllocator2, valid_2);
        FRIEND_TEST(TestAllocator2, valid_3);
        FRIEND_TEST(TestAllocator2, valid_4);
        FRIEND_TEST(TestAllocator2, valid_5);
        FRIEND_TEST(TestAllocator2, valid_6);
        bool valid () const {
            int i = 0;
            while(i < sizeof(a)/sizeof(*a)){
                int first = (*this)[i];
                i += 4;
                i += abs(first);
                int last = (*this)[i];
                if(first != last){
                    cout << "first: " << first << " last: " << last << endl;
                    return false;
                }
                i += 4;
            }
            return true;}

        /**
         * O(1) in space
         * O(1) in time
         * Read/Write index access to the array from a pointer value
         * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
        FRIEND_TEST(TestAllocator2, index);
        int& operator [] (int i) {
            return *reinterpret_cast<int*>(&a[i]);}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        Allocator () {
            if(N < sizeof(T) + (2 * sizeof(int))) {
                bad_alloc x;
                throw x;
            }
            (*this)[0] = N - 8; 
            (*this)[N - 4] = N - 8;
            assert(valid());}


        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * throw a bad_alloc exception, if n is invalid
         * allocates space for n value-types
         */
        pointer allocate (size_type n) {
            assert(valid());
            if(n == 0)
                return nullptr;
            int min = sizeof(T) + (2 * sizeof(int));
            int size = n * sizeof(value_type);
            int i = 0;
            pointer ptr = reinterpret_cast<pointer>(&(*this)[i + 4]);
            while(i < sizeof(a)/sizeof(*a)){
                int first = (*this)[i];
                if(first >= min - 8){
                    if((size > first || first - size < min) && first - size != 0) {
                        bad_alloc x;
                        throw x;
                    }
                    (*this)[i] = -1 * size;
                    (*this)[i + size + 4] = -1 * size;
                    if(first - size != 0){ // if block does not exactly fit
                        (*this)[i + size + 8] = first - size - 8;
                        (*this)[i + first + 4] = first - size - 8;
                    }
                    ptr = reinterpret_cast<pointer>(&(*this)[i + 4]);
                }
                i += 4;
                i += abs(first);
                i += 4;
            }
            assert(valid());
            return ptr;}

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * throw an invalid_argument exception, if p is invalid
         * deallocates memory at pointer p for n value-types
         * coalesces the block with other adjacent blocks if they are free
         */
        void deallocate (pointer p, size_type n) {
            assert(valid());
            if(p != nullptr) {
                int size = n * sizeof(value_type);
                int T_size = sizeof(T);

                if(p < reinterpret_cast<const pointer>(&(*this)[0]) || p >= reinterpret_cast<const pointer>(&(*this)[N]) || size != *(reinterpret_cast<int*>(p) - 1) * -1) {
                    invalid_argument x("invalid pointer");
                    throw x;
                }
                int* start_sent = reinterpret_cast<int*>(p) - 1;
                int* end_sent = reinterpret_cast<int*>(p + n);

                *start_sent = size;
                *end_sent = size;
                int temp_size = size;
                // coalesce adjacent free blocks
                if(reinterpret_cast<int*>(p) - 2 > reinterpret_cast<const int*>(&(*this)[0]) && *(reinterpret_cast<int*>(p) - 2) > 0) { // if free block in front of pointer
                    int prev_s = *(reinterpret_cast<int*>(p) - 2); // previous block size
                    start_sent = reinterpret_cast<int*>(p - prev_s/T_size) - 3; // new start sentinal
                    temp_size = temp_size + 8 + prev_s; // new size
                    *start_sent = temp_size;
                    *end_sent = temp_size;
                }
                if(reinterpret_cast<int*>(p + size/T_size) + 1 < reinterpret_cast<const int*>(&(*this)[N]) && *(reinterpret_cast<int*>(p + size/T_size) + 1) > 0) {
                    int next_s = *(reinterpret_cast<int*>(p + size/T_size) + 1); // next block size
                    end_sent = reinterpret_cast<int*>(reinterpret_cast<char*>(p + size/T_size) + next_s + 8); // new end sentinal
                    temp_size = temp_size + 8 + next_s; // new size
                    *start_sent = temp_size;
                    *end_sent = temp_size;
                }
            }
            assert(valid());}

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * Read-only index access to array from pointer value
         */
        const int& operator [] (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

#endif // Allocator_h
