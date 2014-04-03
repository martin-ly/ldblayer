#ifndef DATABASE_ABSTRACT_H
#define DATABASE_ABSTRACT_H

#include <leveldb/status.h>

namespace ldblayer 
{

class LayerTransaction;
class LayerIterator;

class DatabaseAbstract 
{
public:
	virtual leveldb::Status Put(const std::string& key, const std::string& value) = 0;
	virtual leveldb::Status Get(const std::string& key, std::string* value) = 0;
	virtual leveldb::Status Del(const std::string& key) = 0;
	
	virtual void close() = 0;
	
	virtual LayerTransaction createTransaction() = 0;
	virtual LayerIterator createIterator() = 0;
};

}	// end of namespace

#endif