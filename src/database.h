#ifndef DATABASE_H
#define DATABASE_H

#include <set>
#include <leveldb/db.h>
#include "database_abstract.h"

namespace ldblayer
{

class Layer;
class LevelDatabaseIterator;

class Database : public DatabaseAbstract
{
public:
	using prefix_list_t = std::set<std::string>;

	Database() noexcept;
	Database(const leveldb::Options& options, const std::string& path);
	Database(const Database&) = delete;
	~Database();

	Database& operator=(const Database&) = delete;

	leveldb::Status open(const leveldb::Options& options, const std::string& path);

	leveldb::Status Put(const std::string& key, const std::string& value);
	leveldb::Status Del(const std::string& key);
	leveldb::Status Get(const std::string& key, std::string* value);
	leveldb::Status Write(leveldb::WriteBatch* batch);

	Layer getLayer(const std::string& layerName);
	void registerPrefix(const std::string& layer);

	virtual LayerTransaction createTransaction();
	virtual LevelDatabaseIterator createIterator();

	leveldb::Iterator* createRawIterator();
	const prefix_list_t& getDbPrefixes() const noexcept { return prefixs; }

	leveldb::Status destroy();

	leveldb::DB* getDb() noexcept { return db; }

private:
	leveldb::DB* db;
	leveldb::ReadOptions readOptions;
	leveldb::WriteOptions writeOptions;

	std::string path;

	prefix_list_t prefixs;
};

}	// end of namespace

#endif	// end of include guard