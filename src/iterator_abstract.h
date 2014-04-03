#ifndef ITERATOR_ABSTRACT_H
#define ITERATOR_ABSTRACT_H

#include <string>

namespace ldblayer 
{

class IteratorAbstract
{
public:
	virtual bool seekToFirst() = 0;
	virtual bool seekToLast() = 0;
	virtual void seekToBegin() = 0;
	virtual void seekToEnd() = 0;
	virtual bool seek(const std::string& key) = 0;
	virtual bool next() = 0;
	virtual bool prev() = 0;
	virtual bool isValid() const = 0;

	virtual std::string key() const = 0;
	virtual std::string value() const = 0;
};

}

#endif