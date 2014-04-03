#include "layer.h"
#include "database.h"
#include "layer_transaction.h"
#include "layer_iterator.h"

namespace ldblayer 
{

using std::string;

Layer::Layer() 
: m_db(nullptr)
{
}

Layer::Layer(Database* levelDatabase, const std::string& layerName, bool registerPrefix)
: m_db(levelDatabase), prefix(layerName)
{
	if (registerPrefix)
		m_db->registerPrefix(prefix);
}

void Layer::open(Database* levelDatabase, const std::string& layerName, bool registerPrefix)
{
	m_db = levelDatabase;
	prefix = layerName;

	if (registerPrefix) {
		m_db->registerPrefix(prefix);
	}
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

LayerTransaction Layer::createTransaction() 
{
	return LayerTransaction(m_db, this);
}

LayerIterator Layer::createIterator()
{
	assert(m_db);

	LayerIterator iterator = m_db->createIterator();
	iterator.setLayout(this);	
	
	return iterator;
}

} // end of namespace
