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
            Allocator<char, 200>, 
            Allocator<short, 500>,
            Allocator<long, 180>,
            Allocator<float, 295>,
            Allocator<double, 1111> >
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

TEST(TestAllocator2, valid_1){
    Allocator<int, 100> x;
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_2){
    Allocator<int, 100> x;
    x.allocate(20);
    x.allocate(1);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_3){
    Allocator<int, 100> x;
    int* p = x.allocate(20);
    int* q = x.allocate(1);
    x.deallocate(p, 20);
    x.deallocate(q, 1);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_4){
    Allocator<double, 104> x;
    x.allocate(10);
    x.allocate(1);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_5){
    Allocator<double, 104> x;
    double* p = x.allocate(10);
    double* q = x.allocate(1);
    x.deallocate(p, 10);
    x.deallocate(q, 1);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_6){
    Allocator<short, 100> x;
    short* p = x.allocate(40);
    short* q = x.allocate(2);
    x.deallocate(p, 40);
    x.deallocate(q, 2);
    ASSERT_EQ(x.valid(), true);}

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

TEST(TestAllocator4, constructor_1) {
    const Allocator<int, 1000> x;
    ASSERT_EQ(x[0], 992);
    ASSERT_EQ(x[996], 992);}

TEST(TestAllocator4, constructor_2) {
    const Allocator<int, 12> x;
    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[8], 4);}

TEST(TestAllocator4, constructor_3) {
    const Allocator<double, 16> x;
    ASSERT_EQ(x[0], 8);
    ASSERT_EQ(x[12], 8);}

TEST(TestAllocator5, allocate_1) {
    Allocator<int, 100> x;
    ASSERT_NO_THROW(x.allocate(10));
    ASSERT_THROW(x.allocate(20), bad_alloc);}

TEST(TestAllocator5, allocate_2) {
    Allocator<int, 500> x;
    int* p = x.allocate(10);
    int* q = x.allocate(10);
    int* r = x.allocate(10);
    ASSERT_EQ(p + 12, q);
    ASSERT_EQ(q + 12, r);
    ASSERT_EQ(p + 24, r);}

TEST(TestAllocator5, allocate_3) {
    Allocator<int, 500> x;
    int* p = x.allocate(10);
    int* q = x.allocate(10);
    ASSERT_EQ(p + 12, q);}

TEST(TestAllocator5, allocate_4) {
    Allocator<double, 100> x;
    ASSERT_THROW(x.allocate(10), bad_alloc);}

TEST(TestAllocator5, allocate_5) {
    Allocator<short, 100> x;
    ASSERT_NO_THROW(x.allocate(46));}

TEST(TestAllocator6, deallocate_1) {
    Allocator<int, 100> x;
    int* p = x.allocate(10);
    x.deallocate(p, 10);
    ASSERT_EQ(*(p - 1), 92);}

TEST(TestAllocator6, deallocate_2) {
    Allocator<int, 500> x;
    int* p = x.allocate(10);
    int* q = x.allocate(10);
    int* r = x.allocate(10);
    x.deallocate(q, 10);
    ASSERT_EQ(*(p - 1), -40);
    ASSERT_EQ(*(q - 1), 40);
    ASSERT_EQ(*(r - 1), -40);}

TEST(TestAllocator6, deallocate_3) {
    Allocator<int, 500> x;
    int* p = x.allocate(10);
    int* q = x.allocate(10);
    int* r = x.allocate(10);
    x.deallocate(p, 10);
    ASSERT_EQ(*(p - 1), 40);
    x.deallocate(r, 10);
    ASSERT_EQ(*(r - 1), 396);
    x.deallocate(q, 10);
    ASSERT_EQ(*(p - 1), 492);}

TEST(TestAllocator6, deallocate_4) {
    Allocator<double, 500> x;
    double* p = x.allocate(10);
    ASSERT_THROW(x.deallocate(p - 100, 10), invalid_argument);
    ASSERT_THROW(x.deallocate(p + 1000, 10), invalid_argument);
    ASSERT_THROW(x.deallocate(p, 20), invalid_argument);}

TEST(TestAllocator2, const_index) {
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[96], 92);}

//  g++ -fprofile-arcs -ftest-coverage -pedantic -std=c++11 -Wall TestAllocator.c++ -o TestAllocator -lgtest -lgtest_main -lpthread
