#include "layer_db.h"
#include "LevelDatabaseTransaction.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(simpledata, LayerTransaction, ::testing::Values(no_keys, one_key, simple_keys));

void LayerTransaction::transactionUpload(LevelDatabaseTransaction* transaction, keyval values) 
{		
	for (const auto& row : values) {
		transaction->Put(row.first, row.second);
	}
}

TEST_P(LayerTransaction, put) 
{
	keyval values = GetParam();
	LevelDatabaseTransaction txn1 = db2.createTransaction();
	
	transactionUpload(&txn1, values);
	simpleCheckData(&db2, values, false);
	
	// data commited
	EXPECT_TRUE(txn1.commit().ok());
	
	simpleCheckData(&db1, values, false);
	simpleCheckData(&db2, values);
	simpleCheckData(&db1, values, false);
	
	// commit to other database
}

TEST_P(LayerTransaction, get) 
{
	keyval values = GetParam();
	LevelDatabaseTransaction txn1 = db2.createTransaction();
	
	transactionUpload(&txn1, values);
	simpleCheckData(&db2, values, false);
	
	// data commited
	EXPECT_TRUE(txn1.commit().ok());
	simpleCheckData(&db1, values, false);
	simpleCheckData(&db2, values);
	simpleCheckData(&db1, values, false);
}

TEST_P(LayerTransaction, del)
{
	keyval values = GetParam();
	LevelDatabaseTransaction txn1 = db1.createTransaction();

	transactionUpload(&txn1, values);
	EXPECT_TRUE(txn1.commit().ok());
	simpleCheckData(&db1, values);

	if (values.empty())
		return;

	txn1.Delete(values.begin()->first);
	simpleCheckData(&db1, values);
	simpleCheckData(&db2, values, false);
	simpleCheckData(&db3, values, false);

	EXPECT_TRUE(txn1.commit().ok());

	values.pop_front();
	simpleCheckData(&db1, values);
}