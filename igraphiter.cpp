#include "igraph.hpp"
#include <iostream>

/* Breadth-First Search */

IGraphBFSIter IGraphBFSIterGen::begin()
{
	return IGraphBFSIter(m_graph, false, m_visits, m_root);
}

IGraphBFSIter IGraphBFSIterGen::end()
{
	return IGraphBFSIter(m_graph, true, m_visits, 0);
}

IGraphBFSIter& IGraphBFSIter::operator=(const IGraphBFSIter& rhs)
{
	if (this == &rhs)
		return *this;

	/* ugly workaround */
	memcpy((void*)this, (void*)&rhs, sizeof(IGraphBFSIter));
	return *this;
}

IGraphBFSIter& IGraphBFSIter::operator++()
{

	if (m_end)
		return *this;

	m_visits.insert(m_current_node);

	auto neighbours = m_graph.retornarVizinhos(m_current_node);

	if (neighbours) for (auto n : *neighbours)
		m_queue.push(n);

	if (m_queue.size() == 0)
	{
		m_end = true;
		return *this;
	}

	while (m_visits.find(m_current_node) != m_visits.end())
	{
		if (m_queue.size() == 0)
		{
			m_end = true;
			return *this;
		}
		m_current_node = m_queue.front();
		m_queue.pop();
	}

	return *this;
}

id_t IGraphBFSIter::operator*() const
{
	return m_current_node;
}



bool operator==(const IGraphBFSIter& lhs, const IGraphBFSIter& rhs)
{

	if (&lhs.m_graph != &rhs.m_graph)
		return false;

	if (lhs.m_end && rhs.m_end)
		return true;

	return (lhs.m_current_node == rhs.m_current_node) && !rhs.m_end;
}

bool operator!=(const IGraphBFSIter& lhs, const IGraphBFSIter& rhs)
{
	return !(lhs == rhs);
}

/* Depth-First Search */

IGraphDFSIter IGraphDFSIterGen::begin()
{
	return IGraphDFSIter(m_graph, false, m_visits, m_root);
}

IGraphDFSIter IGraphDFSIterGen::end()
{
	return IGraphDFSIter(m_graph, true, m_visits, m_root);
}

IGraphDFSIter& IGraphDFSIter::operator=(const IGraphDFSIter& rhs)
{
	if (this == &rhs)
		return *this;

	memcpy((void*)this, (void*)&rhs, sizeof(IGraphDFSIter));
	return *this;
}

IGraphDFSIter& IGraphDFSIter::operator++()
{

	if (m_end)
		return *this;

	m_visits.insert(m_current_node);

	auto neighbours = m_graph.retornarVizinhos(m_current_node);

	if (neighbours) for (auto n : *neighbours)
		m_stack.push(n);

	if (m_stack.size() == 0)
	{
		m_end = true;
		return *this;
	}

	while (m_visits.find(m_current_node) != m_visits.end())
	{
		if (m_stack.size() == 0)
		{
			m_end = true;
			return *this;
		}
		m_current_node = m_stack.top();
		m_stack.pop();
	}

	return *this;
}

id_t IGraphDFSIter::operator*() const
{
	return m_current_node;
}

bool operator==(const IGraphDFSIter& lhs, const IGraphDFSIter& rhs)
{

	if (&lhs.m_graph != &rhs.m_graph)
		return false;

	if (lhs.m_end && rhs.m_end)
		return true;

	return (lhs.m_current_node == rhs.m_current_node) && !rhs.m_end;
}

bool operator!=(const IGraphDFSIter& lhs, const IGraphDFSIter& rhs)
{
	return !(lhs == rhs);
}
