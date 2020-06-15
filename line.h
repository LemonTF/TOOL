#ifndef _LINE_HPP_
#define _LINE_HPP_
#include "point.h"
struct line
{
	double a,b,c;
	line()
		:a(0)
		,b(0)
		,c(0)
	{
	}
	line(double k,double b_)
		:a(k)
		,b(-1)
		,c(b_)
	{
	}

	line(double a_,double b_,double c_)
		:a(a_)
		,b(b_)
		,c(c_)
	{
	}

	line(const point&p0,const point&p1)
	{
		set(p0,p1);
	}

	void clear()
	{
		a=b=c=0;
	}

	bool empty() const
	{
		return a==0 && b==0 && c==0;
	}

	void set(const point&p0,const point&p1)
	{
		auto abc=p0.get_abc(p1);

		a=std::get<0>(abc);
		b=std::get<1>(abc);
		c=std::get<2>(abc);
	}

	std::string to_string()const
	{
		char buf[256];
		int len=sprintf(buf,"a=%.3lf,b=%.3lf,c=%.3lf",a,b,c);
		return std::string(buf,len);
	}

	double cos_k()const
	{
		if(b==0) 
			return 0;

		double k=-a/b;

		return 1/sqrt(1+k*k);
	}

	double sin_k()const
	{
		if(b==0) 
			return 1;

		double k=-a/b;

		return k/sqrt(1+k*k);
	}

	double arg()const
	{
		return acos(cos_k())*(sin_k()>0?1:-1);
	}

	static bool eq(double a,double b,double e)
	{
		return fabs(a-b)<e;
	}

	point projection(const point&lv)const
	{
		line pp(-b,a,b*lv.x-a*lv.y);
		return crossing(pp);
	}

	double dist(const point&v)const
	{
		return v.dist(projection(v));
	}

	bool parallel(const line&l2)const
	{
		return eq(cos_k(),l2.cos_k(),0.0001) && eq(sin_k(),l2.sin_k(),0.0001);
	}

	point  crossing(const line&l)const
	{
		if(parallel(l))
			return point();

		return point ( (c*l.b-l.c*b) /(l.a*b-a*l.b) ,
			           (c*l.a-l.c*a) /(l.b*a-b*l.a) );
	}

	bool contain(const point&p,double e)const
	{
		return contain(p.x,p.y,e);
	}

	bool contain(double x,double y,double e)const
	{
		return fabs(a*x+b*y+c)<e;
	}
};
struct line_v:line//线段
{
	std::array<point,2> v;
    point angle;
	line_v()
	{
		v[0].set(0,0);
		v[1].set(0,0);
	}

	line_v(const point&p0,const point&p1)
		:line(p0,p1)
	{
		v[0]=p0;
		v[1]=p1;
        recalc();
	}

	line_v widen(double length) const
	{
		point pt1(v[0].x-length*cos(),v[0].y-length*sin());
		point pt2(v[1].x+length*cos(),v[1].y+length*sin());

		return line_v(pt1,pt2);
	}

	void swap_point()
	{
		point tmp=v[0];
		v[0]=v[1];
		v[1]=tmp;
		recalc();
	}

    void recalc()
    {
        double dx = v[1].x - v[0].x;
        double dy = v[1].y - v[0].y;
        double r = sqrt(dx*dx + dy*dy);
        double cos = dx/r;
        double sin = dy/r;
        angle = point(cos,sin);
    }

	void set_point(int i,const point&p)
	{
		v[i]=p;
		recalc();
	}

    double cos() const {return angle.x;}
    double sin() const {return angle.y;}
	point&operator[](int i) { return v[i]; }
	const point&operator[](int i)const { return v[i]; }

	std::string to_string()const
	{
		char buf[256];
		int len=sprintf(buf,"[(%.3lf,%.3lf),(%.3lf,%.3lf)(%f,%f)]",v[0].x,v[0].y,v[1].x,v[1].y,cos(),sin());

		return std::string(buf,len);
	}

	point  crossing(const line&l)const
	{
		point pt=line::crossing(l);

		return contain(pt)?pt:point();
	}

	point  crossing(const line_v&l)const
	{
		point pt=line::crossing(l);

		return contain(pt)&&l.contain(pt)?pt:point();
	}

	bool is_same(const line_v&l,double e=0.0001)const
	{
		return line::contain(l[0],e) && line::contain(l[1],e);
	}

	point  projection(const point&p)const
	{
		point pt=line::projection(p);
		return contain(pt)?pt:point();
	}

	line_v projection(const line_v&lv)const
	{
		std::array<point,2> o,t;

		o[0]=line::projection(lv[0]);
		o[1]=line::projection(lv[1]);
		if(o[1]<o[0]) o[0].swap(o[1]);

		t[0]=v[0];
		t[1]=v[1];
		if(t[1]<t[0]) t[0].swap(t[1]);
		
		if(o[1] < t[0] || t[1] < o[0])
			return line_v(point(0,0),point(0,0));

		return line_v(point::max(t[0],o[0]),point::min(t[1],o[1]));
	}

	bool empty()const
	{
		return length()<0.01;
	}

	double length()const
	{
		return v[0].dist(v[1]);
	}

	bool contain(const point&p,double e=0.0001)const
	{
		return contain(p.x,p.y,e);
	}

	bool contain(double x,double y,double e=0.0001)const
	{
		return eq(v[0].dist(v[1]),v[0].dist(x,y)+v[1].dist(x,y),e);
	}

	double dist(const point&v)const
	{
		point p=projection(v);
		if(p.empty())
			return -1;

		return v.dist(p);
	}

	const line &as_line()const
	{
		return *this;
	}
};
struct line_r:line
{
    std::array<point,2> v;
	line_r()
	{
		v[0].set(0,0);
		v[1].set(0,0);
	}


	line_r(const point&p0,const point&p1)
		:line(p0,p1)
	{
		v[0]=p0;
		v[1]=p1;
	}

	point&operator[](int i) { return v[i]; }
	const point&operator[](int i)const { return v[i]; }

	std::string to_string()const
	{
		char buf[256];
		int len=sprintf(buf,"[(%.3lf,%.3lf),(%.3lf,%.3lf)]",v[0].x,v[0].y,v[1].x,v[1].y);

		return std::string(buf,len);
	}

	point  crossing(const line&l)const
	{
		point pt=line::crossing(l);

		return contain(pt)?pt:point();
	}

	point  crossing(const line_r&l)const
	{
		point pt=line::crossing(l);

		return contain(pt)&&l.contain(pt)?pt:point();
	}

	point  projection(const point&p)const
	{
		point pt=line::projection(p);
		return contain(pt)?pt:point();
	}

	bool invalid()const
	{
        return v[0].dist(v[1])<0.01;
	}

	bool contain(const point&p,double e=0.0001)const
	{
		return contain(p.x,p.y,e);
	}
    bool is_same_direction(const line_r &lr)
    {
        bool f=false;
        if(v[0]==lr[0]){
            if(contain(lr[1]))
                f=true;
        }
        else{
            f=(contain(lr[0]) && contain(lr[1]) && !lr.contain(v[0]))|| (lr.contain(v[0]) && lr.contain(v[1]) && !contain(lr[0]));
        }
        return f;
    }
	bool contain(double x,double y,double e=0.0001)const
	{
        return line::contain(x,y,e)?in_contain(x,y,e)||out_contain(x,y,e):false;
	}
    bool in_contain(const point &p,double e=0.0001)
    {
        return in_contain(p.x,p.y,e);
    }
	bool in_contain(double x,double y,double e=0.0001)const
    {
        return line::contain(x,y,e)?eq(v[0].dist(v[1]),v[0].dist(x,y)+v[1].dist(x,y),e):false;
    }
	bool out_contain(double x,double y,double e=0.0001)const
    {
        return line::contain(x,y,e)?(v[0].dist(x,y)>=v[1].dist(x,y)):false;
	}
	double dist(const point&v)const
	{
		point p=projection(v);
		if(p.empty())
			return -1;

		return v.dist(p);
	}

	const line &as_line()const
	{
		return *this;
	}

};
#endif


