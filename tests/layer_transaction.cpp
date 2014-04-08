#include "layer_db.h"
#include "layer_transaction.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(simpledata, TestLayerTransaction, ::testing::Values(no_keys, one_key, simple_keys));

void TestLayerTransaction::transactionUpload(std::unique_ptr<TransactionAbstract>& transaction, keyval values) 
{		
	for (const auto& row : values) {
		transaction->Put(row.first, row.second);
	}
}

TEST_P(TestLayerTransaction, put) 
{
	keyval values = GetParam();
	std::unique_ptr<TransactionAbstract> txn1 = db2.createTransaction();
	
	transactionUpload(txn1, values);
	simpleCheckData(&db2, values, false);
	
	// data commited
	EXPECT_TRUE(txn1->commit());
	
	simpleCheckData(&db1, values, false);
	simpleCheckData(&db2, values);
	simpleCheckData(&db1, values, false);
	
	// commit to other database
}

TEST_P(TestLayerTransaction, get) 
{
	keyval values = GetParam();
	std::unique_ptr<TransactionAbstract> txn1 = db2.createTransaction();
	
	transactionUpload(txn1, values);
	simpleCheckData(&db2, values, false);
	
	// data commited
	EXPECT_TRUE(txn1->commit());
	simpleCheckData(&db1, values, false);
	simpleCheckData(&db2, values);
	simpleCheckData(&db1, values, false);
}

TEST_P(TestLayerTransaction, del)
{
	keyval values = GetParam();
	std::unique_ptr<TransactionAbstract> txn1 = db1.createTransaction();

	transactionUpload(txn1, values);
	EXPECT_TRUE(txn1->commit());
	simpleCheckData(&db1, values);

	if (values.empty())
		return;

	txn1->Delete(values.begin()->first);
	simpleCheckData(&db1, values);
	simpleCheckData(&db2, values, false);
	simpleCheckData(&db3, values, false);

	EXPECT_TRUE(txn1->commit());

	values.pop_front();
	simpleCheckData(&db1, values);
}