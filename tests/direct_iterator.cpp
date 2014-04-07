#include "direct_db.h"

#include "layer_iterator.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(simpledata, Iterator, ::testing::Values(no_keys, one_key, simple_keys));

TEST_P(Iterator, seek_to_first)
{
    transactionUpload();
    
	LayerIterator iterator = db.createIterator();
    
    keyval values = GetParam();
    
	iterator.seekToFirst();
    if (!values.empty()) {
        EXPECT_TRUE(iterator.isValid());
    } else {
		EXPECT_FALSE(iterator.isValid());
    }
}

TEST_P(Iterator, seek_to_last)
{
    transactionUpload();
    
	LayerIterator iterator = db.createIterator();
    
    keyval values = GetParam();    
	iterator.seekToLast();
    if (!values.empty()) {
        EXPECT_TRUE(iterator.isValid());
    } else {
		EXPECT_FALSE(iterator.isValid());
    }
}

TEST_P(Iterator, seek)
{
    transactionUpload();
    
	LayerIterator iterator = db.createIterator();
    
    keyval values = GetParam();
   

    if (values.empty()) {       
		iterator.seek("");
        EXPECT_FALSE(iterator.isValid());        
        return;
    }
    
    for (auto i = values.rbegin(); i != values.rend(); i++) {
		iterator.seek(i->first);
        EXPECT_TRUE(iterator.isValid());
    }    
    
    // seek non exist key, that should be lexicography first
    iterator.seek("a");
    EXPECT_TRUE(iterator.isValid()) << "leveldb found: " << iterator.key() << " - " << iterator.value();
    EXPECT_EQ(iterator.key(), "key_a");
    EXPECT_EQ(iterator.value(), "data_a");
    
	// seek non exist key that lexicography should be the last
	iterator.seek("zzzzzzzzzzzzz");
	EXPECT_FALSE(iterator.isValid());
	
	// seek non exist key that lexicography exist
	iterator.seek("a_ke");
	EXPECT_TRUE(iterator.isValid());
	EXPECT_EQ(iterator.key(), "key_a");
}

TEST_P(Iterator, next) 
{
    transactionUpload();
    
	LayerIterator db_iterator = db.createIterator();
    
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
    db_iterator.next();
    EXPECT_FALSE(db_iterator.isValid());
    
    if (values.empty()) {
        return;
    }
    
    // seek for last element, and run next
    const std::string lastKey = values.back().first;
    const std::string lastValue= values.back().second;
    
	db_iterator.seek(lastKey);
    EXPECT_TRUE(db_iterator.isValid());
    EXPECT_EQ(db_iterator.key(), lastKey);
    EXPECT_EQ(db_iterator.value(), lastValue);
    
    // we are already at the end
	db_iterator.next();    
    EXPECT_FALSE(db_iterator.isValid());
    
    // should be the same
	db_iterator.seekToLast();
    EXPECT_TRUE(db_iterator.isValid());
    EXPECT_EQ(db_iterator.key(), lastKey);
    EXPECT_EQ(db_iterator.value(), lastValue);
    
    // we are already at the end
	db_iterator.next();
    EXPECT_FALSE(db_iterator.isValid());
    
    // check seek to first and next
}