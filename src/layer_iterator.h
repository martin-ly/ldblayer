#ifndef LAYER_ITERATOR_H
#define LAYER_ITERATOR_H

#include <leveldb/iterator.h>

#include "iterator_abstract.h"

namespace ldblayer
{

class Database;
class Layer;

class LayerIterator : public IteratorAbstract
{
public:
	LayerIterator(Database* db, Layer* layout = nullptr);
	~LayerIterator();
    
	virtual bool seekToFirst();
	virtual bool seekToLast();
	virtual bool seek(const std::string& key);
	virtual bool next();
	virtual bool prev();
	virtual bool isValid() const noexcept { return m_isValid; }
	
	virtual std::string key() const;
	virtual std::string value() const;
	
	void reopen();

	std::string operator* () const { return value(); }	
	void setLayout(Layer* layout) { activeLayout = layout; }
private:    
	Database* database;
	Layer* activeLayout;
	leveldb::Iterator* it;    
	bool m_isValid;
};

}	// end of namespace

#endif