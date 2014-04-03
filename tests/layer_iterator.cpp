#include "layer_db.h"

#include "LevelDatabaseIterator.h"
#include "LevelDatabaseTransaction.h"
#include <algorithm>

using namespace test;

INSTANTIATE_TEST_CASE_P(simpledata, LayerIterator, ::testing::Values(no_keys, one_key, simple_keys));
INSTANTIATE_TEST_CASE_P(ordereddata, LayerIterator, ::testing::Values(begin_keys, midle_keys, end_keys));

TEST_F(LayerIterator, registered_prefixes) 
{
	Database::prefix_list_t prefixes = dbPhisical.getDbPrefixes();
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
	
	// end of test block
	
	// insert keys AFTER this database
	LevelDatabaseTransaction txn3 = db3.createTransaction();
	transactionUpload(&txn3, test::end_keys);
	EXPECT_TRUE(txn3.commit().ok());
	
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
	
	EXPECT_TRUE(iterator3.seek(end_keys.begin()->first));
	EXPECT_TRUE(iterator3.isValid());
	EXPECT_EQ(test::end_keys.begin()->first, iterator3.key());
	EXPECT_EQ(test::end_keys.begin()->second, iterator3.value());
	
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
	
	// seek non exist key that lexicography exist (first key)
	EXPECT_TRUE(iterator2.seek("a"));
	EXPECT_TRUE(iterator2.isValid());
	EXPECT_EQ(values.begin()->first, iterator2.key());
	EXPECT_EQ(values.begin()->second, iterator2.value());
	
	// try to seek existing key, but from other database	
	if (values.begin()->first != begin_keys.begin()->first)  {
		EXPECT_FALSE(iterator1.seek(values.begin()->first)) << values.begin()->first;
	}	
}


TEST_P(LayerIterator, next) 
{
	keyval values = GetParam();
	
	LevelDatabaseTransaction txn2 = db2.createTransaction();
	transactionUpload(&txn2, values);
	EXPECT_TRUE(txn2.commit().ok());
	
	LevelDatabaseIterator iterator1 = db1.createIterator();
	LevelDatabaseIterator iterator2 = db2.createIterator();
	LevelDatabaseIterator iterator3 = db3.createIterator();
	
	if (values.empty()) {		
		return;
	}
	
	unsigned int count = 0;
	
	keyval sorted_values = values;
	sorted_values.sort();

	auto i = sorted_values.begin();
	for (iterator2.seekToFirst(); iterator2.isValid(); iterator2.next(), ++i, ++count) {
		EXPECT_EQ(iterator2.key(), i->first);
		EXPECT_EQ(iterator2.value(), i->second);
	}
	
	EXPECT_EQ(values.size(), count);
	
	EXPECT_TRUE(iterator2.seekToLast());
	EXPECT_FALSE(iterator2.next());
	
	// insert keys BEFORE this database (to first db1)
	LevelDatabaseTransaction txn1 = db1.createTransaction();
	transactionUpload(&txn1, test::begin_keys);
	EXPECT_TRUE(txn1.commit().ok());	
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	count = 0;
	i = sorted_values.begin();
	for (iterator2.seekToFirst(); iterator2.isValid(); iterator2.next(), ++i, ++count) {
		EXPECT_EQ(iterator2.key(), i->first);
		EXPECT_EQ(iterator2.value(), i->second);
	}
	
	EXPECT_EQ(values.size(), count);	
	
	keyval begin_keyes_sorted = begin_keys;
	begin_keyes_sorted.sort();
	
	count = 0;
	auto begin_keys_iter = begin_keyes_sorted.begin();
	for (iterator1.seekToFirst(); iterator1.isValid(); iterator1.next(), ++begin_keys_iter, ++count) {
		EXPECT_EQ(iterator1.key(), begin_keys_iter->first);
		EXPECT_EQ(iterator1.value(), begin_keys_iter->second);
	}
	
	EXPECT_EQ(begin_keyes_sorted.size(), count);
	
	EXPECT_TRUE(iterator1.seekToLast());
	EXPECT_FALSE(iterator1.next());
	
	// insert keys AFTER this database
	LevelDatabaseTransaction txn3 = db3.createTransaction();
	transactionUpload(&txn3, test::end_keys);
	EXPECT_TRUE(txn3.commit().ok());
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	count = 0;
	i = sorted_values.begin();
	for (iterator2.seekToFirst(); iterator2.isValid(); iterator2.next(), ++i, ++count) {
		EXPECT_EQ(iterator2.key(), i->first);
		EXPECT_EQ(iterator2.value(), i->second);
	}
	
	EXPECT_EQ(values.size(), count);
}

TEST_P(LayerIterator, prev)
{
	keyval values = GetParam();
	
	LevelDatabaseTransaction txn2 = db2.createTransaction();
	transactionUpload(&txn2, values);
	EXPECT_TRUE(txn2.commit().ok());
	
	LevelDatabaseIterator iterator1 = db1.createIterator();
	LevelDatabaseIterator iterator2 = db2.createIterator();
	LevelDatabaseIterator iterator3 = db3.createIterator();
	
	if (values.empty()) {		
		return;
	}
	
	unsigned int count = 0;
	
	keyval sorted_values = values;
	sorted_values.sort();
	
	auto i = sorted_values.rbegin();
	for (iterator2.seekToLast(); iterator2.isValid(); iterator2.prev(), ++i, ++count) {
		EXPECT_EQ(iterator2.key(), i->first);
		EXPECT_EQ(iterator2.value(), i->second);
	}
	
	EXPECT_EQ(values.size(), count);
	
	EXPECT_TRUE(iterator2.seekToFirst());
	EXPECT_FALSE(iterator2.prev());
	
	// insert keys BEFORE this database (to first db1)
	LevelDatabaseTransaction txn1 = db1.createTransaction();
	transactionUpload(&txn1, test::begin_keys);
	EXPECT_TRUE(txn1.commit().ok());	
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	count = 0;
	i = sorted_values.rbegin();
	for (iterator2.seekToLast(); iterator2.isValid(); iterator2.prev(), ++i, ++count) {
		EXPECT_EQ(iterator2.key(), i->first);
		EXPECT_EQ(iterator2.value(), i->second);
	}
	
	EXPECT_EQ(values.size(), count);	
	
	keyval begin_keyes_sorted = begin_keys;
	begin_keyes_sorted.sort();
	
	count = 0;
	auto begin_keys_iter = begin_keyes_sorted.rbegin();
	for (iterator1.seekToLast(); iterator1.isValid(); iterator1.prev(), ++begin_keys_iter, ++count) {
		EXPECT_EQ(iterator1.key(), begin_keys_iter->first);
		EXPECT_EQ(iterator1.value(), begin_keys_iter->second);
	}
	
	EXPECT_EQ(begin_keyes_sorted.size(), count);
	
	EXPECT_TRUE(iterator1.seekToFirst());
	EXPECT_FALSE(iterator1.prev());
	
	// insert keys AFTER this database
	LevelDatabaseTransaction txn3 = db3.createTransaction();
	transactionUpload(&txn3, test::end_keys);
	EXPECT_TRUE(txn3.commit().ok());
	
	// revalidate iterators after commit of transactions
	iterator1.reopen();
	iterator2.reopen();
	iterator3.reopen();
	
	count = 0;
	i = sorted_values.rbegin();
	for (iterator2.seekToLast(); iterator2.isValid(); iterator2.prev(), ++i, ++count) {
		EXPECT_EQ(iterator2.key(), i->first);
		EXPECT_EQ(iterator2.value(), i->second);
	}
	
	EXPECT_EQ(values.size(), count);
	
	count = 0;
	keyval end_keys_sorted = end_keys;
	end_keys_sorted.sort();
	i = end_keys_sorted.rbegin();
	for (iterator3.seekToLast(); iterator3.isValid(); iterator3.prev(), ++i, ++count) {
		EXPECT_EQ(iterator3.key(), i->first);
		EXPECT_EQ(iterator3.value(), i->second);
	}
	
	EXPECT_EQ(end_keys_sorted.size(), count);
}