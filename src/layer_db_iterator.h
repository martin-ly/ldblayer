#ifndef LAYER_ITERATOR_H
#define LAYER_ITERATOR_H

#include <leveldb/iterator.h>

#include "iterator_abstract.h"

namespace ldblayer
{

class Database;
class Layer;

/**
 * LayerIterator provide same functionality as leveldb::Iterator,
 * but know about existing prefixes in database and navigates only
 * in prefix database
 * Use this class in situation, where you need the same behaviour as
 * leveldb::Iterator class
 */
class LayerDbIterator : public IteratorAbstract
{
public:
	LayerDbIterator(Layer* layout);
	~LayerDbIterator();
    
	virtual void seekToFirst();
	virtual void seekToLast();
	virtual void seek(const std::string& key);
	virtual void next();
	virtual void prev();
	virtual bool isValid() const noexcept { return m_isValid; }
	
	/**
	 * Doesn't supported with this class
	 * @exception std::exception
	 */
	virtual void seekToBegin();

	/**
	 * Doesn't supported with this class
	 * @exception std::exception
	 */
	virtual void seekToEnd();

	/**
	 * Doesn't supported with this class
	 * @exception std::exception
	 */
	virtual bool isBegin() const;
	
	/**
	 * Doesn't supported with this class
	 * @exception std::exception
	 */
	virtual bool isEnd() const;
	
	virtual std::string key() const;
	virtual std::string value() const;

	void reopen();

	std::string operator* () const { return value(); }
	void setLayout(Layer* layout) { activeLayout = layout; }
private:
	Layer* activeLayout;
	leveldb::Iterator* it;    
	bool m_isValid;
};

}	// end of namespace

#endif