//Reference: https://github.com/h0tk3y/learning-cpp-list
List::List()
{
	head = Node(-1, nullptr, nullptr);
	tail = Node(-1, &head, nullptr);
	head.next = &tail;
	begin = iterator(&head);
	end = iterator(&tail);
}

List::List(List const& from)
{
	head = Node(-1, nullptr, nullptr);
	tail = Node(-1, &head, nullptr);
	head.next = &tail;
	begin = iterator(&head);
	end = iterator(&tail);
	for (iterator i = iterator(from.head.next); i.node_ptr->next != nullptr; i = i++)
		insert(&tail, *i);
}

List::List& operator=(List const& from)
{
	List l = List();
	for (iterator i = iterator(from.head.next); i.node_ptr->next != nullptr; i = i++)
		l.insert(&l.tail, *i);
	return l;
}

~List::List()
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
	return end.node_ptr->prev->value;
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
	return begin.node_ptr->next->value;
}

void List::erase(Node* what)
{
	what->next->prev = what->prev;
	what->prev->next = what->next;
	delete what;
}

iterator List::insert(Node* before, Object *object)
{ 
	before->prev = new Node(object, before->prev, before);
	before->prev->prev->next = before->prev;
	return iterator(before->prev);
}