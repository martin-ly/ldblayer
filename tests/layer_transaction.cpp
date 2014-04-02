#include "layer_db.h"
#include "LevelDatabaseTransaction.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(Transaction, LayerTransaction, ::testing::Values(no_keys, one_key, simple_keys));

TEST_P(LayerTransaction, put) 
{    
}

TEST_P(LayerTransaction, get) 
{    
}

TEST_P(LayerTransaction, del) 
{    
}