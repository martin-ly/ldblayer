#include "layer_iterator.h"
#include "layer.h"

namespace ldblayer 
{

LayerIterator::LayerIterator(Layer* layout)
: iterator(layout)
{
	seekToFirst();
}

void LayerIterator::seekToFirst() 
{	
	iterator.seekToFirst();

	start = false;
	end = !iterator.isValid();
}

void LayerIterator::seekToLast() 
{
	iterator.seekToLast();	
	
	start = !iterator.isValid();
	end = false;
}

void LayerIterator::seekToBegin() 
{
	start = true;
	end = false;	
}

void LayerIterator::seekToEnd() 
{
	start = false;
	end = true;
}

void LayerIterator::seek(const std::string& key) 
{	
	iterator.seek(key);
	start = false;
	end = !iterator.isValid();
}

void LayerIterator::next() 
{
	assert( !end );
	if (start) {
		seekToFirst();
		return;
	}

	if (iterator.isValid()) {
		iterator.next();
		
		if (!iterator.isValid()) {
			end = true;
		}
	} else {
		end = true;
	}
}

void LayerIterator::prev() 
{
	assert( !start );
	if (end) {
		seekToLast();
		return;
	}

	if (iterator.isValid()) {
		iterator.prev();

		if (!iterator.isValid()) {
			start = true;
		}
	} else {
		start = true;
	}
}

bool LayerIterator::isValid() const 
{
	return !(start || end);
}

std::string LayerIterator::key() const 
{
	return iterator.key();
}

std::string LayerIterator::value() const 
{
	return iterator.value();
}

void LayerIterator::save()
{
	if (iterator.isValid()) {
		savedKey = iterator.key();
	} else {
		savedKey.clear();
	}
}

void LayerIterator::restore()
{
	iterator.reopen();

	if (!savedKey.empty()) {
		seek(savedKey);
	}
}

void LayerIterator::setLayout(Layer* layout)
{
	iterator.setLayout(layout);
}

void LayerIterator::reopen() 
{
	iterator.reopen();
}

} // end of namespace