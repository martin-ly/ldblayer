#include "database.h"
#include "database_transaction.h"
#include "database_iterator.h"
#include "layer.h"
#include "layer_db_iterator.h"

#include "defs.h"

#include <assert.h>

namespace ldblayer
{

using std::string;

Database::Database() noexcept
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

		loadPrefixes();
	} else {
		db = nullptr;
	}

	return status;
}

void Database::close()
{
	if (db) {
		delete db;
		db = nullptr;
	}
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

leveldb::Status Database::registerPrefix(const std::string& prefix) 
{
	leveldb::Status status;
	
	if (prefixs.find(prefix) == prefixs.end()) {
		Layer layer(this, InternalDatabaseLayer, false);
		status = layer.Put(prefix, prefix);

	if (status.ok())
		prefixs.insert(prefix);
	}
	
	return status;
}

leveldb::Status Database::Write(leveldb::WriteBatch* batch) 
{
	assert(db);
	return db->Write(writeOptions, batch);
}

std::unique_ptr<TransactionAbstract> Database::createTransaction()
{
	assert(db);
	return std::unique_ptr<TransactionAbstract> (new DatabaseTransaction(this));
}

std::unique_ptr<IteratorAbstract> Database::createIterator()
{
	assert(db);
	return std::unique_ptr<IteratorAbstract> ( new DatabaseIterator(this) );
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

void Database::loadPrefixes()
{
	Layer layer(this, InternalDatabaseLayer, false);
	std::unique_ptr<IteratorAbstract> iterator = layer.createIterator();
	
	for (iterator->seekToFirst(); iterator->isValid(); iterator->next()) {
		prefixs.insert(iterator->value());		
	}
}


}	// end of namespace