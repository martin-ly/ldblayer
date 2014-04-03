#include "layer_iterator.h"
#include "database.h"
#include "layer.h"
#include <algorithm>

namespace ldblayer
{

inline bool startsWith(const std::string& prefix, const std::string& toCheck) 
{
	return std::mismatch(prefix.begin(), prefix.end(), toCheck.begin()).first == prefix.end();    
}

LayerIterator::LayerIterator(Database* db, Layer* layout)
: database(db),
  activeLayout(layout),
  m_isValid (false)
{
	it = db->createRawIterator();
}

LayerIterator::~LayerIterator() 
{    
	if (it)
		delete it;
}

bool LayerIterator::next() 
{   
	if (!m_isValid) {
		return false;
	}

	it->Next();

	if (it->Valid()) {
		if (activeLayout) {
			const std::string key = it->key().ToString();
			if (startsWith(activeLayout->getPrefix(), key)) {
				return m_isValid = true;
			}
		} else {
			return m_isValid = true;
		}
	}

	return m_isValid = false;
}

bool LayerIterator::prev() 
{
	if (!m_isValid) {
		return false;
	}

	it->Prev();

	if (it->Valid()) {
		if (activeLayout) {
			const std::string key = it->key().ToString();
			if (startsWith(activeLayout->getPrefix(), key)) {
				return m_isValid = true;
			}
		} else {
			return m_isValid = true;
		}
	}

	return m_isValid = false;    
}

bool LayerIterator::seekToFirst() 
{
	if (activeLayout) {
		it->Seek(activeLayout->getPrefix());

		if (!(m_isValid = it->Valid())) 
			return false;

		const std::string key = it->key().ToString();
		if (activeLayout) {
			if (startsWith(activeLayout->getPrefix(), key)) {
				return m_isValid = true;
			} else {
				return m_isValid = false;
			}
		}
	} else {
		it->SeekToFirst();
	}

	return m_isValid = it->Valid();
}

bool LayerIterator::seekToLast() 
{
	if (activeLayout) {
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
			if (!it->Valid())
				return m_isValid = false;

			const std::string& key = it->key().ToString();
			return m_isValid = startsWith(activeLayout->getPrefix(), key);
		}

		if (it->Valid()) {	// we found first key of next database. So, point iter to prev. key
			it->Prev();

			if (!it->Valid()) {	// no prev. key, so no data
				return (m_isValid = false);
			}			
		} else {	// we have no keys for next database, so just navigate to last database key
			it->SeekToLast();			
		}

		if (it->Valid()) {
			// check, if this key belongs to current layout.
			// SeekToLast can return key from prev. layout, if this layout doesn't contain any data
			const std::string& key = it->key().ToString();
			return m_isValid = startsWith(activeLayout->getPrefix(), key);
		}
		
	} else {   
		it->SeekToLast();
		return m_isValid = it->Valid();
	}

	return m_isValid = false;
}

bool LayerIterator::seek(const std::string& key) 
{
	if (activeLayout) {
		std::string seek_key = activeLayout->getPrefix();
		seek_key += key;

		it->Seek(seek_key);
		if (it->Valid()) {
			// if no such key, seek will return next lexicographical key
			const std::string& foundKey = it->key().ToString();

			if (startsWith(activeLayout->getPrefix(), foundKey)) {
				return m_isValid = true;
			}
		}
	} else {
		it->Seek(key);
		return m_isValid = it->Valid();
	}

	return m_isValid = false;
}

std::string LayerIterator::key() const { 
	std::string key;
	
	if (m_isValid) {
		assert(it->Valid());		
		if (activeLayout) {
			std::string ldb_key = it->key().ToString();
			key.reserve(ldb_key.size() - activeLayout->getPrefix().size());

			std::copy(ldb_key.begin() + activeLayout->getPrefix().size(), ldb_key.end(), std::back_inserter(key));
		} else {
			key = it->key().ToString();
		}
	}

	return key;
}

std::string LayerIterator::value() const 
{ 
	std::string value;
	if (m_isValid) {
		assert(it->Valid());
		value = it->value().ToString();
	}

	return value;
}

void LayerIterator::reopen() 
{
	if (it) 
		delete it;

	it = database->createRawIterator();
}

} // end of namespace