#include "memory_transaction.h"
#include "memory_database.h"

namespace ldblayer 
{

MemoryTransaction::MemoryTransaction(MemoryDatabase* database)
: db(database)
{
}


void MemoryTransaction::Put(const std::string& key, const std::string& value)
{
	txn_data[key] = value;
}

void MemoryTransaction::Delete(const std::string& key)
{
	txn_data.erase(key);
}

bool MemoryTransaction::commit()
{
	db->merge(txn_data);
	txn_data.clear();
	
	return true;
}

}