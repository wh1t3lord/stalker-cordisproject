#include "stdafx.h"
#include "SDK_Names.h"
#include "SDKUI_Helpers.h"

namespace Cordis
{
	namespace SDK
	{
		const char xml_data_default_english[] = "<?xml version=\"1.0\" ?>\
												<string_table>\
												<!-- System information -->\
												<string id=\"st_language\">Integrated English</string>\
												<!-- System information -->\
												<!-- Main data -->\
												<string id=\"st_direction\">Direction</string>\
												<string id=\"st_position\">Position</string>\
												<string id=\"st_scale\">Scale</string>\
												<string id=\"st_altitude\">Altitude</string>\
												<string id=\"st_magnitude\">Magnitude</string>\
												<string id=\"st_\"></string>\
												<string id=\"st_x\">X</string>\
												<string id=\"st_y\">Y</string>\
												<string id=\"st_z\">Z</string>\
												<string id=\"st_warning\">Warning</string>\
												<string id=\"st_error\">Error</string>\
												<!-- Main data -->\
												<!-- Camera Manager -->\
												<string id=\"st_manager_show_camera\">Show camera manager</string>\
												<string id=\"st_label_manager_camera\">Camera manager</string>\
												<!-- Camera Manager -->\
												<!-- Log Manager -->\
												<string id=\"st_manager_show_log\">Show log</string>\
												<string id=\"st_label_manager_log\">Log manager</string>\
												<!-- Log Manager -->\
												<!-- Weather Manager -->\
												<string id=\"st_manager_show_weather\">Show weather manager</string>\
												<!-- Weather Manager -->\
												<!-- Right Window -->\
												<string id=\"st_name_staticgeometry\">Static geometry</string>\
												<string id=\"st_name_groups\">Groups</string>\
												<string id=\"st_name_light\">Lights</string>\
												<string id=\"st_name_staticparticles\">Static particles</string>\
												<string id=\"st_name_waypoints\">Way points</string>\
												<string id=\"st_name_sound_environment\">Sound environments</string>\
												<string id=\"st_name_ainodes\">AI-Nodes</string>\
												<string id=\"st_name_spawnelements\">Spawn elements</string>\
												<!-- Right Window -->\
												<!-- Overlay -->\
												<string id=\"st_grid\">Grid</string>\
												<string id=\"st_background_color\">Background color</string>\
												<string id=\"st_grid_color\">Grid color</string>\
												<!-- Overlay -->\
												<!-- Main Menu Bar -->\
												<string id=\"st_help\">Help</string>\
												<string id=\"st_file\">File</string>\
												<string id=\"st_quit\">Quit</string>\
												<string id=\"st_recent_files\">Recent files</string>\
												<!-- Main Menu Bar -->\
												</string_table>";

		void SDK_Names::Initialize(void)
		{
			if (SDK_FileSystemState::m_is_translationempty)
			{
				SDKUI_Log::Widget().SetColor(warning);
				SDKUI_Log::Widget().AddText("Can't use any language! Because translation folder is empty!");
				XML::TiXmlDocument document;

				document.Parse(xml_data_default_english);
				SDKUI_Log::Widget().SetColor(unimportant);
				SDKUI_Log::Widget().AddText("Parsing default language - English");
				std::pair<xr_string, xr_map<xr_string, xr_string>> data;
				std::pair<xr_string, xr_string> data_map;
				XML::TiXmlElement* element_root = document.RootElement();
				if (element_root)
				{
					XML::TiXmlElement* element_string = element_root->FirstChildElement();
					if (element_string)
					{
						while (element_string)
						{
							SDKUI_Log::Widget().SetColor(error);
							if (element_string->Attribute("id"))
							{
								if (element_string->GetText())
								{
									data_map.first = element_string->Attribute("id");
									data_map.second = element_string->GetText();
									data.second.insert(data_map);
								}
							}
							element_string = element_string->NextSiblingElement();
						}

						data.first = data.second["st_language"];
						this->m_current_language = data.first;
						this->m_data.push_back(data);

						ImGuiIO& io = ImGui::GetIO();

						io.Fonts->AddFontDefault();
						io.Fonts->Build();
					}
					else
					{
						CHECK_OR_EXIT(false, "System error root elements were null!");
					}
				}
				else
				{
					CHECK_OR_EXIT(false, "System error root doesn't exist!");
				}



				return;
			}

#ifdef WINDOWS
			ImGuiIO& io = ImGui::GetIO();
			xr_string filename;
			xr_string path = (SDK_FileSystemState::m_path_rawdata.append(translation_folder)).c_str();
			bool was_found_any_ttf = false;
			bool was_found_any_xml = false;
			for (auto file : std::filesystem::directory_iterator(path.c_str()))
			{
				filename = Helper::converter.to_bytes(file.path().c_str());
 

				if (strstr(filename.c_str(), ".xml"))
				{
					if (this->LoadXml(filename))
						was_found_any_xml = true;
				}

				if ((strstr(filename.c_str(), ".ttf") || strstr(filename.c_str(), ".otf")) && !was_found_any_ttf)
				{
					ImVector<ImWchar> ranges;
					ImFontGlyphRangesBuilder builder;
					builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
					builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
					builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
					builder.AddRanges(io.Fonts->GetGlyphRangesThai());
					builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
					builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
					builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
					builder.BuildRanges(&ranges);
					io.Fonts->AddFontFromFileTTF(filename.c_str(), 13.0f, NULL, ranges.Data);
					io.Fonts->Build();
					was_found_any_ttf = true;
				}
			}


			if (was_found_any_ttf == false || was_found_any_xml == false)
			{
				SDK_FileSystemState::m_is_translationempty = true;
				return this->Initialize();
			}

			this->m_current_language = this->m_data.front().first;
			//io.Fonts->AddFontFromFileTTF()
#endif
		}

		bool SDK_Names::LoadXml(const xr_string& filepath)
		{
			if (filepath.empty())
				return false;

			XML::TiXmlDocument document(filepath.c_str());

			std::pair<xr_string, xr_map<xr_string, xr_string>> data;
			std::pair<xr_string, xr_string> data_map;

			if (document.LoadFile(XML::TIXML_ENCODING_UTF8))
			{
				XML::TiXmlElement* element_root = document.RootElement();

				if (element_root)
				{
					XML::TiXmlElement* element_string = element_root->FirstChildElement();
					if (element_string)
					{
						bool once_found_language = false;
						while (element_string)
						{
							xr_string attribute = u8"";
							xr_string text = u8"";
							attribute.append(element_string->Attribute("id"));
							text.append(element_string->GetText());
							if (!attribute.empty())
							{
								if (attribute == "st_language" && !once_found_language)
								{
									once_found_language = true;
									this->m_languages.push_back(element_string->GetText());
									data.first = this->m_languages.back();
								}

								if (!text.empty())
								{
									xr_string string_id = u8"##ID";
									++m_generated_index;
									string_id += std::to_string(this->m_generated_index).c_str();
									data_map.first = attribute;
									data_map.second = u8"" + text;

									if (!strstr(data_map.first.c_str(), "_text"))
										data_map.second.append(string_id);

									data.second.insert(data_map);
								}
							}


							element_string = element_string->NextSiblingElement();
						}


						this->m_data.push_back(data);
						return true;
					}
				}
			}

			return false;
		}
	}
}