#include "point.h"

point::point() 
	:x(0) ,y(0),z(0)
{
}

void point::set(double x_,double y_,double z_) 
{
	x=x_;
	y=y_;
	z=z_;
}

void point::swap(point&p)
{
	point tmp=*this;
	*this=p;
	p=tmp;
}

