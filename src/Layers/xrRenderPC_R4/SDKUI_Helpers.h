#pragma once

#include <codecvt>
 
namespace Cordis
{
    namespace SDK
    {	
 

		namespace Helper
		{
			static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		}

    enum SDKErrorType
    {
        default, // white
        warning, // yellow
        error, // red
        good, // green
        special, // dark-blue
        unimportant // gray
    };

    class SDKUI_Log
    {
    private:
        SDKUI_Log(void) noexcept
            : m_is_visible(true), m_is_syscall(false), m_show_errors(true), m_show_warnings(true), m_show_default(true),
              m_show_special(true), m_show_unimportant(true), m_show_good(true), m_currentsize_x(0), m_currentsize_y(0),
              m_currentposition_x(0), m_currentposition_y(0), m_currentcolor(default)
        {
            this->Clear();
        }

    public:
        inline static SDKUI_Log& Widget(void) noexcept
        {
            static SDKUI_Log instance;
            return instance;
        }
        SDKUI_Log(const SDKUI_Log&) = delete;
        SDKUI_Log& operator=(const SDKUI_Log&) = delete;
        SDKUI_Log(SDKUI_Log&&) = delete;
        SDKUI_Log& operator=(SDKUI_Log&&) = delete;
        ~SDKUI_Log(void) noexcept = default;

        void Draw(void);

        inline void Show(void) noexcept { this->m_is_visible = true; }
        inline void Hide(void) noexcept { this->m_is_visible = false; }
        inline bool GetVisible(void) const noexcept { return this->m_is_visible; }
        inline void Init(int x, int y, int SizeX, int SizeY, ImGuiWindowFlags flag = NULL)
        {
            this->m_currentsize_x = SizeX;
            this->m_currentsize_y = SizeY;
            this->m_currentposition_x = x;

            if (this->m_currentposition_x - (this->m_currentsize_x) < 0)
                this->m_currentposition_x = 0;
            else
                this->m_currentposition_x -= this->m_currentsize_x;

            this->m_currentposition_y = y;

            if (this->m_currentposition_y - (this->m_currentsize_y) < 0)
                this->m_currentposition_y = 0;
            else
                this->m_currentposition_y -= this->m_currentsize_y;

            this->m_flag = flag;
        }

        inline void Init(const ImVec2& pos, const ImVec2& size, ImGuiWindowFlags flag = NULL)
        {
            Init(pos.x, pos.y, size.x, size.y, flag);
        }

        inline void Init(const ImVec2& pos, int SizeX, int SizeY, ImGuiWindowFlags flag = NULL)
        {
            Init(pos.x, pos.y, SizeX, SizeY, flag);
        }

        inline void Init(int x, int y, const ImVec2& size, ImGuiWindowFlags flag = NULL)
        {
            Init(x, y, size.x, size.y, flag);
        }

        // @ Use it before call AddText method
        inline void SetColor(SDKErrorType type)
        {
            this->m_currentcolor = type;
            this->m_is_syscall = true;
        }

        inline void AddText(const char* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            this->m_buffer.appendfv(fmt, args);
            xr_string result = u8"[";

			std::chrono::system_clock::time_point x = std::chrono::system_clock::now();
			std::time_t t = std::chrono::system_clock::to_time_t(x);
			xr_string _result_time = std::ctime(&t);
			xr_string r2 = _result_time.substr(_result_time.rfind(' ') - 8);
			r2.erase(r2.rfind(' '));
            result += r2;

            if (!this->m_is_syscall)
                this->m_currentcolor = default;

            if (this->m_currentcolor == SDKErrorType::error)
                result += SDK_Names::getInstance().getName("st_text_log_state_error");
            else if (this->m_currentcolor == SDKErrorType::warning)
                result += SDK_Names::getInstance().getName("st_text_log_state_warning");
            else if (this->m_currentcolor == SDKErrorType::special)
                result += SDK_Names::getInstance().getName("st_text_log_state_info");
            else if (this->m_currentcolor == SDKErrorType::unimportant)
                result += SDK_Names::getInstance().getName("st_text_log_state_old");
            else if (this->m_currentcolor == SDKErrorType::good)
                result += SDK_Names::getInstance().getName("st_text_log_state_successful");

            result += u8"] ";
            result += this->m_buffer.c_str();

            if (result[result.size() - 1] != u8'\n')
                result += u8'\n';

            va_end(args);
            this->m_string_buffer.push_back(result);
            this->m_buffer.clear();
            this->m_is_syscall = false;
        }

        inline void Clear(void)
        {
            this->m_buffer.clear();
            this->m_string_buffer.clear();
            this->m_lineoffset.clear();
            this->m_lineoffset.push_back(0);
        }

    private:
        bool m_is_visible;
        bool m_is_syscall;

        bool m_show_errors;
        bool m_show_warnings;
        bool m_show_default;
        bool m_show_special;
        bool m_show_unimportant;
        bool m_show_good;

        int m_currentsize_x;
        int m_currentsize_y;
        int m_currentposition_x;
        int m_currentposition_y;
        SDKErrorType m_currentcolor;
        ImGuiWindowFlags m_flag;
        ImGuiTextBuffer m_buffer;
        xr_vector<xr_string> m_string_buffer;

        ImVector<int> m_lineoffset;
    };

    class SDKUI_Overlay
    {
    private:
        SDKUI_Overlay(void) noexcept
            : m_is_visible(true), m_showonly_sdkinfo(true), m_showonly_sysinfo(true), m_showonly_projectinfo(true),
              m_flag(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize |
                  ImGuiWindowFlags_NoBackground),
              m_gridtype(0), m_savedposition({0, 0, 0})
        {
        }

    public:
        inline static SDKUI_Overlay& Widget(void) noexcept
        {
            static SDKUI_Overlay instance;
            return instance;
        }

        ~SDKUI_Overlay(void) noexcept = default;
        SDKUI_Overlay(SDKUI_Overlay&) = delete;
        SDKUI_Overlay& operator=(const SDKUI_Overlay&) = delete;
        SDKUI_Overlay(SDKUI_Overlay&&) = delete;
        SDKUI_Overlay& operator=(SDKUI_Overlay&&) = delete;
        void Draw(void);
        inline void Show(void) noexcept { this->m_is_visible = true; }
        inline void Hide(void) noexcept { this->m_is_visible = false; }
        inline bool GetVisible(void) const noexcept { return this->m_is_visible; }

    private:
        bool m_is_visible;
        bool m_showonly_sdkinfo;
        bool m_showonly_sysinfo;
        bool m_showonly_projectinfo;
        int m_gridtype;
        ImGuiWindowFlags m_flag;
        Fvector m_savedposition;
    };

    class SDKUI_CameraHelper
    {
    private:
        SDKUI_CameraHelper(void) noexcept : m_is_visible(false) {}

    public:
        inline static SDKUI_CameraHelper& Widget(void) noexcept
        {
            static SDKUI_CameraHelper instance;
            return instance;
        }
        SDKUI_CameraHelper(const SDKUI_CameraHelper&) = delete;
        SDKUI_CameraHelper& operator=(const SDKUI_CameraHelper&) = delete;
        SDKUI_CameraHelper(SDKUI_CameraHelper&&) = delete;
        SDKUI_CameraHelper& operator=(SDKUI_CameraHelper&&) = delete;
        ~SDKUI_CameraHelper(void) noexcept = default;

        void Draw(void);
        inline void Show(void) noexcept { this->m_is_visible = true; }
        inline void Hide(void) noexcept { this->m_is_visible = false; }
        inline bool GetVisible(void) const noexcept { return this->m_is_visible; }

    private:
        bool m_is_visible;
    };


	struct SDK_BuildingConstants
	{
		static constexpr int kJitterSamplesLow = 1;
		static constexpr int kJitterSamplesMedium = 4;
		static constexpr int kJitterSamplesHigh = 9;
	};

    class SDKUI_SceneOptions
    {
    private:
        SDKUI_SceneOptions(void) noexcept : m_is_visible(false), m_hemisphere_quality(3), m_sun_shadow_quality(3), m_pixel_per_meter(10.0f), m_light_map_collapsing(4),
			m_light_map_zero(4), m_jitter_samples(2), m_normal_smooth_angle(75.0f), m_weld_distance(0.0050f), m_level_prefix("level"), m_flag(ImGuiWindowFlags_AlwaysAutoResize) {}

    public:
        inline static SDKUI_SceneOptions& Widget(void) noexcept
        {
            static SDKUI_SceneOptions instance;
            return instance;
        }
        SDKUI_SceneOptions(const SDKUI_SceneOptions&) = delete;
        SDKUI_SceneOptions& operator=(const SDKUI_SceneOptions&) = delete;
        SDKUI_SceneOptions(SDKUI_SceneOptions&&) = delete;
        SDKUI_SceneOptions& operator=(SDKUI_SceneOptions&&) = delete;
        ~SDKUI_SceneOptions(void);

        inline void Show(void) noexcept { this->m_is_visible = true; }
        inline void Hide(void) noexcept { this->m_is_visible = false; }
        inline bool IsVisible(void) noexcept { return this->m_is_visible; }

        void Draw(void);

    private:
        bool m_is_visible;
		ImGuiWindowFlags m_flag;
		int m_hemisphere_quality;
		int m_sun_shadow_quality;
		int m_light_map_collapsing;
		int m_light_map_zero;
		int m_normal_smooth_angle;
		int m_jitter_samples;
		float m_weld_distance;
		float m_pixel_per_meter;
		char m_level_prefix[30];

    };

    }
}
