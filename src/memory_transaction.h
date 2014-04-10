#ifndef MEMORY_TRANSACTION_H
#define MEMORY_TRANSACTION_H

#include "transaction_abstract.h"
#include <map>

namespace ldblayer
{

class MemoryDatabase;

class MemoryTransaction : public TransactionAbstract
{
public:
	MemoryTransaction(MemoryDatabase* database);
	
	virtual void Put(const std::string& key, const std::string& value);
	virtual void Delete(const std::string& key);
	
	virtual bool commit();
private:
	std::map<std::string, std::string> txn_data;
	MemoryDatabase* db;
};

} // end of namespace

#endif