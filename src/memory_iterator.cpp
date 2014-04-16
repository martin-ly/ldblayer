#include "memory_iterator.h"
#include "memory_database.h"
#include <assert.h>

namespace ldblayer 
{

MemoryIterator::MemoryIterator(MemoryDatabase* database)
: db(database)
{
	assert(db);
	seekToFirst();
}

/*MemoryIterator& MemoryIterator::operator= (iterator_t iter) 
{
	iterator = iter;
	start = false;
	
	return *this;
}*/

void MemoryIterator::seekToFirst() 
{
	iterator = db->getBuffer().begin();
	start = false;
}

void MemoryIterator::seekToLast() 
{	
	start = db->getBuffer().empty();
	if (!start) iterator = --db->getBuffer().end();
}

void MemoryIterator::seek(const std::string& key) 
{
	iterator = db->getBuffer().lower_bound(key);
	start = false;
}

void MemoryIterator::next() 
{
	if (isEnd())
		return;

	if (start)
		start = false;
	else
		++iterator;
}

void MemoryIterator::prev() 
{
	if (isBegin())
		return;
	
	if (iterator == db->getBuffer().begin())	
		seekToBegin();
	else	
		--iterator;
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
	return !isBegin() && iterator == db->getBuffer().end();
}

void MemoryIterator::seekToBegin() 
{
	start = true;
	iterator = db->getBuffer().begin();
}

void MemoryIterator::seekToEnd() 
{
	iterator = db->getBuffer().end();
}

void MemoryIterator::reopen() 
{
	std::string key = iterator->first;
	iterator = db->getBuffer().find(key);
}

}