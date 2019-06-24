#pragma once

namespace Cordis
{
	namespace SDK
	{
		class SDKUI_Settings
		{
		private:
			SDKUI_Settings(void) : m_is_visible(false), m_flag(ImGuiWindowFlags_AlwaysAutoResize)
			{

			}
		public:
			inline static SDKUI_Settings& Widget(void) noexcept
			{
				static SDKUI_Settings instance;
				return instance;
			}

			SDKUI_Settings(const SDKUI_Settings&) = delete;
			SDKUI_Settings& operator=(const SDKUI_Settings&) = delete;
			SDKUI_Settings(SDKUI_Settings&&) = delete;
			SDKUI_Settings& operator=(SDKUI_Settings&&) = delete;
			~SDKUI_Settings(void);

			void Draw(void);

			inline bool IsVisible(void) noexcept { return this->m_is_visible; }
			inline void Show(void) noexcept { this->m_is_visible = true; }
			inline void Hide(void) noexcept { this->m_is_visible = false; }
		private:
			bool m_is_visible;
			ImGuiWindowFlags m_flag;
		};

	}
}

