#ifndef _POINT_HPP_
#define _POINT_HPP_
#include <math.h>
#include <tuple>
struct point
{
	point();
	point(double x_,double y_,double z_=0)
		:x(x_)
		,y(y_)
		,z(z_)
	{
	
	}

	point(const point&pt)
		:x(pt.x)
		,y(pt.y)
		,z(pt.z)
	{
	
	}

	static point min(const point&a,const point&b)
	{
		return b<a?b:a;
	}

	static point max(const point&a,const point&b)
	{
		return a<b?b:a;
	}

	void set(const point&p)
	{
		x=p.x;
		y=p.y;
		z=p.z;
	}

	void set(double,double,double z=0);
	void swap(point&p);

	bool operator<(const point&p)const
	{
		double c=x-p.x;
		if(c!=0) 
			return c<0;

		c=y-p.y;
		if(c!=0) 
			return c<0;

		return z-p.z<0;
	}

	static bool eq(double l,double r,double deta)
	{
		return fabs(l-r)<=deta;
	}

	bool empty()const
	{
		return x==0 && y==0 && z==0;
	}

	bool operator==(const point&r)const
	{
		return eq(x,r.x,1e-10)&&eq(y,r.y,1e-10)&&eq(z,r.z,1e-10);
	}

	double dist_direct(const point&o)const
	{
		double d=dist(o);
		return o<*this?-d:d;
	}

	//计算this到(x,y)的距离
	double dist_direct(double x,double y)const
	{
		return dist_direct(point(x,y));

	}

	double dist(const point&o)const
	{
		return dist(o.x,o.y);
	}

	//计算this到(x,y)的距离
	double dist(double x,double y)const
	{
		double dx=this->x-x;
		double dy=this->y-y;
		return sqrt(dx*dx+dy*dy);
	}

	double cos_k(const point&o)const
	{
		double dx=o.x-x;
		double dy=o.y-y;
		return dx/sqrt(dx*dx+dy*dy);
	}

	double sin_k(const point&o)const
	{
		double dx=o.x-x;
		double dy=o.y-y;
		return dy/sqrt(dx*dx+dy*dy);
	}

	point middle(const point&o)const
	{
		return point((x+o.x)/2,(y+o.y)/2);
	}

	std::tuple<double,double,double> get_abc(const point&o)const
	{
		double cos=fabs(cos_k(o));
		if(cos==0)
			return std::make_tuple(1,0,-x);

		if(cos==1)
			return std::make_tuple(0,1,-y);

		double dx=o.x-x;
		double dy=o.y-y;
		double k=dy/dx;
		double deno=sqrt(k*k+1);
		return std::make_tuple(k/deno,-1/deno,(y-k*x)/deno);
		//return std::make_tuple(k,-1,y-k*x);
	}

public:
	double x,y,z;
};
#endif

