#include "database_iterator.h"

namespace ldblayer 
{

DatabaseIterator::DatabaseIterator(Database* db, Layer* layout)
 : iterator(db, layout)
{
	seekToFirst();
}

void DatabaseIterator::seekToFirst() 
{	
	iterator.seekToFirst();

	start = false;
	end = !iterator.isValid();
}

void DatabaseIterator::seekToLast() 
{
	iterator.seekToLast();	
	
	start = !iterator.isValid();
	end = false;
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

void DatabaseIterator::seek(const std::string& key) 
{	
	iterator.seek(key);
	start = false;
	end = !iterator.isValid();
}

void DatabaseIterator::next() 
{
	assert( !end );
	if (start) {
		seekToFirst();
		return;
	}

	if (iterator.isValid()) {
		iterator.next();
		
		if (!iterator.isValid()) {
			end = true;
		}
	} else {
		end = true;
	}
}

void DatabaseIterator::prev() 
{
	assert( !start );
	if (end) {
		seekToLast();
		return;
	}

	if (iterator.isValid()) {
		iterator.prev();

		if (!iterator.isValid()) {
			start = true;
		}
	} else {
		start = true;
	}
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