// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::size_type       size_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int,    100>,
            Allocator<double, 500> >
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::size_type       size_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const size_type       s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::size_type       size_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, valid){
    Allocator<int, 100> x;
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, allocate1) {
    Allocator<int, 500> x;
    int* p = x.allocate(10);
    int* q = x.allocate(10);
    ASSERT_EQ(p + 12, q);
}

TEST(TestAllocator2, allocate2) {
    Allocator<int, 100> x;
    ASSERT_NO_THROW(x.allocate(10));
    ASSERT_THROW(x.allocate(20), bad_alloc);
}

TEST(TestAllocator2, deallocate1) {
    Allocator<int, 100> x;
    int* p = x.allocate(10);
    x.deallocate(p, 10);
    ASSERT_EQ(*(p - 1), 92);
}

// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::size_type       size_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            Allocator<int,    100>,
            Allocator<double, 500> >
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::size_type       size_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const size_type       s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::size_type       size_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator4
// --------------

// TEST(TestAllocator4, const_index) {
//     const Allocator<int, 100> x;
//     ASSERT_EQ(x[0], 92);}

// TEST(TestAllocator4, index) {
//     Allocator<int, 100> x;
//     ASSERT_EQ(x[96], 92);}

// TEST(TestAllocator4, valid){
//     Allocator<int, 100> x;
//     ASSERT_EQ(x.valid(), true);}

// TEST(TestAllocator4, allocate1) {
//     Allocator<int, 500> x;
//     int* p = x.allocate(10);
//     int* q = x.allocate(10);
//     ASSERT_EQ(p + 12, q);
// }

// TEST(TestAllocator4, allocate2) {
//     Allocator<int, 100> x;
//     ASSERT_NO_THROW(x.allocate(10));
//     ASSERT_THROW(x.allocate(20), bad_alloc);
// }

// TEST(TestAllocator4, deallocate1) {
//     Allocator<int, 100> x;
//     int* p = x.allocate(10);
//     x.deallocate(p, 10);
//     ASSERT_EQ(*(p - 1), 92);
// }    
//  g++ -fprofile-arcs -ftest-coverage -pedantic -std=c++11 -Wall TestAllocator.c++ -o TestAllocator -lgtest -lgtest_main -lpthread
