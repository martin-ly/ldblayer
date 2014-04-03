#include "layer.h"
#include "database.h"
#include "LevelDatabaseTransaction.h"
#include "LevelDatabaseIterator.h"

namespace ldblayer 
{

using std::string;

Layer::Layer() 
: m_db(nullptr)
{
}

Layer::Layer(Database* levelDatabase, const std::string& layerName)
: m_db(levelDatabase), prefix(layerName)
{
	m_db->registerPrefix(prefix);
}

void Layer::open(Database* levelDatabase, const std::string& layerName)  noexcept
{
	m_db = levelDatabase;
	prefix = layerName;

	m_db->registerPrefix(prefix);
}

void Layer::close()
{
	m_db = nullptr;
	prefix.clear();
}

leveldb::Status Layer::Put(const std::string& key, const std::string& value) 
{
	assert(m_db);
	string put_key = prefix + key;
	return m_db->Put(put_key, value);
}

leveldb::Status Layer::Get(const std::string& key, std::string* value) 
{
	assert(m_db);
	string get_key = prefix + key;
	return m_db->Get(get_key, value);
}

leveldb::Status Layer::Del(const std::string& key) 
{
	assert(m_db);
	string get_key = prefix + key;
	return m_db->Del(key);
}

LevelDatabaseTransaction Layer::createTransaction() 
{
	return LevelDatabaseTransaction(m_db, this);
}

LevelDatabaseIterator Layer::createIterator()
{
	assert(m_db);

	LevelDatabaseIterator iterator = m_db->createIterator();
	iterator.setLayout(this);	
	
	return iterator;
}

} // end of namespace
