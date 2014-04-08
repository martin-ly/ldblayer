#ifndef DATABASE_TRANSACTION_H
#define DATABASE_TRANSACTION_H

#include "transaction_abstract.h"
#include <leveldb/write_batch.h>

namespace ldblayer 
{

class Database;

class DatabaseTransaction : public TransactionAbstract
{
public:
	DatabaseTransaction(Database* database);

	void Put(const std::string& key, const std::string& value);
	void Delete(const std::string& key);

	bool commit();
	
private:
	Database* db;
	leveldb::WriteBatch writeBatch;
	leveldb::Status status;
};

} // end of namespace

#endif