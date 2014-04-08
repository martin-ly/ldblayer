#include "direct_db.h"
#include "layer_transaction.h"

using namespace test;
INSTANTIATE_TEST_CASE_P(simpledata, Transaction, ::testing::Values(no_keys, one_key, simple_keys));

void Transaction::transactionUpload()
{
	std::unique_ptr<TransactionAbstract> transaction = db.createTransaction();
    keyval values = GetParam();
    for (auto i = values.rbegin(); i != values.rend(); i++) {
        transaction->Put(i->first, i->second);
    }
    
    EXPECT_TRUE(transaction->commit());
}

TEST_P(Transaction, put) 
{    
    transactionUpload();    
    simpleCheckData();
}

TEST_P(Transaction, del) 
{
    transactionUpload();
    
	std::unique_ptr<TransactionAbstract> transaction = db.createTransaction();
    keyval values = GetParam();
    for (auto row: values) {
        transaction->Put(row.first, row.second);
    }
    
    EXPECT_TRUE(transaction->commit());
    
    simpleCheckData();
}