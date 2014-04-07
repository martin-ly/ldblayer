#ifndef MEMORY_ITERATOR_H
#define MEMORY_ITERATOR_H

#include "iterator_abstract.h"
#include <map>

namespace ldblayer
{

class MemoryIterator : public IteratorAbstract 
{
public:
	using list_t = std::map<std::string, std::string> ;
	using iterator_t = list_t::const_iterator;
	
	MemoryIterator() = delete;
	MemoryIterator(const list_t* buf);
	MemoryIterator(const list_t* buf, const iterator_t& iter);
	
	MemoryIterator& operator= (iterator_t iter);

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
	
private:
	const list_t* buffer;
	iterator_t iterator;
	bool start;
};

} // end of namespace

#endif