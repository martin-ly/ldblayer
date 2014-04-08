#include "database_transaction.h"
#include "database.h"

namespace ldblayer
{

DatabaseTransaction::DatabaseTransaction(Database* database)
: db(database)
{
	assert(db);
}

void DatabaseTransaction::Put(const std::string& key, const std::string& value) 
{
	writeBatch.Put(key, value);
}

void DatabaseTransaction::Delete(const std::string& key) 
{
	writeBatch.Delete(key);
}

bool DatabaseTransaction::commit() 
{
	leveldb::Status s = db->Write(&writeBatch);
	if (s.ok()) {
		writeBatch.Clear();
	}
	
	status = s;
	
	return s.ok();
}

}