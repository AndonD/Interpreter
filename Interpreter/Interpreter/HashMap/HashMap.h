#pragma once
#include <functional>

template <class Key, class Value>
class HashMap
{
private:
	struct TableElement
	{
		Key key;
		Value value;
		TableElement *next;

		TableElement(const Key& _key, const Value& _value, TableElement* _next = nullptr)
		{
			key = _key;
			value = _value;
			next = _next;
		}
	};

public:
	HashMap();
	HashMap(size_t size, std::function<size_t(const Key&)>);
	HashMap(const HashMap& other);
	~HashMap();

	HashMap& operator=(const HashMap& other);
	Value operator[](const Key& key) const;
	Value& operator[](const Key& key);

	bool hasKey(const Key& key) const;
	void remove(const Key& key);
	void print() const;

private:
	void copy(const HashMap& other);
	void destroy();
	TableElement *locate(size_t index, const Key& key) const;

private:
	TableElement **table;
	size_t size;
	std::function<size_t(const Key&)> hashFunc;
};

template <class Key, class Value>
void HashMap<Key, Value>::copy(const HashMap& other)
{
	size = other.size;
	hashFunc = other.hashFunc;
	table = new TableElement*[size];

	TableElement *current = nullptr;
	TableElement *copy = nullptr;
	for (size_t i = 0; i < size; ++i)
	{
		if (other.table[i] == nullptr)
		{
			table[i] = nullptr;
		}
		else
		{
			table[i] = new TableElement(other.table[i]->key, other.table[i]->value, nullptr);
		}
		copy = other.table[i];
		current = table[i];
		while (copy != nullptr && copy->next != nullptr)
		{
			current->next = new TableElement{ copy->next->key, copy->next->value, nullptr };
			copy = copy->next;
			current = current->next;
		}
	}
}

template <class Key, class Value>
void HashMap<Key, Value>::destroy()
{
	TableElement* temp = nullptr;
	TableElement* line = nullptr;
	for (size_t i = 0; i < size; ++i)
	{
		line = table[i];
		while (line != nullptr)
		{
			temp = line;
			line = line->next;
			delete temp;
		}
		table[i] = nullptr;
	}
	delete[] table;
}

template <class Key, class Value>
typename HashMap<Key, Value>::TableElement *HashMap<Key, Value>::locate(size_t index, const Key& key) const
{
	TableElement *current = table[index];
	while (current != nullptr && current->key != key)
	{
		current = current->next;
	}
	return current;
}

template<class Key, class Value>
inline HashMap<Key, Value>::HashMap()
{
	size = 0;
	table = nullptr;
	hashFunc = 0;
}

template <class Key, class Value>
HashMap<Key, Value>::HashMap(size_t _size, std::function<size_t(const Key&)> _h) :size(_size), hashFunc(_h)
{
	table = new TableElement*[size];
	for (size_t i = 0; i < size; ++i)
	{
		table[i] = nullptr;
	}
}

template<class Key, class Value>
HashMap<Key, Value>::HashMap(const HashMap& other)
{
	this->copy(other);
}

template<class Key, class Value>
HashMap<Key, Value>::~HashMap()
{
	this->destroy();
}

template<class Key, class Value>
HashMap<Key, Value>& HashMap<Key, Value>::operator=(const HashMap& other)
{
	if (this != &other)
	{
		destroy();
		copy(other);
	}
	return *this;
}

template <class Key, class Value>
Value HashMap<Key, Value>::operator[](const Key& key) const
{
	TableElement *current = locate(hashFunc(key), key);
	return current->value;
}

template<class Key, class Value>
Value& HashMap<Key, Value>::operator[](const Key& key)
{
	size_t index = hashFunc(key);
	TableElement *current = locate(index, key);

	if (current == nullptr)	//if there is no such key in the table, push it as first element with that index
	{
		table[index] = new TableElement(key, Value(), table[index]);
		current = table[index];
	}

	return current->value;
}

template<class Key, class Value>
bool HashMap<Key, Value>::hasKey(const Key& key) const
{
	TableElement *current = locate(hashFunc(key), key);
	return (current != nullptr);
}

template<class Key, class Value>
void HashMap<Key, Value>::remove(const Key& key)
{
	size_t index = hashFunc(key);
	if (table[index] != nullptr)
	{
		if (table[index]->key == key)
		{
			TableElement *toDelete = table[index];
			table[index] = table[index]->next;
			delete toDelete;
			return;
		}
		TableElement *current = table[index];
		while (current->next->key != key && current != nullptr)
		{
			current = current->next;
		}

		TableElement *toDelete = current->next;
		current->next = current->next->next;
		delete toDelete;
	}
	return;
}

template<class Key, class Value>
void HashMap<Key, Value>::print() const
{
	TableElement *temp = nullptr;
	for (size_t i = 0; i < size; ++i)
	{
		if (table[i] == nullptr)
		{
			continue;
		}
		temp = table[i];
		while (temp != nullptr)
		{
			std::cout << temp->key << " --> " << temp->value << std::endl;
			temp = temp->next;
		}
	}
}
