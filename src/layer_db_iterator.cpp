#include "layer_db_iterator.h"
#include "database.h"
#include "layer.h"
#include <algorithm>

namespace ldblayer
{

inline bool startsWith(const std::string& prefix, const std::string& toCheck) 
{
	return std::mismatch(prefix.begin(), prefix.end(), toCheck.begin()).first == prefix.end();    
}

LayerDbIterator::LayerDbIterator(Layer* layout)
: activeLayout(layout),
  m_isValid (false)
{
	assert(activeLayout);

	it = activeLayout->db()->createRawIterator();
}

LayerDbIterator::~LayerDbIterator() 
{    
	if (it)
		delete it;
}

void LayerDbIterator::next() 
{   
	if (!m_isValid) {
		return;
	}

	it->Next();

	if (it->Valid()) {
		const std::string key = it->key().ToString();
		if (startsWith(activeLayout->getPrefix(), key)) {
			m_isValid = true;
			return;
		}		
	}

	m_isValid = false;
}

void LayerDbIterator::prev() 
{
	if (!m_isValid) {
		return;
	}

	it->Prev();

	if (it->Valid()) {
		const std::string key = it->key().ToString();
		if (startsWith(activeLayout->getPrefix(), key)) {
			m_isValid = true;
			return;
		}
	}

	m_isValid = false;
}

void LayerDbIterator::seekToFirst() 
{
	it->Seek(activeLayout->getPrefix());

	if (!(m_isValid = it->Valid())) 
		return;

	const std::string key = it->key().ToString();
	m_isValid = startsWith(activeLayout->getPrefix(), key);	
}

void LayerDbIterator::seekToLast() 
{
	Database::prefix_list_t prefixes = activeLayout->db()->getDbPrefixes();   
	auto iter = prefixes.find(activeLayout->getPrefix());

	// database should contain current active layout.
	// if not -> than it is bug and this object was created outside of this database
	assert (iter != prefixes.end());

	// get next database prefix
	++iter;

	// check if current prefix is at the end of database
	if (iter != prefixes.end()) {
		const std::string searchPrefix = *iter;
		it->Seek(searchPrefix);
	} else {
		// current prefix are last, so just navigate to last key;
		it->SeekToLast();
		if (!it->Valid()) {
			m_isValid = false;
			return;
		}
			const std::string& key = it->key().ToString();
		m_isValid = startsWith(activeLayout->getPrefix(), key);
		return;
	}

	if (it->Valid()) {	// we found first key of next database. So, point iter to prev. key
		it->Prev();
			if (!it->Valid()) {	// no prev. key, so no data
			m_isValid = false;
			return;
		}			
	} else {	// we have no keys for next database, so just navigate to last database key
		it->SeekToLast();			
	}

	if (it->Valid()) {
		// check, if this key belongs to current layout.
		// SeekToLast can return key from prev. layout, if this layout doesn't contain any data
		const std::string& key = it->key().ToString();
		m_isValid = startsWith(activeLayout->getPrefix(), key);
		return;
	}

	m_isValid = false;
}

void LayerDbIterator::seekToBegin() 
{
	// layer iterator doesn't support this functions
	throw std::exception();
}

void LayerDbIterator::seekToEnd() 
{
	// layer iterator doesn't support this functions
	throw std::exception();
}

bool LayerDbIterator::isBegin() const
{
	// layer iterator doesn't support this functions
	throw std::exception();
}

bool LayerDbIterator::isEnd() const
{
	// layer iterator doesn't support this functions
	throw std::exception();
}

void LayerDbIterator::seek(const std::string& key) 
{

	std::string seek_key = activeLayout->getPrefix();
	seek_key += key;

	it->Seek(seek_key);
	if (it->Valid()) {
		// if no such key, seek will return next lexicographical key
		const std::string& foundKey = it->key().ToString();
		if (startsWith(activeLayout->getPrefix(), foundKey)) {
			m_isValid = true;
			return;
		}
	}

	m_isValid = false;
}

std::string LayerDbIterator::key() const { 
	std::string key;

	if (m_isValid) {
	assert(it->Valid());		

		std::string ldb_key = it->key().ToString();
		key.reserve(ldb_key.size() - activeLayout->getPrefix().size());
			std::copy(ldb_key.begin() + activeLayout->getPrefix().size(), ldb_key.end(), std::back_inserter(key));
	}

	return key;
}

std::string LayerDbIterator::value() const 
{ 
	std::string value;
	if (m_isValid) {
		assert(it->Valid());
		value = it->value().ToString();
	}

	return value;
}

void LayerDbIterator::reopen() 
{
	if (it) 
		delete it;

	it = activeLayout->db()->createRawIterator();
}

} // end of namespace