//Reference: https://github.com/h0tk3y/learning-cpp-list
#ifndef __DLLIST_H__
#define __DLLIST_H__

class Object;

struct List
{
	List();
	List(List const& from);
	~List();
	void push_back(Object* object);
	void pop_back();
	Object* back();
	void push_front(Object* object);
	void pop_front();
	Object* front();
	Object* forNext();

	struct Node
	{
		public:
			Object* object;
			Node* next;
			Node* prev;

			Node() {};

			Node(Object* ob, Node* prev, Node* next)
			{
				this->object = ob;
				this->next = next;
				this->prev = prev;
			}
	};

	Node head, tail;

	struct iterator 
	{
		Node* node_ptr;

		iterator(){};

		iterator(Node* i)
		{
			this->node_ptr = i;
		}

		Object*& operator*() const
		{
			return node_ptr->object;
		}

		void operator++()
		{
			this->node_ptr = &(*node_ptr->next);
		}

		iterator operator++(int)
		{
			return iterator(node_ptr->next);
		}

		void operator--()
		{
			this->node_ptr = &(*node_ptr->prev);
		}

		iterator operator--(int)
		{
			return iterator(node_ptr->prev);
		}
	};

	iterator begin;
	iterator end; 
	iterator at;
	void erase(Node* what);
	iterator insert(Node* before, Object* object);
};
#endif