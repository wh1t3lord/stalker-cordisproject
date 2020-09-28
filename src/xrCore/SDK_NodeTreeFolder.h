/*
	Data: 27.06.2019
	Author: Lord
	Description: simple elemet for SDK_TreeFolder class
*/

#pragma once

namespace Cordis
{
	namespace SDK
	{

		class XRCORE_API SDK_NodeTreeFolder
		{
		public:
			SDK_NodeTreeFolder(void) = default;
			~SDK_NodeTreeFolder(void);


			inline const xr_string& getValue(void) const { return this->m_value; }
			inline const xr_string& getSectionName(void) const { if (this->IsFolder()) return ""; else return this->m_real_name; }
			inline bool IsFolder(void) const { return this->m_is_folder; }
			inline xr_vector<SDK_NodeTreeFolder*>& getChildrens(void) { return this->m_childrens; }
			inline void setValue(const xr_string& value) { this->m_value = value; }
			inline void setValue(const bool& is_folder) { this->m_is_folder = is_folder; }
			inline void setRealName(const xr_string& value) { if (!this->IsFolder()) this->m_real_name = value; }
			SDK_NodeTreeFolder* AddElement(SDK_NodeTreeFolder* node);
			SDK_NodeTreeFolder* AddElement(const xr_string& value, const bool& is_folder);
 
			void AddElementRecursivly(SDK_NodeTreeFolder* element, const xr_vector<xr_string>& data, const xr_string& data_realname, int start_index);

		private:
			bool m_is_folder;
			xr_string m_value;
			xr_string m_real_name; 
			xr_vector<SDK_NodeTreeFolder*> m_childrens;
		};


	}
}
