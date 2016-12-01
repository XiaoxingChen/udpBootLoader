/**
  ******************************************************************************
  * @file    kit.h
  * @author  Chenxx
  * @version V1.1
  * @date    2015-10-14
  * @brief   an easy and dummy map.
  ******************************************************************************/
#ifndef KIT_H
#define KIT_H
#include "list.h"
#include "stdint.h"
template <typename T>
class kit
{
public:
	
	struct Association
	{
		/*default constructor*/
		Association()
		{}
		/*mostly used constructor*/
		Association(int16_t key, const T& item)
			:key_(key), item_(item)
		{}
		/*copy constructor*/	
		Association(const Association& other)
			:key_(other.key_), item_(other.item_)
		{}
		int16_t key_;
		T item_;
	};

	/*typedef list<int>::Iterator Iterator;*/
	typedef typename list<Association>::Iterator Iterator;
	kit()
	{}
	~kit()
	{}

	void add(int16_t key, T item)
	{
		registry_.push_back(Association(key, item));
	}
	void destroy(int16_t key)
	{
		Iterator it = registry_.begin_->next_;
		while (it != registry_.begin_)
		{
			if (it->elem_.key_ == key)
			{
				registry_.destroy(it);
				break;
			}
			it = it->next_;
		}
	}
	T& handle(int16_t key) const
	{
		Iterator it = registry_.begin_->next_;
		while (it != registry_.begin_)
		{
			if (it->elem_.key_ == key)
			{
				return it->elem_.item_;
			}
			it = it->next_;
		}
		return registry_.begin_->elem_.item_;
	}

protected:
	list<Association> registry_;

};



#endif
/*End of File*/
