#pragma once
#include "ImageManager.h"
#include "SDK_ObjectShape.h"

#include "xrCore/SDK_TreeFolder.h"


#include "SDK_Cache.h"

namespace Cordis
{
namespace SDK
{

	class SDKUI_RightWindow
	{
	private:
		SDKUI_RightWindow(void)
			: m_flag(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
				ImGuiWindowFlags_HorizontalScrollbar),
			  m_id_currentsection(0), m_currentselected_staticobject_name(""), m_is_visible(true), m_is_show_popup(false),
			  m_is_init(false), m_checked_box_selection(true), m_checked_sphere_selection(false)
		{
			this->m_currentselected_sectionname = SDK_Names::getInstance().getName("st_name_staticgeometry");
			if (ImGui::BeginMainMenuBar())
			{
				ImVec2 size = ImGui::GetWindowSize();
				this->m_size_mainmenubar_y = size.y;
				ImGui::EndMainMenuBar();
			}

			if (SDK_Cache::GetInstance().getSpawnElementsCache().size())
			{
/*
				const xr_vector<std::pair<xr_string, xr_string>>& spawnelements_data = SDK_Cache::GetInstance().getSpawnElementsCache();
				this->m_folder.Initialize(SDK_TreeFolder::ParsePath(spawnelements_data[28].first));
				for (unsigned int i = 29; i < spawnelements_data.size(); ++i)
				{
					xr_vector<xr_string>& data = SDK_TreeFolder::ParsePath(spawnelements_data[i].first);
					if (data.back() == this->m_folder.m_root->getValue())
					{
						this->m_folder.m_root->AddElementRecursivly(this->m_folder.m_root, data, data.size() - 2);
					}
						
				}*/

				const xr_vector<std::pair<xr_string, xr_string>>& cache_spawnelements = SDK_Cache::GetInstance().getSpawnElementsCache();
				
				// @ Prepare folders
				for (xr_vector<std::pair<xr_string, xr_string>>::value_type it : cache_spawnelements)
				{
					xr_vector<xr_string>& data = SDK_TreeFolder::ParsePath(it.first);
					if (data.size() < 2)
					{
						SDK_NodeTreeFolder* node = new SDK_NodeTreeFolder();
						node->setValue(data.back());
						node->setValue(false);
						node->setRealName(it.second);
						this->m_folders[data.back()].m_root = node;
					}

					if (!this->m_folders[data.back()].m_root)
						this->m_folders[data.back()].Initialize(data, it.second);
				}


				for (xr_vector<std::pair<xr_string, xr_string>>::value_type it : cache_spawnelements)
				{
					xr_vector<xr_string>& data = SDK_TreeFolder::ParsePath(it.first);

					if (this->m_folders[data.back()].m_root->IsFolder())
						this->m_folders[data.back()].m_root->AddElementRecursivly(this->m_folders[data.back()].m_root, data, it.second, data.size() - 2);
				}
			}
		}

	public:
		inline static SDKUI_RightWindow& Widget(void) noexcept
		{
			static SDKUI_RightWindow instance;
			return instance;
		}
		SDKUI_RightWindow(const SDKUI_RightWindow&) = delete;
		SDKUI_RightWindow& operator=(const SDKUI_RightWindow&) = delete;
		SDKUI_RightWindow(SDKUI_RightWindow&&) = delete;
		SDKUI_RightWindow& operator=(SDKUI_RightWindow&&) = delete;

		~SDKUI_RightWindow(void);

		inline void Hide(void) noexcept { this->m_is_visible = false; }
		inline void Show(void) noexcept { this->m_is_visible = true; }
		inline bool IsVisible(void) const noexcept { return this->m_is_visible; }
		inline unsigned int GetSection(void) const noexcept { return this->m_id_currentsection; }
		inline const xr_string& GetCurrentSelectedStaticObject(void) const noexcept
		{
			return this->m_currentselected_staticobject_name;
		}
		inline const xr_string& getCurrentSelectedSpawnElement(void) const noexcept
		{
			return this->m_currentselected_spawnelement_name;
		}
		inline ShapeType GetCurrentShapeType() const noexcept
		{
			if (this->m_id_currentsection == kSection_Shapes)
			{
				if (this->m_checked_box_selection)
					return kShapeType_Box;
				else
					return kShapeType_Sphere;
			}

			return kShapeType_NonSelected;
		}

		inline void ChangeSectionByObject_CLASS(const CurrentSectionType& id)
		{
			this->m_currentselected_sectionname = this->m_sections[id];
			this->m_id_currentsection = id;
		}

		void Draw(void);


	private:
		void RenderTreeView_Folders(SDK_NodeTreeFolder* node);
		void RenderTreeView_Items(SDK_NodeTreeFolder* node);

	private:
		bool m_is_visible;
		bool m_is_show_popup;
		bool m_is_init;
		bool m_checked_sphere_selection;
		bool m_checked_box_selection;
		int m_id_currentsection;
		float m_size_mainmenubar_y;
		ImGuiWindowFlags m_flag;
		xr_string m_currentselected_staticobject_name; // @ From list
		xr_string m_currentselected_spawnelement_name;
		xr_string m_currentselected_sectionname;
		xr_vector<xr_string> m_sections;

		//SDK_TreeFolder m_folder;
		xr_map<xr_string, SDK_TreeFolder> m_folders;
	};

} // namespace SDK
} // namespace Cordis
