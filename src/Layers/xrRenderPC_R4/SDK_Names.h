#pragma once

#include "SDK_FileSystemState.h"
#include "xrCore/Cordis/XML/cordis_xml.h"
#include <filesystem>

namespace Cordis
{
	namespace SDK
	{
		class SDK_Names
		{
		private:
			SDK_Names(void) noexcept : m_generated_index(0) 
			{
			
			}

		public:
			inline static SDK_Names& getInstance(void) noexcept
			{
				static SDK_Names instance;
				return instance;
			}

			SDK_Names(const SDK_Names&) = delete;
			SDK_Names& operator=(const SDK_Names&) = delete;
			SDK_Names(SDK_Names&&) = delete;
			SDK_Names& operator=(SDK_Names&&) = delete;
			~SDK_Names(void) = default;

			inline xr_string getName(const xr_string& id)
			{
				if (id.empty() || this->m_current_language.empty())
					return u8"st_invalid_id";

				for (std::pair<xr_string, xr_map<xr_string, xr_string>> it : this->m_data)
				{
					if (it.first == this->m_current_language)
					{
						xr_string result = it.second[id];
						if (result.empty())
						{
							result = id;
						}
							
						return result;
					}
				}

				return u8"st_invalid_id";
			}
			
			inline const xr_string& getCurrentLanguage(void)
			{
				if (this->m_current_language.empty())
					return xr_string(TEXT("Empty"));
				 
				return this->m_current_language;
			}

			inline void setCurrentLanguage(const xr_string& str)
			{
				if (str.empty())
					return;

				this->m_current_language = str;
			}

			void Initialize(void);

		private:
			bool LoadXml(const xr_string& filepath);
		private:
			int m_generated_index;
			xr_string m_current_language;
		public:
			xr_vector<xr_string> m_languages;
		private:
			// @ Key - name from xml, value - from <text> Heh </text>
			xr_list<std::pair<xr_string, xr_map<xr_string, xr_string>>> m_data;
		};
	}
}