#ifndef DATABASE_ITERATOR_H
#define DATABASE_ITERATOR_H

#include "iterator_abstract.h"
#include <leveldb/iterator.h>

namespace ldblayer 
{

class Database;

class DatabaseIterator : public IteratorAbstract 
{
public:
	DatabaseIterator(Database* database);
	~DatabaseIterator();

	virtual void seekToFirst();
	virtual void seekToLast();
	virtual void seekToBegin();
	virtual void seekToEnd();
	virtual void seek(const std::string& key);
	virtual void next();
	virtual void prev();
	virtual bool isValid() const;
	
	virtual std::string key() const;
	virtual std::string value() const;

	virtual bool isBegin() const;
	virtual bool isEnd() const;

	virtual void reopen();
private:
	Database* db;
	leveldb::Iterator* it;
	
	bool start;
	bool end;
	leveldb::Slice saveKey;
};

}

#endif