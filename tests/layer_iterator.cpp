#include "layer_db.h"

#include "LevelDatabaseIterator.h"
#include "LevelDatabaseTransaction.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(simpledata, LayerIterator, ::testing::Values(no_keys, one_key, simple_keys));
INSTANTIATE_TEST_CASE_P(ordereddata, LayerIterator, ::testing::Values(begin_keys, midle_keys, end_keys));

TEST_F(LayerIterator, registered_prefixes) 
{
	LevelDatabase::prefix_list_t prefixes = dbPhisical.getDbPrefixes();
	auto i = prefixes.begin();
	EXPECT_EQ(*i, db1.getPrefix());
	EXPECT_EQ(*++i, db2.getPrefix());
	EXPECT_EQ(*++i, db3.getPrefix());
}

TEST_P(LayerIterator, seek_to_first)
{
	keyval values = GetParam();
	
	LevelDatabaseTransaction txn2 = db2.createTransaction();
	transactionUpload(&txn2, values);
	EXPECT_TRUE(txn2.commit().ok());
		
	LevelDatabaseIterator iterator1 = db1.createIterator();
	LevelDatabaseIterator iterator2 = db2.createIterator();
	LevelDatabaseIterator iterator3 = db3.createIterator();
	
	if (values.empty()) {
		EXPECT_FALSE(iterator2.seekToFirst());    
		EXPECT_FALSE(iterator2.isValid());    
		return;
	}
		
	EXPECT_TRUE(iterator2.seekToFirst());
	EXPECT_TRUE(iterator2.isValid());
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	// check if empty layouts realy empty
	EXPECT_FALSE(iterator1.seekToFirst());
	EXPECT_FALSE(iterator1.isValid());
	
	// insert keys BEFORE this database (to first db1)
	LevelDatabaseTransaction txn1 = db1.createTransaction();
	transactionUpload(&txn1, test::begin_keys);
	EXPECT_TRUE(txn1.commit().ok());
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	// check if layots iterators still return correct data
	EXPECT_TRUE(iterator1.seekToFirst());
	EXPECT_TRUE(iterator1.isValid());
	EXPECT_EQ(test::begin_keys.begin()->first, iterator1.key());
	EXPECT_EQ(test::begin_keys.begin()->second, iterator1.value());
	
	EXPECT_TRUE(iterator2.seekToFirst());
	EXPECT_TRUE(iterator2.isValid());	
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	EXPECT_FALSE(iterator3.seekToFirst());
	EXPECT_FALSE(iterator3.isValid());
}


TEST_P(LayerIterator, seek_to_last)
{
	keyval values = GetParam();
	
	LevelDatabaseTransaction txn2 = db2.createTransaction();
	transactionUpload(&txn2, values);
	EXPECT_TRUE(txn2.commit().ok());
	
	LevelDatabaseIterator iterator1 = db1.createIterator();
	LevelDatabaseIterator iterator2 = db2.createIterator();
	LevelDatabaseIterator iterator3 = db3.createIterator();
	
	if (values.empty()) {
		EXPECT_FALSE(iterator2.seekToLast());    
		EXPECT_FALSE(iterator2.isValid());    
		return;
	}
	
	EXPECT_TRUE(iterator2.seekToLast());
	EXPECT_TRUE(iterator2.isValid());
	EXPECT_EQ(values.rbegin()->first, iterator2.key());
	EXPECT_EQ(values.rbegin()->second, iterator2.value());

	// check if empty layouts realy empty
	EXPECT_FALSE(iterator1.seekToFirst());
	EXPECT_FALSE(iterator1.isValid());
	
	EXPECT_FALSE(iterator3.seekToFirst());
	EXPECT_FALSE(iterator3.isValid());
	
	// insert keys BEFORE this database (to first db1)
	LevelDatabaseTransaction txn1 = db1.createTransaction();
	transactionUpload(&txn1, test::begin_keys);
	EXPECT_TRUE(txn1.commit().ok());
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	// check if layots iterators still return correct data
	EXPECT_TRUE(iterator1.seekToLast());
	EXPECT_TRUE(iterator1.isValid());
	EXPECT_EQ(test::begin_keys.rbegin()->first, iterator1.key());
	EXPECT_EQ(test::begin_keys.rbegin()->second, iterator1.value());
	
	EXPECT_TRUE(iterator2.seekToLast());
	EXPECT_TRUE(iterator2.isValid());	
	EXPECT_EQ(values.rbegin()->first, iterator2.key());
	EXPECT_EQ(values.rbegin()->second, iterator2.value());
	
	EXPECT_FALSE(iterator3.seekToFirst());
	EXPECT_FALSE(iterator3.isValid());
	
	// insert keys AFTER this database
	LevelDatabaseTransaction txn3 = db3.createTransaction();
	transactionUpload(&txn3, test::end_keys);
	EXPECT_TRUE(txn3.commit().ok());
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	// check if layots iterators still return correct data
	EXPECT_TRUE(iterator1.seekToLast());
	EXPECT_TRUE(iterator1.isValid());
	EXPECT_EQ(test::begin_keys.rbegin()->first, iterator1.key());
	EXPECT_EQ(test::begin_keys.rbegin()->second, iterator1.value());
	
	EXPECT_TRUE(iterator2.seekToLast());
	EXPECT_TRUE(iterator2.isValid());	
	EXPECT_EQ(values.rbegin()->first, iterator2.key());
	EXPECT_EQ(values.rbegin()->second, iterator2.value());
	
	EXPECT_TRUE(iterator3.seekToLast());
	EXPECT_TRUE(iterator3.isValid());	
	EXPECT_EQ(test::end_keys.rbegin()->first, iterator3.key());
	EXPECT_EQ(test::end_keys.rbegin()->second, iterator3.value());
}


TEST_P(LayerIterator, seek)
{
	keyval values = GetParam();
	
	LevelDatabaseTransaction txn2 = db2.createTransaction();
	transactionUpload(&txn2, values);
	EXPECT_TRUE(txn2.commit().ok());
	
	LevelDatabaseIterator iterator1 = db1.createIterator();
	LevelDatabaseIterator iterator2 = db2.createIterator();
	LevelDatabaseIterator iterator3 = db3.createIterator();
	
	if (values.empty()) {
		EXPECT_FALSE(iterator2.seekToLast());    
		EXPECT_FALSE(iterator2.isValid());    
		return;
	}
	
	std::string value;
	EXPECT_TRUE(iterator2.seek(values.begin()->first));
	EXPECT_TRUE(iterator2.isValid());
	
	// check if we search for first element (the same as seekToFirst)
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	EXPECT_FALSE(iterator1.seek(values.begin()->first));
	EXPECT_FALSE(iterator1.isValid());
	EXPECT_FALSE(iterator3.seek(values.begin()->first));
	EXPECT_FALSE(iterator3.isValid());
	
	if (values.size() == 1)
		return;
	
	// seek for some random value (second element in test data)
	auto i = values.begin();
	++i;
	
	EXPECT_TRUE(iterator2.seek(i->first));
	EXPECT_TRUE(iterator2.isValid());
	
	// check if we search for first element (the same as seekToFirst)
	EXPECT_EQ(i->first, iterator2.key());
	EXPECT_EQ(i->second, iterator2.value());
	
	// seek non exist key that never exists on any test data
	EXPECT_FALSE(iterator2.seek("zzzzzzzzzzzzz"));
	EXPECT_FALSE(iterator2.isValid());
	
	// seek non exist key that lexicography exist
	EXPECT_TRUE(iterator2.seek("a"));
	EXPECT_TRUE(iterator2.isValid());
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	
	// end of test block
	
	// insert keys BEFORE this database (to first db1)
	LevelDatabaseTransaction txn1 = db1.createTransaction();
	transactionUpload(&txn1, test::begin_keys);
	EXPECT_TRUE(txn1.commit().ok());
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();

	// check if we search for first element (the same as seekToFirst)
	EXPECT_TRUE(iterator2.seek(values.begin()->first));
	EXPECT_TRUE(iterator2.isValid());	
	
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	EXPECT_TRUE(iterator1.seek(begin_keys.begin()->first));
	EXPECT_TRUE(iterator1.isValid());
	EXPECT_EQ(test::begin_keys.begin()->first, iterator1.key());
	EXPECT_EQ(test::begin_keys.begin()->second, iterator1.value());
	
	EXPECT_FALSE(iterator3.seek(values.begin()->first));
	EXPECT_FALSE(iterator3.isValid());
	
	if (values.size() == 1)
		return;
	
	// seek for some random value (second element in test data)
	i = values.begin();
	++i;
	
	EXPECT_TRUE(iterator2.seek(i->first));
	EXPECT_TRUE(iterator2.isValid());
	
	// check if we search for first element (the same as seekToFirst)
	EXPECT_EQ(i->first, iterator2.key());
	EXPECT_EQ(i->second, iterator2.value());
	
	// seek non exist key that never exists on any test data
	EXPECT_FALSE(iterator2.seek("zzzzzzzzzzzzz"));
	EXPECT_FALSE(iterator2.isValid());
	
	// seek non exist key that lexicography exist
	EXPECT_TRUE(iterator2.seek("a"));
	EXPECT_TRUE(iterator2.isValid());
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	// try to seek existing key, but from other database	
	if (values.begin()->first != begin_keys.begin()->first)  {
		EXPECT_FALSE(iterator1.seek(values.begin()->first)) << values.begin()->first;
	}	
}

/*
TEST_P(IteratorsOperations, iterator_next) 
{
	transactionUpload();
	
	LevelDatabaseIterator db_iterator = db.createIterator(nullptr);
	
	keyval values = GetParam();
	
	// simple navigation from start to end
	auto iter = values.begin();    
	for ( db_iterator.seekToFirst(); db_iterator.isValid(); db_iterator.next()) {        
		EXPECT_EQ(db_iterator.key(), iter->first);
		EXPECT_EQ(db_iterator.value(), iter->second);
		EXPECT_EQ(*db_iterator, iter->second);
		
		++iter;
	}
	
	// we are already at the end
	EXPECT_FALSE(db_iterator.next());
	
	if (values.empty()) {
		return;
	}
	
	// seek for last element, and run next
	const std::string lastKey = values.back().first;
	const std::string lastValue= values.back().second;
	
	EXPECT_TRUE(db_iterator.seek(lastKey));
	EXPECT_EQ(db_iterator.key(), lastKey);
	EXPECT_EQ(db_iterator.value(), lastValue);
	
	// we are already at the end
	EXPECT_FALSE(db_iterator.next());
	EXPECT_FALSE(db_iterator.isValid());
	
	// should be the same
	EXPECT_TRUE(db_iterator.seekToLast());
	EXPECT_EQ(db_iterator.key(), lastKey);
	EXPECT_EQ(db_iterator.value(), lastValue);
	
	// we are already at the end
	EXPECT_FALSE(db_iterator.next());
	EXPECT_FALSE(db_iterator.isValid());
	
	// check seek to first and next
}*/