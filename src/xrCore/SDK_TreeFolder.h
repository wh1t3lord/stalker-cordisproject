/*
	Data: 27.06.2019 
	Author: Lord
	Description: Simple data structure for tree-folder represantation
*/

#pragma once

#include "SDK_NodeTreeFolder.h"

namespace Cordis
{
	namespace SDK
	{
		class XRCORE_API SDK_TreeFolder
		{
		public:
			SDK_TreeFolder(void) = default;
			SDK_TreeFolder(const SDK_TreeFolder&) = delete;
			SDK_TreeFolder& operator=(const SDK_TreeFolder&) = delete;
			SDK_TreeFolder(SDK_TreeFolder&&) = delete;
			SDK_TreeFolder& operator=(SDK_TreeFolder&&) = delete;
			~SDK_TreeFolder(void);

			void Initialize(const xr_vector<xr_string>& data, const xr_string& data_realname);
			static xr_vector<xr_string> ParsePath(xr_string path)
			{
				if (path.empty())
					return xr_vector<xr_string>();

				static int generated_id = 0;
				xr_vector<xr_string> data;

				if (path.find('\\') == xr_string::npos)
				{
					data.push_back(path);
					return data;
				}

				xr_string object_name = path.substr(path.rfind('\\') + 1); // Get last element e.g. folder\\folder2\\our_object_name.txt

				if (object_name.empty()) // if folder\\folder2\\ <- nothing there
				{
					object_name = "unkown_item";
					generated_id += 1;
					object_name += generated_id;
				}


				xr_string temporary;
				data.push_back(object_name);
				path.erase(path.rfind('\\'));
				while (path.rfind('\\') != xr_string::npos)
				{
					temporary = path.substr(path.rfind('\\') + 1);
					data.push_back(temporary);
					path.erase(path.rfind('\\'));
				}

				data.push_back(path); // pushes last folder

				return data;
			}
		public:
			SDK_NodeTreeFolder* m_root;
		};

	}
}

