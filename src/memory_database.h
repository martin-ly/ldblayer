#ifndef MEMORY_DATABASE_H
#define MEMORY_DATABASE_H

#include "database_abstract.h"
#include <map>
#include <string>

namespace ldblayer 
{

class MemoryDatabase : public DatabaseAbstract
{
public:
	virtual leveldb::Status Put(const std::string& key, const std::string& value);
	virtual leveldb::Status Get(const std::string& key, std::string* value);
	virtual leveldb::Status Del(const std::string& key);

	virtual void close() ;

	virtual std::unique_ptr<TransactionAbstract> createTransaction();
	virtual LayerIterator createIterator();

	void merge(std::map<std::string, std::string> txn);
private:
	std::map<std::string, std::string> buffer;
};

}

#endif