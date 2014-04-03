#ifndef LAYER_H
#define LAYER_H

#include "database_abstract.h"

namespace ldblayer
{

class Database;

class Layer : public DatabaseAbstract
{
public:
	Layer();
	Layer(Database* levelDatabase, const std::string& layerName);

	void open(Database* levelDatabase, const std::string& layerName) noexcept;
	void close();
    
	virtual leveldb::Status Put(const std::string& key, const std::string& value);
	virtual leveldb::Status Get(const std::string& key, std::string* value);
	virtual leveldb::Status Del(const std::string& key);
	
	virtual LayerTransaction createTransaction();
	virtual LevelDatabaseIterator createIterator();

	const std::string& getPrefix() const noexcept { return prefix; }   

	Database* db() const noexcept { return m_db; }
    
private:
	Database* m_db;
	std::string prefix;
};

} // end of namespace

#endif