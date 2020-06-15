#ifndef _VISITOR_HPP_
#define _VISITOR_HPP_

template<typename T>
struct visitor
{
	virtual bool visit(T o)=0;
	virtual ~visitor(){}
};

template<typename T>
struct acceptor
{
	virtual void accept(visitor<T>&v)
	{
	}

	virtual ~acceptor(){}
};

#endif

