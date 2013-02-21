/////////////////////////////////////////////////////////////////////////////
// Filename: DoublyLinkedArray.h
// Author:   Sameh Khamis
//
// Description: Algorithm-specific data structure - a doubly linked list
//              embedded in an array
/////////////////////////////////////////////////////////////////////////////
#ifndef _DOUBLY_LINKED_ARRAY
#define _DOUBLY_LINKED_ARRAY

template <typename Type>
class DoublyLinkedArray
{
private:
	typedef pair<Type, Type> Node;
	Type first, last;
	Node *nodes;
	Type size, count;

public:
	DoublyLinkedArray(Type size)
	{
		this->size = size;
		nodes = new Node[size];
		for (unsigned i = 0; i < size; i++)
		{
			nodes[i].first = size;
			nodes[i].second = size;
		}
		first = last = size;
		count = 0;
	}

	~DoublyLinkedArray()
	{
		delete[] nodes;
	}

	unsigned get_count()
	{
		return count;
	}

	bool empty()
	{
		return first == size;
	}

	void push_back(const Type& id)
	{
		if (!empty())
		{
			nodes[last].second = id;
			nodes[id].first = last;
		}
		else
			first = id;
		last = id;
		count++;
	}

	Type pop_front()
	{
		Type old_first = first;
		first = nodes[first].second;
		nodes[old_first].second = size;

		if (!empty())
			nodes[first].first = size;
		else
			last = size;

		count--;
		return old_first;
	}

	void remove(Type id)
	{
		if (id == first)
			pop_front();
		else
		{
			if (id == last)
				last = nodes[id].first;
			else
				nodes[nodes[id].second].first = nodes[id].first;

			nodes[nodes[id].first].second = nodes[id].second;
			nodes[id].first = size;
			nodes[id].second = size;
			count--;
		}
	}
};

#endif
