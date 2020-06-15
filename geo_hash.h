#ifndef _GEO_HASH__
#define _GEO_HASH__

#include "point.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <tuple>
#include <boost/container/flat_set.hpp>
#include <string>

struct ghash
{
	static std::tuple<int,int> decode(unsigned h)
	{
		const unsigned S=0x80000000;

		int x=0,y=0;
		for(int i=0;i<16;i++)
		{
			x<<=1;
			y<<=1;

			if(h&S) 
				x|=1;
			h<<=1;

			if(h&S) 
				y|=1;
			h<<=1;
		}

		return std::make_tuple(x-32768,y-32768);
	}

	static unsigned encode(int x, int y)
	{
		return encode_(x+32768,y+32768);
	}

public: //test
	static void test_code(int x,int y)
	{
		unsigned h=ghash::encode(x,y);
		auto t=ghash::decode(h);

		printf("src x=%X,y=%X hash=%X,check x=%X,y=%X\n",x,y,h,std::get<0>(t),std::get<1>(t));
	}

	static void test()
	{
		for(int i=0;i<10;i++)
		{
			test_code((4<<i)-1,(4<<i)-1);
			test_code((4<<i)-1,(4<<i));
			test_code((4<<i)-1,(4<<i)-1);
			test_code((4<<i),(4<<i)-1);
		}
	}
private:
	static unsigned encode_(unsigned short x, unsigned short y)
	{
		const unsigned S=0x8000;
		unsigned r=0;
		for(int i=0;i<16;i++)
		{
			r<<=2;
			if(x&S)
			{
				r|=(y&S)?3:2;
			}
			else
			{
				if(y&S) r|=1;
			}

			x<<=1;
			y<<=1;
		}

		return r;
	}
};

struct geo_list
{
private:
	std::multimap<unsigned int,uint64_t> geo2card;
	std::map<uint64_t,unsigned int> card2geo;

	inline void find_near(std::vector<uint64_t>&ret,int x,int y,unsigned h,uint32_t dist2,unsigned mask,uint64_t card_no);

public:

	std::vector<uint64_t> find_near(int x,int y,int dist,uint64_t card_no);

	//std::vector<std::string> find_near(const char*card_no,int dist)
	std::vector<uint64_t> find_near(uint64_t card_no,int dist);
	//void update(int x,int y,const char*card_no)
	void update(int x,int y,uint64_t card_no);

	size_t size()
	{
		return card2geo.size();
	}

	void print()
	{
		for(auto it=card2geo.begin();it!=card2geo.end();++it)
		{
			std::cout<<it->first<<"\n";
		}
	}

	geo_list()
	{
	}

	~geo_list()
	{
	}
};
#endif
