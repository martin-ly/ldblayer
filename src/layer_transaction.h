#ifndef LAYER_TRANSACTION
#define LAYER_TRANSACTION

#include <string>
#include <leveldb/write_batch.h>
#include "transaction_abstract.h"

namespace ldblayer
{

class Database;
class Layer;

class LayerTransaction : public TransactionAbstract
{
public:
	LayerTransaction(Layer* layer = nullptr);    
	void setActiveLayer(Layer* layer);
    
	void Put(const std::string& key, const std::string& value);
	void Delete(const std::string& key);
    
	bool commit();
private:
	Database* db;
	Layer* activeLayer;
	leveldb::WriteBatch writeBatch;
};

} // end of namespace

#endif