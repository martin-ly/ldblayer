#ifndef TEST_DATABASE_ITERATOR_H
#define TEST_DATABASE_ITERATOR_H

#include "layer_db.h"
#include "database_iterator.h"

using namespace ldblayer;

#define AssertLdbOk( S ) ASSERT_TRUE( (S).ok() ) << (S).ToString();
#define ExpectLdbOk( S ) EXPECT_TRUE( (S).ok() ) << (S).ToString();

struct TestDatabaseIterator : public LayerDatabase
{	
	void initSample()
	{		
		AssertLdbOk( db2.Put("b", "db-0") );
		AssertLdbOk( db2.Put("d", "db-1") );
		AssertLdbOk( db2.Put("e", "db-2") );
		AssertLdbOk( db2.Put("g", "db-3") );
	}
};

#endif