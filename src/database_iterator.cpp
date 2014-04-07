#include "database_iterator.h"

namespace ldblayer 
{

DatabaseIterator::DatabaseIterator(Database* db, Layer* layout)
 : iterator(db, layout)
{
	seekToFirst();
}

bool DatabaseIterator::seekToFirst() 
{	
	iterator.seekToFirst();
	
	start = false;
	end = !iterator.isValid();
	
	return true;
}

bool DatabaseIterator::seekToLast() 
{	
	bool result;
	result = iterator.seekToLast();	
	
	start = !result;
	end = false;

	return result;
}

void DatabaseIterator::seekToBegin() 
{
	start = true;
	end = false;	
}

void DatabaseIterator::seekToEnd() 
{
	start = false;
	end = true;
}

bool DatabaseIterator::seek(const std::string& key) 
{	
	iterator.seek(key);
	start = false;
	end = !iterator.isValid();

	return true;
}

bool DatabaseIterator::next() 
{
	assert( !end );
	if (start) {
		seekToFirst();
		return true;
	}

	if (iterator.isValid()) {
		iterator.next();
		
		if (!iterator.isValid()) {
			end = true;
		}

	} else {
		end = true;
	}

	return true;
}

bool DatabaseIterator::prev() 
{
	assert( !start );
	if (end) {
		seekToLast();
		return true;
	}

	if (iterator.isValid()) {
		iterator.prev();

		if (!iterator.isValid()) {
			start = true;
		}
	} else {
		start = true;
	}

	return true;
}

bool DatabaseIterator::isValid() const 
{
	return !(start || end);
}

std::string DatabaseIterator::key() const 
{
	assert(iterator.isValid());	// check if we really can get key
	return iterator.key();
}

std::string DatabaseIterator::value() const 
{
	assert(iterator.isValid());	// check if we really can get value
	return iterator.value();
}

void DatabaseIterator::save()
{
	if (iterator.isValid()) {
		savedKey = iterator.key();		
	} else {
		savedKey.clear();
	}
}

void DatabaseIterator::restore()
{
	iterator.reopen();
	
	if (!savedKey.empty()) {
		seek(savedKey);
	}
}

} // end of namespace