#include "LevelDatabaseLayer.h"
#include "LevelDatabase.h"
#include "LevelDatabaseTransaction.h"
#include "LevelDatabaseIterator.h"

namespace ldblayer 
{

using std::string;

LevelDatabaseLayer::LevelDatabaseLayer() 
: m_db(nullptr)
{
}

LevelDatabaseLayer::LevelDatabaseLayer(LevelDatabase* levelDatabase, const std::string& layerName)
: m_db(levelDatabase), prefix(layerName)
{
	m_db->registerPrefix(prefix);
}

void LevelDatabaseLayer::open(LevelDatabase* levelDatabase, const std::string& layerName)  noexcept
{
	m_db = levelDatabase;
	prefix = layerName;

	m_db->registerPrefix(prefix);
}

void LevelDatabaseLayer::close()
{
	m_db = nullptr;
	prefix.clear();
}

leveldb::Status LevelDatabaseLayer::Put(const std::string& key, const std::string& value) 
{
	assert(m_db);
	string put_key = prefix + key;
	return m_db->Put(put_key, value);
}

leveldb::Status LevelDatabaseLayer::Get(const std::string& key, std::string* value) 
{
	assert(m_db);
	string get_key = prefix + key;
	return m_db->Get(get_key, value);
}

leveldb::Status LevelDatabaseLayer::Del(const std::string& key) 
{
	assert(m_db);
	string get_key = prefix + key;
	return m_db->Del(key);
}

LevelDatabaseTransaction LevelDatabaseLayer::createTransaction() 
{
	return LevelDatabaseTransaction(m_db, this);
}

LevelDatabaseIterator LevelDatabaseLayer::createIterator()
{
	assert(m_db);

	LevelDatabaseIterator iterator = m_db->createIterator();
	iterator.setLayout(this);	
	
	return iterator;
}

} // end of namespace
