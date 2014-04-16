#ifndef DATABASE_ITERATOR_H
#define DATABASE_ITERATOR_H

#include "layer_db_iterator.h"

namespace ldblayer 
{

class LayerIterator : public IteratorAbstract 
{
public:
	LayerIterator(Layer* layout);

	virtual void seekToFirst();
	virtual void seekToLast();
	virtual void seekToBegin();
	virtual void seekToEnd();
	virtual void seek(const std::string& key);
	virtual void next();
	virtual void prev();
	virtual bool isValid() const;
	
	virtual std::string key() const;
	virtual std::string value() const;
	virtual void reopen();
	
	virtual bool isBegin() const { return start; }
	virtual bool isEnd() const { return end; }

	void save();
	void restore();
	
	void setLayout(Layer* layout);

private:
	LayerDbIterator iterator;
	bool start;
	bool end;
	std::string savedKey;
};

} // end of namespace

#endif // end of include guard