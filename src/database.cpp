#include "database.h"
#include "layer.h"
#include "LevelDatabaseTransaction.h"
#include "LevelDatabaseIterator.h"

#include <assert.h>

namespace ldblayer
{

using std::string;

Database::Database()  noexcept
 : db(nullptr)
{
}

Database::Database(const leveldb::Options& options, const std::string& dbpath) 
 : db(nullptr)
{    
	assert(open(options, path).ok());
}

Database::~Database()
{
	if (db)
		delete db;
}

leveldb::Status Database::open(const leveldb::Options& options, const std::string& path) 
{
	if (db)
		delete db;

	leveldb::Status status = leveldb::DB::Open(options, path.c_str(), &db);
	if (status.ok()) {
		this->path = path;        
	} else {
		db = nullptr;
	}

	return status;
}

leveldb::Status Database::Put(const string& key, const string& value)
{
	assert(db);
	return db->Put(writeOptions, key, value);
}

leveldb::Status Database::Del(const std::string& key) 
{
	assert(db);
	return db->Delete(writeOptions, key);
}

leveldb::Status Database::Get(const string& key, std::string* value)
{
	assert(db);
	return db->Get(readOptions, key, value);
}

Layer Database::getLayer(const std::string& layerName) 
{    
	Layer layer(this, layerName);
	registerPrefix(layer.getPrefix());
	return layer;
}

void Database::registerPrefix(const std::string& prefix) 
{
	prefixs.insert(prefix);
}

leveldb::Status Database::Write(leveldb::WriteBatch* batch) 
{
	assert(db);
	return db->Write(writeOptions, batch);
}

LevelDatabaseTransaction Database::createTransaction()
{
	assert(db);
	return LevelDatabaseTransaction(this);
}

LevelDatabaseIterator Database::createIterator()
{
	assert(db);
	return LevelDatabaseIterator(this);
}

leveldb::Iterator* Database::createRawIterator() 
{
	assert(db);
	leveldb::Iterator* it = db->NewIterator(readOptions);
	return it;
}

leveldb::Status Database::destroy() 
{
	assert(!path.empty());

	if (db) {
		delete db;
		db = nullptr;
	}

	return leveldb::DestroyDB(path, leveldb::Options());
}

}	// end of namespace