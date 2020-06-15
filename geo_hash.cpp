#include "geo_hash.h"

void geo_list::find_near(std::vector<uint64_t>&ret,int x,int y,unsigned h,uint32_t dist2,unsigned mask,uint64_t card_no)
{
	for(auto it=geo2card.lower_bound(h);it!=geo2card.end();++it)
	{
		if((it->first&mask)!=h)
			break;

		auto pos=ghash::decode(it->first);

		int xi=std::get<0>(pos);
		int yi=std::get<1>(pos);
		unsigned sn = (xi-x)*(xi-x)+(yi-y)*(yi-y);
		if(sn > dist2)
			continue;
		if (card_no==it->second)
			continue;
		//std::stringstream ss;
		//ss<<"-----LemonHash---findnear---cardid"<<card_no<<"x:"<<x<<"y:"<<y<<"dist2:"<<dist2<<" cardid:"<<it->second<<"x:"<<xi<<"y:"<<yi<<"sn"<<sn;
		//debug_print_syslog(0,"%s",ss.str().c_str());
		ret.push_back(it->second);
	}
}
std::vector<uint64_t> geo_list::find_near(int x,int y,int dist,uint64_t card_no)
{
	int t=1;
	while(t<dist) t<<=1;

	int x0=x&~(t-1) , y0=y&~(t-1);

	std::vector<uint64_t> r;
	const int dist2=dist*dist;

	unsigned mask=~(t*t-1);

	find_near(r,x,y,ghash::encode(x0-t,y0-t),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0,y0-t),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0+t,y0-t),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0-t,y0),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0,y0),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0+t,y0),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0-t,y0+t),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0,y0+t),dist2,mask,card_no);
	find_near(r,x,y,ghash::encode(x0+t,y0+t),dist2,mask,card_no);

	return std::move(r);
}

//std::vector<std::string> find_near(const char*card_no,int dist)
std::vector<uint64_t> geo_list::find_near(uint64_t card_no,int dist)
{
	std::vector<uint64_t> r;
	auto it=card2geo.find(card_no);

	if(it==card2geo.end())
		return std::move(r);

	auto h=ghash::decode(it->second);

	return find_near(std::get<0>(h),std::get<1>(h),dist,card_no);
}

//void update(int x,int y,const char*card_no)
void geo_list::update(int x,int y,uint64_t card_no)
{
	//std::stringstream ss;
	unsigned h=ghash::encode(x,y);
	//ss<<"-----LemonHash--update--"<<card_no<<"x:"<<x<<"y:"<<y<<"h:"<<h;
	//debug_print_syslog(0,"%s",ss.str().c_str());

	auto it=card2geo.find(card_no);
	if(it==card2geo.end())
	{
		card2geo.insert(std::make_pair(card_no,h));
		geo2card.insert(std::make_pair(h,card_no));
	}
	else
	{
		it->second=h;

		for(auto it1=geo2card.begin();it1!=geo2card.end();++it1)
		{
			if(it1->second==card_no)
			{
				geo2card.erase(it1);
				break;
			}

		}



		geo2card.insert(std::make_pair(h,card_no));
	}
}
