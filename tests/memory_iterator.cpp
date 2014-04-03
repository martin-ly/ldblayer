#include "memory_iterator.h"
#include <gtest/gtest.h>

using namespace ldblayer;

struct TestMemoryIterator: public ::testing::Test
{
	std::map<std::string, std::string> db;
	
	void SetUp()
	{
		initSample();
	}
	
	void TearDown()
	{
		db.clear();
	}
	
	void initSample()
	{
		db["b"] = "db-0";
		db["d"] = "db-1";
		db["e"] = "db-2";
		db["g"] = "db-3";
	}
};


TEST_F(TestMemoryIterator, tails)
{
	MemoryIterator it(&db);
	
	it.seekToFirst();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
	it.prev();
	ASSERT_FALSE( it.isValid() );
	EXPECT_TRUE( it.isBegin() );
	EXPECT_FALSE( it.isEnd() );
	
	it.seekToLast();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
	it.next();
	EXPECT_FALSE( it.isValid() );
	EXPECT_FALSE( it.isBegin() );
	EXPECT_TRUE( it.isEnd() );
	
	it.seekToBegin();
	EXPECT_FALSE( it.isValid() );
	EXPECT_TRUE( it.isBegin() );
	EXPECT_FALSE( it.isEnd() );
	it.next();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
	
	it.seekToEnd();
	EXPECT_FALSE( it.isValid() );
	EXPECT_FALSE( it.isBegin() );
	EXPECT_TRUE( it.isEnd() );
	it.prev();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
}

TEST_F(TestMemoryIterator, walk)
{
	MemoryIterator it(&db);
	
	it.seekToFirst();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
	it.next();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "d", it.key() );
	EXPECT_EQ( "db-1", it.value() );
	it.next();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "e", it.key() );
	EXPECT_EQ( "db-2", it.value() );
	it.prev();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "d", it.key() );
	EXPECT_EQ( "db-1", it.value() );
	it.next();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "e", it.key() );
	EXPECT_EQ( "db-2", it.value() );
	it.next();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
	it.next();
	EXPECT_FALSE( it.isValid() );
	EXPECT_FALSE( it.isBegin() );
	EXPECT_TRUE( it.isEnd() );
	it.prev();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
	it.seekToFirst();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
	it.prev();
	EXPECT_FALSE( it.isValid() );
	EXPECT_TRUE( it.isBegin() );
	EXPECT_FALSE( it.isEnd() );
	it.next();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
}

TEST_F(TestMemoryIterator, seek)
{
	MemoryIterator it(&db);
	
	// it.seek("b");
	it = db.lower_bound("b");
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
	
	// it.seek("c");
	it = db.lower_bound("c");
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "d", it.key() );
	EXPECT_EQ( "db-1", it.value() );
	
	// it.seek("g");
	it = db.lower_bound("g");
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
}

TEST_F(TestMemoryIterator, seekReverse)
{
	MemoryIterator it(&db);

	// it.seek("h");
	it = db.lower_bound("h");

	EXPECT_FALSE( it.isValid() );
	EXPECT_FALSE( it.isBegin() );
	EXPECT_TRUE( it.isEnd() );
	it.prev();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
}

TEST_F(TestMemoryIterator, seekMissing)
{
	MemoryIterator it(&db);
	
	// it.seek("d");
	it = db.lower_bound("d");
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "d", it.key() );
	EXPECT_EQ( "db-1", it.value() );
	it.prev();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "b", it.key() );
	EXPECT_EQ( "db-0", it.value() );
}

TEST_F(TestMemoryIterator, seekOutside)
{
	MemoryIterator it(&db);
	
	// it.seek("0");
	it = db.lower_bound("zzz");
	EXPECT_FALSE( it.isValid() );
	EXPECT_FALSE( it.isBegin() );
	EXPECT_TRUE( it.isEnd() );
	
	it.prev();
	ASSERT_TRUE( it.isValid() );
	EXPECT_EQ( "g", it.key() );
	EXPECT_EQ( "db-3", it.value() );
}

