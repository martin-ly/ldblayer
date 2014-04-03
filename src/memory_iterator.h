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

	virtual bool seekToFirst();
	virtual bool seekToLast();
	virtual bool seek(const std::string& key);
	virtual bool next();
	virtual bool prev();
	virtual bool isValid() const;
	
	void seekToBegin();
	void seekToEnd();
	
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