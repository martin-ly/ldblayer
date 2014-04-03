#ifndef LAYER_TRANSACTION
#define LAYER_TRANSACTION

#include <string>
#include <leveldb/write_batch.h>

namespace ldblayer
{

class Database;
class Layer;

class LayerTransaction 
{
public:
	LayerTransaction(Database* database, Layer* layer = nullptr);    
	void setActiveLayer(Layer* layer);
    
	void Put(const std::string& key, const std::string& value);
	void Delete(const std::string& key);
    
	leveldb::Status commit();
private:
	Database* db;
	Layer* activeLayer;
	leveldb::WriteBatch writeBatch;
};

} // end of namespace

#endif