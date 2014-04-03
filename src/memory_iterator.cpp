#include "memory_iterator.h"
#include <assert.h>

namespace ldblayer 
{

MemoryIterator::MemoryIterator(const list_t* buf)
: buffer(buf)
{
	assert(buffer);
	seekToFirst();
}

MemoryIterator::MemoryIterator(const list_t* buf, const iterator_t& iter)
: buffer(buf),
  iterator(iter)
{
	seekToFirst();
}

MemoryIterator& MemoryIterator::operator= (iterator_t iter) 
{
	iterator = iter;
	start = false;
	
	return *this;
}

bool MemoryIterator::seekToFirst() 
{
	iterator = buffer->begin();
	start = false;
	
	return isValid();
}

bool MemoryIterator::seekToLast() 
{	
	start = buffer->empty();
	if (!start) iterator = --buffer->end();
	
	return isValid();
}

bool MemoryIterator::seek(const std::string& key) 
{
	iterator = buffer->find(key);
	start = false;

	return isValid();
}

bool MemoryIterator::next() 
{
	if (isEnd())
		return false;

	if (start)
		start = false;
	else
		++iterator;

	return isValid();
}

bool MemoryIterator::prev() 
{
	if (isBegin())
		return false;
	
	if (iterator == buffer->begin())	
		seekToBegin();
	else	
		--iterator;
	
	return isValid();
}

bool MemoryIterator::isValid() const 
{
	return !(isBegin() || isEnd());
}

std::string MemoryIterator::key() const 
{
	std::string current_key;
	
	if (isValid()) {
		current_key = iterator->first;
	}
	
	return current_key;
}

std::string MemoryIterator::value() const 
{
	std::string current_value;
	
	if (isValid()) {
		current_value = iterator->second;
	}

	return current_value;
}

bool MemoryIterator::isBegin() const
{
	return start;
}

bool MemoryIterator::isEnd() const
{
	return !isBegin() && iterator == buffer->end();
}

void MemoryIterator::seekToBegin() 
{
	start = true;
	iterator = buffer->begin();
}

void MemoryIterator::seekToEnd() 
{
	iterator = buffer->end();
}


}