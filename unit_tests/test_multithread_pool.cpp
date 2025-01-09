// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/multithread_pool.h"
#include "../ctle/status_return.h"

#include "unit_tests.h"
#include <future>

using namespace ctle;

TEST( multithread_pool, basic_test )
{
	multithread_pool<int> mypool;

	std::vector<std::unique_ptr<int>> objlist;
	objlist.push_back( std::unique_ptr<int>( new int( 1 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 2 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 3 ) ) );

	mypool.initialize( objlist );
	EXPECT_TRUE( objlist.empty() );

	EXPECT_TRUE( mypool.item_available() );
	int *p3 = mypool.borrow_item();
	EXPECT_EQ( *p3, 3 );
	EXPECT_TRUE( mypool.item_available() );
	int *p2 = mypool.borrow_item();
	EXPECT_EQ( *p2, 2 );
	EXPECT_TRUE( mypool.item_available() );
	int *p1 = mypool.borrow_item();
	EXPECT_EQ( *p1, 1 );
	EXPECT_FALSE( mypool.item_available() );

	EXPECT_TRUE( mypool.return_item( p3 ) );
	EXPECT_TRUE( mypool.return_item( p2 ) );
	EXPECT_TRUE( mypool.return_item( p1 ) );

	int *pp1 = mypool.borrow_item();
	EXPECT_EQ( *pp1, 1 );
	int *pp2 = mypool.borrow_item();
	EXPECT_EQ( *pp2, 2 );
	int *pp3 = mypool.borrow_item();
	EXPECT_EQ( *pp3, 3 );

	mypool.deinitialize( objlist );
	EXPECT_EQ( objlist.size(), 3 );
}

TEST( multithread_pool, multithread_test )
{
	multithread_pool<int> mypool;
	std::vector<std::future<void>> tasks( 2 );

	std::vector<std::unique_ptr<int>> objlist;
	objlist.push_back( std::unique_ptr<int>( new int( 1 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 2 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 3 ) ) );

	mypool.initialize( objlist );
	EXPECT_TRUE( objlist.empty() );

	// spawn 2 treads which use the pool to borrow and return items
	for (size_t inx = 0; inx < 2; ++inx)
	{
		tasks[inx] = std::async( std::launch::async, [&]
			{
				for( size_t inx = 0; inx < 100; ++inx )
				{
					// since we only have 2 threads, we should be able to borrow at 
					// all times, since each thread will return the item before borrowing again
					int* p = mypool.borrow_item();
					EXPECT_TRUE( p != nullptr );
					if (p)
					{
						*p = *p + 1;
						std::this_thread::yield();
						mypool.return_item(p);
					}
				}
			} );
	}
	for( size_t inx = 0; inx < 2; ++inx )
	{
		tasks[inx].wait();
	}
	
	EXPECT_FALSE(mypool.item_borrowed());

	mypool.deinitialize(objlist);

	// since we only borrowed from two threads, we should have 1 left at index 0, which was never borrowed
	EXPECT_EQ(*(objlist[0].get()), 1);

	// since the other two were borrowed 100 times each (on average), they should total 205 (=100+100+2+3)
	size_t total_for_borrowed = *(objlist[1].get()) + *(objlist[2].get());
	EXPECT_EQ(total_for_borrowed, 205); 

	EXPECT_EQ(objlist.size(), 3);
}