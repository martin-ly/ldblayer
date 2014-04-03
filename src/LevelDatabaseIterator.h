#ifndef LEVELDBITERATOR_H
#define LEVELDBITERATOR_H

#include <leveldb/iterator.h>

namespace ldblayer
{

class Database;
class Layer;

class LevelDatabaseIterator
{
public:
	LevelDatabaseIterator(Database* db, Layer* layout = nullptr);
	~LevelDatabaseIterator();
    
	bool seekToFirst();
	bool seekToLast();
	bool seek(const std::string& key);
	bool next();
	bool prev();
	bool isValid() const noexcept { return m_isValid; }

	std::string operator* () const { return value(); }
	std::string key() const;
	std::string value() const;
	void setLayout(Layer* layout) { activeLayout = layout; }    
    
	void reopen();
private:    
	Database* database;
	Layer* activeLayout;
	leveldb::Iterator* it;    
	bool m_isValid;
};

}	// end of namespace

#endif