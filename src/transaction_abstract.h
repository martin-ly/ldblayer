#ifndef TRANSACTION_ABSTRACT_H
#define TRANSACTION_ABSTRACT_H

#include <string>

namespace ldblayer 
{

class TransactionAbstract
{
public:
	virtual void Put(const std::string& key, const std::string& value) = 0;
	virtual void Delete(const std::string& key) = 0;
	
	virtual bool commit() = 0;
};

}

#endif