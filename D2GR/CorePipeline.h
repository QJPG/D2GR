#pragma once

#include <iostream>

template <class T>
class Key {
	T* ref = nullptr;

	uint32_t _count = 0;

public:
	~Key();

	T get() const;
};

class Resource {};


class CorePipeline
{
};

template<class T>
inline Key<T>::~Key()
{
	_count -= 1;

	if (_count < 1) {
		delete ref;
	}
}

template<class T>
inline T Key<T>::get() const
{
	_count += 1;

	return *ref;
}
