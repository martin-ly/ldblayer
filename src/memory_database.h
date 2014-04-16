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
	using list_t = std::map<std::string, std::string>;
	
	virtual leveldb::Status Put(const std::string& key, const std::string& value);
	virtual leveldb::Status Get(const std::string& key, std::string* value);
	virtual leveldb::Status Del(const std::string& key);

	virtual void close() ;

	virtual std::unique_ptr<TransactionAbstract> createTransaction();
	virtual std::unique_ptr<IteratorAbstract> createIterator();

	void merge(std::map<std::string, std::string> txn);

	const list_t& getBuffer() const noexcept { return buffer; }
private:
	std::map<std::string, std::string> buffer;
};

}

#endif