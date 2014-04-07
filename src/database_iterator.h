#ifndef DATABASE_ITERATOR_H
#define DATABASE_ITERATOR_H

#include "layer_iterator.h"

namespace ldblayer 
{

class DatabaseIterator : public IteratorAbstract 
{
public:
	DatabaseIterator(Database* db, Layer* layout = nullptr);

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
	
	bool isBegin() { return start; }
	bool isEnd() { return end; }

	void save();
	void restore();
private:
	LayerIterator iterator;
	bool start;
	bool end;
	std::string savedKey;
};

} // end of namespace

#endif // end of include guard