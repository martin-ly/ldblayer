#include "db_iterator.h"

TEST_F(TestDatabaseIterator, tails)
{
	initSample();
	auto it = db2.createIterator();
	
	it->seekToFirst();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	it->prev();
	EXPECT_FALSE( it->isValid() );
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

TEST_F(TestDatabaseIterator, walk)
{
	initSample();
	auto it = db2.createIterator();
	
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

TEST_F(TestDatabaseIterator, seek)
{
	initSample();
	auto it = db2.createIterator();
	
	it->seek("b");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	
	it->seek("c");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	
	it->seek("g");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
}

TEST_F(TestDatabaseIterator, seekReverse)
{
	initSample();
	auto it = db2.createIterator();
	
	it->seek("d");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
}

TEST_F(TestDatabaseIterator, seekMissing)
{
	initSample();
	auto it = db2.createIterator();
	
	it->seek("000");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	it->prev();
	EXPECT_FALSE( it->isValid() );
	EXPECT_TRUE( it->isBegin() );
	EXPECT_FALSE( it->isEnd() );
	
	it->seek("000");
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "b", it->key() );
	EXPECT_EQ( "db-0", it->value() );
	it->next();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "d", it->key() );
	EXPECT_EQ( "db-1", it->value() );
	
}

TEST_F(TestDatabaseIterator, seekOutside)
{
	initSample();
	auto it = db2.createIterator();
	
	it->seek("zzz");
	EXPECT_FALSE( it->isValid() );
	
	it->prev();
	ASSERT_TRUE( it->isValid() );
	EXPECT_EQ( "g", it->key() );
	EXPECT_EQ( "db-3", it->value() );
}