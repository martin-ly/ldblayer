#include "layer_transaction.h"
#include "database.h"
#include "layer.h"

namespace ldblayer
{

	LayerTransaction::LayerTransaction(Database* database, Layer* layer)
: db(database),
  activeLayer(layer)
{
}

void LayerTransaction::setActiveLayer(Layer* layer)
{
	activeLayer = layer;
}

void LayerTransaction::Put(const std::string& key, const std::string& value)
{
	std::string put_key;
	if (activeLayer) {
		put_key = activeLayer->getPrefix();
	}

	put_key += key;
	writeBatch.Put(put_key, value);	
}

void LayerTransaction::Delete(const std::string& key)
{
	std::string del_key;
	if (activeLayer) {
		del_key = activeLayer->getPrefix();
	}

	del_key += key;
	writeBatch.Delete(del_key);
}

leveldb::Status LayerTransaction::commit() 
{
	leveldb::Status s;
	s = db->Write(&writeBatch);
	if (s.ok())
		writeBatch.Clear();

	return s;
}

} // end of namespace