#ifndef MEMORY_ITERATOR_H
#define MEMORY_ITERATOR_H

#include "iterator_abstract.h"
#include "memory_database.h"
#include <map>

namespace ldblayer
{
	
class MemoryDatabase;

class MemoryIterator : public IteratorAbstract 
{
public:
	MemoryIterator(MemoryDatabase* db);

	virtual void seekToFirst();
	virtual void seekToLast();
	virtual void seek(const std::string& key);
	virtual void next();
	virtual void prev();
	virtual bool isValid() const;
	
	virtual void seekToBegin();
	virtual void seekToEnd();
	
	bool isBegin() const;
	bool isEnd() const;
	
	virtual std::string key() const;
	virtual std::string value() const;

	void reopen();
private:
	MemoryDatabase* db;
	MemoryDatabase::list_t::const_iterator iterator;
	bool start;
};

} // end of namespace

#endif