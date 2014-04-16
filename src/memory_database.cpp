#include "memory_database.h"
#include "layer_iterator.h"
#include "memory_transaction.h"
#include "memory_iterator.h"

namespace ldblayer
{

leveldb::Status MemoryDatabase::Put(const std::string& key, const std::string& value)
{
	buffer[key] = value;

	return leveldb::Status();
}

leveldb::Status MemoryDatabase::Get(const std::string& key, std::string* value)
{
	auto i = buffer.find(key);
	if (i != buffer.end()) {
		*value = i->second;
		leveldb::Status();	// return true
	}

	return leveldb::Status();	// return false
}

leveldb::Status MemoryDatabase::Del(const std::string& key)
{
	size_t size = buffer.erase(key);

	// return true
	return leveldb::Status();
}

void MemoryDatabase::close()
{
	buffer.clear();
}

std::unique_ptr<IteratorAbstract> MemoryDatabase::createIterator()
{
	return std::unique_ptr< IteratorAbstract > { new MemoryIterator(this) };
}

std::unique_ptr< TransactionAbstract > MemoryDatabase::createTransaction()
{
	return std::unique_ptr< TransactionAbstract > { new MemoryTransaction(this) };
}

void MemoryDatabase::merge(std::map< std::string, std::string > txn)
{
	buffer.insert(txn.begin(), txn.end());
}


}