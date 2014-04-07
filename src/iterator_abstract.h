#ifndef ITERATOR_ABSTRACT_H
#define ITERATOR_ABSTRACT_H

#include <string>

namespace ldblayer 
{

class IteratorAbstract
{
public:
	virtual void seekToFirst() = 0;
	virtual void seekToLast() = 0;
	virtual void seekToBegin() = 0;
	virtual void seekToEnd() = 0;
	virtual void seek(const std::string& key) = 0;
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual bool isValid() const = 0;

	virtual std::string key() const = 0;
	virtual std::string value() const = 0;
};

}

#endif