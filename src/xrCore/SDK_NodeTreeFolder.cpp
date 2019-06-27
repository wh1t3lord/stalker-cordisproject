#include "stdafx.h"
#include "SDK_NodeTreeFolder.h"

namespace Cordis
{
	namespace SDK
	{
		SDK_NodeTreeFolder::~SDK_NodeTreeFolder(void)
		{
			for (auto& it : this->m_childrens)
			{
				if (it)
				{
					delete it;
					it = nullptr;
				}
			}

			this->m_childrens.clear();
		}

		SDK_NodeTreeFolder* SDK_NodeTreeFolder::AddElement(const xr_string& value, const bool& is_folder)
		{
			SDK_NodeTreeFolder* node = new SDK_NodeTreeFolder();
			node->setValue(value);
			node->setValue(is_folder);
			return this->AddElement(node);
		}

		SDK_NodeTreeFolder* SDK_NodeTreeFolder::AddElement(SDK_NodeTreeFolder* node)
		{
			static int generated_id = 0;
			if (!this->m_childrens.empty())
			{
				for (xr_vector<SDK_NodeTreeFolder>::value_type* it : this->m_childrens)
				{
					if (it->getValue() == node->getValue())
					{
						if (!it->IsFolder())
							return nullptr;

						return it;
					}

				}
/*
				if (this->m_childrens.back().getValue() == node.getValue())
				{
/ *	Lord: завести функционал только для определения последнего элемента который будет являться item
					xr_string duplicate = node.getValue();
					generated_id += 1;
					duplicate += "_duplicated";
					duplicate += std::to_string(generated_id).c_str();
					node.setValue(duplicate);* /
					return;
				}*/
			}

			this->m_childrens.push_back(node);
			return nullptr;
		}

 
		void SDK_NodeTreeFolder::AddElementRecursivly(SDK_NodeTreeFolder* element, const xr_vector<xr_string>& data, int start_index)
		{
			if (data.size() == 1)
			{
				SDK_NodeTreeFolder* node = new SDK_NodeTreeFolder();
				node->setValue(data.back());
				node->setValue(false);
				element->AddElement(node);
				return;
			}

			if ((data.size() < start_index) || (start_index < 0))
				return;

			SDK_NodeTreeFolder* node = new SDK_NodeTreeFolder();
			node->setValue(data[start_index]);
			node->setValue(true);
 
			if (0 == start_index)
			{
				node->setValue(false);
				element->AddElement(node);
				return;
			}

			SDK_NodeTreeFolder* last_link = element->AddElement(node);
			if (last_link)
			{
				return AddElementRecursivly(last_link, data, start_index - 1);
			}
			
			return AddElementRecursivly(element->getChildrens().back(), data, start_index - 1);


			
		}

	}
}
