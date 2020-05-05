#pragma once

template<class T>
class cfList
{
	cfList *Next;
	cfList *Prev;
	static cfList *First;
	static cfList *Last;

public:
	cfList()
	{
		Prev = Last;
		Next = NULL;
		if (Last)	Last->Next = this;
		else	First = this;

		Last = this;
	};

	virtual ~cfList()
	{
		if (Next) Next->Prev = Prev; else Last = Prev;
		if (Prev) Prev->Next = Next; else First = Next;
	};

	class iterator
	{
		typedef cfList<T> _cfData;
		_cfData *i;

	public:
		iterator() : i(_cfData::First) {};
		iterator(_cfData *x) : i(x) {};
		iterator(const iterator &x) : i(x.i) {};

		iterator &operator ++() { i = i->Next; return *this; };
		iterator &operator ++(int) { i = i->Next; return *this; };
		iterator &operator --() { i = i->Prev; return *this; };
		iterator &operator --(int) { i = i->Prev; return *this; };

		T& operator* () const { return *(T*)i; }
		T* operator-> () const { return (T*)i; }

		operator T* () const { return (T*)i; }
	};

	
	struct SList {
		iterator begin() { return First; }
		iterator end() { return nullptr; }
	};

	static SList List() { return SList(); }

	friend iterator;

	static iterator begin() { return First; }
	static iterator end() { return NULL; }
	static bool empty() { return Last == NULL; }

	static void UnsefeReset() { First = NULL; Last = NULL; }
	static void clear()
	{
		while (begin() != end())
		{
			auto p = begin();
			delete p;
		}
	}
};

template< class T > cfList< T > * cfList< T >::First = NULL;
template< class T > cfList< T > * cfList< T >::Last = NULL;
