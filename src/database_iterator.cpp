#include "database_iterator.h"
#include "database.h"

namespace ldblayer 
{

DatabaseIterator::DatabaseIterator(Database* database)
: db(database)
{
	assert(db);
	it = db->createRawIterator();
	
	seekToFirst();
}

DatabaseIterator::~DatabaseIterator()
{
	delete it;
}

void DatabaseIterator::seekToFirst()
{
	it->SeekToFirst();

	start = false;
	end = !it->Valid();
}

void DatabaseIterator::seekToLast()
{
	it->SeekToLast();

	start = !it->Valid();
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
	it->Seek(key);
	start = false;
	end = !it->Valid();
}

void DatabaseIterator::next()
{
	assert( !end );
	if (start) {
		seekToFirst();
		return;
	}
	
	if (it->Valid()) {
		it->Next();
		
		if (!it->Valid()) {
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
	
	if (it->Valid()) {
		it->Prev();
		
		if (!it->Valid()) {
			start = true;
		}
	} else {
		start = true;
	}
}

bool DatabaseIterator::isValid() const 
{
	return (start || end) && it->Valid();
}

std::string DatabaseIterator::key() const 
{
	assert(it->Valid());

	return it->key().ToString();
}

std::string DatabaseIterator::value() const
{
	assert(it->Valid());

	return it->value().ToString();
}

bool DatabaseIterator::isBegin() const 
{
	return start;
}

bool DatabaseIterator::isEnd() const
{
	return end;
}

void DatabaseIterator::reopen()
{
	saveKey = it->key();

	delete it;
	it = db->createRawIterator();

	it->Seek(saveKey);
	start = false;
	end = !it->Valid();
}

}