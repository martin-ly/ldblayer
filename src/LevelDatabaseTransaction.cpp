#include "LevelDatabaseTransaction.h"
#include "LevelDatabaseLayer.h"
#include "LevelDatabase.h"

namespace ldblayer
{

LevelDatabaseTransaction::LevelDatabaseTransaction(LevelDatabase* database, LevelDatabaseLayer* layer)
: db(database),
  activeLayer(layer)
{
}

void LevelDatabaseTransaction::setActiveLayer(LevelDatabaseLayer* layer)
{
	activeLayer = layer;
}

void LevelDatabaseTransaction::Put(const std::string& key, const std::string& value)
{
	std::string put_key;
	if (activeLayer) {
		put_key = activeLayer->getPrefix();
	}

	put_key += key;
	writeBatch.Put(put_key, value);	
}

void LevelDatabaseTransaction::Delete(const std::string& key)
{
	std::string del_key;
	if (activeLayer) {
		del_key = activeLayer->getPrefix();
	}

	del_key += key;
	writeBatch.Delete(del_key);
}

leveldb::Status LevelDatabaseTransaction::commit() 
{
	leveldb::Status s;
	s = db->Write(&writeBatch);
	if (s.ok())
		writeBatch.Clear();

	return s;
}

} // end of namespace