#include "layer_transaction.h"
#include "database.h"
#include "layer.h"

namespace ldblayer
{

LayerTransaction::LayerTransaction(Layer* layer)
:activeLayer(layer)
{
	assert(layer);
}

void LayerTransaction::setActiveLayer(Layer* layer)
{
	assert(layer);
	activeLayer = layer;
}

void LayerTransaction::Put(const std::string& key, const std::string& value)
{
	std::string put_key = activeLayer->getPrefix() + key;
	writeBatch.Put(put_key, value);	
}

void LayerTransaction::Delete(const std::string& key)
{
	std::string del_key = activeLayer->getPrefix() + key;
	writeBatch.Delete(del_key);
}

bool LayerTransaction::commit() 
{
	leveldb::Status s;
	s = activeLayer->db()->Write(&writeBatch);
	if (s.ok())
		writeBatch.Clear();

	return s.ok();
}

} // end of namespace