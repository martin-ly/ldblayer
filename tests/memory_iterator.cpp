#include "memory_database.h"
#include "memory_iterator.h"
#include <gtest/gtest.h>

using namespace ldblayer;

struct TestMemoryIterator: public ::testing::Test
{
	MemoryDatabase db;
	
	void SetUp()
	{
		initSample();
	}
	
	void TearDown()
	{
		db.close();
	}
	
	void initSample()
	{
		db.Put("b", "db-0");
		db.Put("d", "db-1");
		db.Put("e", "db-2");
		db.Put("g", "db-3");
	}
};


TEST_F(TestMemoryIterator, tails)
{
	auto it = db.createIterator();
	
	it->seekToFirst();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	it->prev();
	ASSERT_FALSE( it->isValid() );
	EXPECT_TRUE( it->isBegin() );
	EXPECT_FALSE( it->isEnd() );
	
	it->seekToLast();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
	it->next();
	EXPECT_FALSE( it->isValid() );
	EXPECT_FALSE( it->isBegin() );
	EXPECT_TRUE( it->isEnd() );
	
	it->seekToBegin();
	EXPECT_FALSE( it->isValid() );
	EXPECT_TRUE( it->isBegin() );
	EXPECT_FALSE( it->isEnd() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	
	it->seekToEnd();
	EXPECT_FALSE( it->isValid() );
	EXPECT_FALSE( it->isBegin() );
	EXPECT_TRUE( it->isEnd() );
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
}

TEST_F(TestMemoryIterator, walk)
{
	auto it = db.createIterator();
	
	it->seekToFirst();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "e", it->key() );
	EXPECT_EQ( "db-2", it->value() );
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "e", it->key() );
	EXPECT_EQ( "db-2", it->value() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
	it->next();
	EXPECT_FALSE( it->isValid() );
	EXPECT_FALSE( it->isBegin() );
	EXPECT_TRUE( it->isEnd() );
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
	it->seekToFirst();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	it->prev();
	EXPECT_FALSE( it->isValid() );
	EXPECT_TRUE( it->isBegin() );
	EXPECT_FALSE( it->isEnd() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
}

TEST_F(TestMemoryIterator, seek)
{
	auto it = db.createIterator();
	
	it->seek("b");
	//it = db.lower_bound("b");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	
	it->seek("c");
	//it = db.lower_bound("c");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	
	 it->seek("g");
	//it = db.lower_bound("g");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
}

TEST_F(TestMemoryIterator, seekReverse)
{
	auto it = db.createIterator();

	it->seek("h");
	//it = db.lower_bound("h");

	EXPECT_FALSE( it->isValid() );
	EXPECT_FALSE( it->isBegin() );
	EXPECT_TRUE( it->isEnd() );
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
}

TEST_F(TestMemoryIterator, seekMissing)
{
	auto it = db.createIterator();
	
	it->seek("d");
	//it = db.lower_bound("d");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
}

TEST_F(TestMemoryIterator, seekOutside)
{
	auto it = db.createIterator();
	
	it->seek("zzz");
	//it->seek("0");
	//it = db.lower_bound("zzz");
	EXPECT_FALSE( it->isValid() );
	EXPECT_FALSE( it->isBegin() );
	EXPECT_TRUE( it->isEnd() );
	
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
}

