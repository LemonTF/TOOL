#ifndef _WRITE_COPY_HPP_
#define  _WRITE_COPY_HPP_
//add之前如果有数据，需要把运行中更改的数据进行保存。
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <mutex>
#include <atomic>
#include <vector>
#include "visit.h"

template<typename T,typename K,typename V> 
struct write_copy_base:acceptor<V>
{
	void accept(visitor<V> &v)
	{
		for(std::pair<K,V> me:m_map)
		{
			v.visit(me.second);
		}
	}

	std::unordered_map<K,V> m_map;
	int m_version=0;
	write_copy_base()
	{}

	int version()const
	{
		return m_version;
	}

	int set_version(int v)
	{
		return m_version=++v;
	}


	V get(K k)const
	{
		return m_map[k];
	}

	V get(K k)
	{
		auto serch = m_map.find(k);
		if(serch != m_map.end())
		  return serch->second;
		else
		  return nullptr;
	}

	std::shared_ptr<T> clone()const
	{
		std::shared_ptr<T> ret=std::make_shared<T>();
		ret->m_map.insert(m_map.begin(),m_map.end());
		ret->set_version(m_version);

		return ret;
	}

	std::shared_ptr<T> clone_add(const std::unordered_map<K,V>&m)const
	{
		std::shared_ptr<T> ret=std::move(clone());
		ret->_add(m);
		return ret;
	}

	std::shared_ptr<T> clone_add(K k,V v)const
	{
		std::shared_ptr<T> ret=std::move(clone());
		ret->_add(k,v);
		return ret;
	}

	std::shared_ptr<T> clone_remove(K k)const
	{
		std::shared_ptr<T> ret=std::move(clone());
		ret->_remove(k);
		return ret;
	}

	std::shared_ptr<T> clone_remove(const std::vector<K>&list)const
	{
		std::shared_ptr<T> ret=std::move(clone());
		ret->_remove(list);
		return ret;
	}

	virtual ~write_copy_base()
	{
//		std::for_each(m_list.begin(),m_list.end(),[](K*it){ });
	}
private:
	void _remove(K k)
	{
		m_map.erase(k);
	}

	void _remove(const std::vector<K>& v)
	{
        for(K k:v)
		{
			m_map.erase(k);
		}
	}

	void _add(K k,V v)
	{
		m_map.insert(std::make_pair(k,v));
	}

	void _add(const std::unordered_map<K,V>&m)
	{
		m_map.insert(m.begin(),m.end());
	}
};

template<typename T>
struct safe_shared_ptr
{
	std::atomic<int> m_mutex;
	std::shared_ptr<T> m_ptr;

	explicit safe_shared_ptr(std::shared_ptr<T> ptr)
	{
		m_mutex=0;
		m_ptr=ptr;
	}

	safe_shared_ptr(safe_shared_ptr<T>&&ptr)
	{
		m_mutex.store(ptr.m_mutex.load());
		m_ptr=std::move(ptr.get());
	}


	bool set(std::shared_ptr<T> ptr)
	{
		lock();
		m_ptr=ptr;
		unlock();

		return true;
	}

	std::shared_ptr<T> get()
	{
		lock();
		std::shared_ptr<T>  ret=m_ptr;
		unlock();
		return ret;
	}
private:
	void lock()
	{
		int expected=0;
		while(!m_mutex.compare_exchange_strong(expected,1))
			expected=0;
		assert(m_mutex.load()==1);
	}

	void unlock()
	{
		m_mutex.store(0);
	}
};

template<typename T,typename K,typename V>
struct single_base:write_copy_base<T,K,V>
{
	typedef write_copy_base<T,K,V> base;

	static safe_shared_ptr<T> m_instance;
	std::mutex         m_mutex;

	static std::shared_ptr<T> instance()
	{
		return m_instance.get();
	}

	void add(K k,V c)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_instance.set(base::clone_add(k,c));
	}

	void add(const std::unordered_map<K,V>&c)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_instance.set(base::clone_add(c));
	}
	void remove(K k,V c)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_instance.set(base::clone_remove(k,c));
	}

	void remove(const std::unordered_map<K,V>&c)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_instance.set(base::clone_remove(c));
	}

    void remove(const std::vector<K>&list)
    {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_instance.set(base::clone_remove(list));
    }

    void remove(K k)
    {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_instance.set(base::clone_remove(k));
    }
};

template<typename T,typename K,typename V> safe_shared_ptr<T> single_base<T,K,V>::m_instance{std::make_shared<T>()};

#endif


