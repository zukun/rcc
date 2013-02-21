/////////////////////////////////////////////////////////////////////////////
// Filename: FixedArray.h
// Author:   Sameh Khamis
//
// Description: Algorithm-specific data structure - a fixed array of a known
//              maximum size wih swapping
/////////////////////////////////////////////////////////////////////////////
#ifndef _FIXED_ARRAY
#define _FIXED_ARRAY

template <typename Type, unsigned Size>
class FixedArray
{
public:
	typedef unsigned Iterator;

private:
	Iterator last;
	Type arr[Size];

public:
	FixedArray() { last = 0; }

	Iterator begin() { return 0; }
	Iterator end() { return last; }

	bool empty() { return last == 0; }
	void push_back(Type val) { arr[last++] = val; }
	void remove(Iterator iter) { arr[iter] = arr[--last]; }
	void set(Iterator iter, const Type& val) { arr[iter] = val; }
	void swap(Iterator iter1, Iterator iter2) { Type temp = arr[iter1]; arr[iter1] = arr[iter2]; arr[iter2] = temp; }
	void clear() { last = 0; }
	unsigned size() { return last; }
	const Type& get(Iterator iter) { return arr[iter]; }
};

#endif
