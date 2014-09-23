//Reference: https://github.com/h0tk3y/learning-cpp-list
#include "include/dllist.h"

List::List()
{
	head = Node(nullptr, nullptr, nullptr);
	tail = Node(nullptr, &head, nullptr);
	head.next = &tail;
	begin = iterator(&head);
	end = iterator(&tail);
	at = iterator(&tail);
}

List::List(List const& from)
{
	head = Node(nullptr, nullptr, nullptr);
	tail = Node(nullptr, &head, nullptr);
	head.next = &tail;
	begin = iterator(&head);
	end = iterator(&tail);
	for (iterator i = iterator(from.head.next); i.node_ptr->next != nullptr; i = i++)
		insert(&tail, *i);
	at = iterator(&tail);
}

List::~List()
{
	iterator it = iterator(head.next);
	while (it.node_ptr->next != nullptr)
	{
		it++;
		delete it.node_ptr->prev;
	}
}

void List::push_back(Object *object)
{
	insert(&tail, object);
}

void List::pop_back()
{
	erase(tail.prev);
}

Object* List::back()
{
	return end.node_ptr->prev->object;
}

void List::push_front(Object *object)
{
	insert(head.next, object);
}

void List::pop_front()
{
	erase(head.next);
}

Object* List::front()
{
	return begin.node_ptr->next->object;
}

void List::erase(Node* what)
{
	what->next->prev = what->prev;
	what->prev->next = what->next;
	delete what;
}

List::iterator List::insert(Node* before, Object *object)
{ 
	before->prev = new Node(object, before->prev, before);
	before->prev->prev->next = before->prev;
	return iterator(before->prev);
}

Object* List::forNext()
{ 
	if(at.node_ptr == &tail)
		at = iterator(head.next);
	else 
		at = at.node_ptr->next;
	if(at.node_ptr == &tail)
		return nullptr;
	else
		return at.node_ptr->object;
}