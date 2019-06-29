#include "stdafx.h"
#include "SDK_NodeTreeFolder.h"
#include "SDK_TreeFolder.h"


namespace Cordis
{
	namespace SDK
	{
		SDK_TreeFolder::~SDK_TreeFolder(void)
		{
			if (this->m_root)
			{
				delete this->m_root;
				this->m_root = nullptr;
			}

		}

		void SDK_TreeFolder::Initialize(const xr_vector<xr_string>& data, const xr_string& data_realname)
		{
			if (data.empty())
				return;
			this->m_root = new SDK_NodeTreeFolder();
			this->m_root->setValue(data.back());
			this->m_root->setValue(true);
			this->m_root->AddElementRecursivly(this->m_root, data, data_realname, data.size() - 2);

		}
	}
}

