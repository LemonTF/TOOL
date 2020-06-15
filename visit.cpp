#include <thread>
#include <chrono>
#include "write-copy.h"

#ifdef _TEST_

struct card
{
	int id;
	card(int _id):id(_id){}
};

struct card_list:single_base<card_list,int,std::shared_ptr<card>>
{
	void accept(visitor<std::shared_ptr<card>>&v)
	{
		for(std::pair<int,std::shared_ptr<card>> me:m_map)
		{
			v.visit(me.second);
			printf("====\n");
		}
	}
};

struct visit_log:visitor<std::shared_ptr<card>>
{
	bool visit(std::shared_ptr<card> c)
	{
		printf("vist :::::::%d\n",c->id);
		return true;
	}
};

template<> std::shared_ptr<card_list> single_base<card_list,int,std::shared_ptr<card>>::m_instance=std::make_shared<card_list>();
void run()
{
	visit_log vl;
	card_list::instance()->accept(vl);
	/*
	while(true)
	{
		printf("111111111111\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	*/
}
int main()
{
	std::vector<std::shared_ptr<card_list>> v;
	//std::thread th(run);
	for(int i=0;i<10000;i++)
	{
		card_list::instance()->add(i,std::make_shared<card>(i));//每次生成一个对象
		printf("%d  %p\n",i,card_list::instance().get());
		v.push_back(card_list::instance());
	}

	for(int i=0;i<100;i++)
	{
	//	printf("%d  %p\n",i,v[i].get());
	}

	//th.join();
	visit_log vl;
	card_list::instance()->accept(vl);
//	printf("%p",card_list::instance()->get(5));
	auto x = card_list::instance()->get(100);
	if(x!=nullptr)
	  printf("...........id:%d\n",x->id);
	else
	  printf("nullptr...\n");
	return 0;
}

#endif


