/**
******************************************************************************
* @file    list.h
* @author  Chenxx
* @version V1.1
* @date    2015-10-18
* @brief   This file defines the doubly-linked list class which is an abridged 
*					 version of cpp standard library.
******************************************************************************/
#ifndef LIST_H
#define LIST_H

template <typename T>
class list
{
public:	
	struct Node
	{
	public:
		Node* next_;
		Node* prev_;
		T elem_;
		Node() :
			next_(this),
			prev_(this)
		{}
		Node(Node* next, Node* prev) :
			next_(next),
			prev_(prev)
		{}
		~Node()
		{}
	};

	typedef int intype;
	typedef Node* Iterator;

	list() :
		begin_(&keepNode_),
		size_(0)
	{
		/*begin_->elem_ = {0};*/
	}
	list(T* elemArray, intype size) :
		begin_(&keepNode_),
		size_(0)
	{
		begin_->elem_ = 0;
		for (intype i = 0; i < size; i++)
		{
			push_back(*(elemArray));
		}
	}
	~list()
	{
		clear();
	}

	//
	// list operation
	//
	list& operator = (const list& other)
	{
		Node* it;
		if (this != &other)
		{
			clear();
			it = other.begin_->next_;
			while (it != other.begin_)
			{
				this->push_back(it->elem_);
				it = it->next_;
			}
		}
		return (*this);
	}
	void clear()
	{
		Node* it;
		it = begin_->prev_;
		while (it != begin_)
		{
			destroy(it);
			it = begin_->prev_;
		}
		size_ = 0;
	}
	Node*& back()
	{
		return begin_->prev_;
	}
	void pop_back()
	{
		Node* it;
		it = begin_->prev_;
		if (it != begin_)
			destroy(it);
	}
	void push_back(T elem)
	{
		Node* it_end;
		it_end = begin_->prev_;
		begin_->prev_ = new Node(begin_, it_end);
		it_end->next_ = begin_->prev_;
		begin_->prev_->elem_ = elem;

		size_++;
	}
	Node*& front()
	{
		return begin_->next_;
	}
	void pop_front()
	{
		Node* it;
		it = begin_->next_;
		if (it != begin_)
			destroy(it);
	}
	void push_front(T elem)
	{
		insert(begin_, elem);
	}
	void insert(Node* position, T elem)
	{
		Node* it;
		it = position->next_;
		position->next_ = new Node(it, position);
		it->prev_ = position->next_;
		position->next_->elem_ = elem;

		size_++;
	}
	void destroy(Node* position)
	{
		if (position != begin_)
		{
			position->prev_->next_ = position->next_;
			position->next_->prev_ = position->prev_;

			delete position;
			size_--;
		}
	}
	int size() const
	{
		return size_;
	}

	Node* begin_;
private:
	/*Node<T>* it_;*/
	Node keepNode_;
	int size_;

};

#endif 
/*End of File*/
