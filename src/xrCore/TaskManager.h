#pragma once

namespace Cordis
{
	// TODO: это также удалить ибо он уже есть
	class XRCORE_API TaskManager
	{
	private:
		TaskManager(void) {}

	public:
		inline static TaskManager& getInstance(void) noexcept { static TaskManager instance; return instance; }
		~TaskManager(void) { xr_delete(this->m_p_sheduler); }
		TaskManager(const TaskManager&) = delete;
		TaskManager& operator=(const TaskManager&) = delete;
		TaskManager(TaskManager&&) = delete;
		TaskManager& operator=(TaskManager&&) = delete;

		inline bool initialize(void) noexcept 
		{
			this->m_p_sheduler = new tbb::task_scheduler_init(this->DetectCores());
			return true;
		}

		inline tbb::task_group* getCore(void) noexcept { return &this->m_Core; }
		inline tbb::task_group* getRender(void) noexcept { return &this->m_Render; }
		inline tbb::task_group* getEngine(void) noexcept { return &this->m_Engine; }
		inline tbb::task_group* getGame(void) noexcept { return &this->m_Game; }
		inline tbb::task_group* getPhysics(void) noexcept { return &this->m_Physics; }
		inline tbb::task_group* getAnimation(void) noexcept { return &this->m_Animation; }

	private:
		inline int DetectCores(void) noexcept 
		{
#ifdef _WIN32 
			SYSTEM_INFO info;
			GetSystemInfo(&info);
			return info.dwNumberOfProcessors;
#endif

			return std::uint32_t(2);
		}

	private:
		tbb::task_scheduler_init* m_p_sheduler;
		tbb::task_group m_Core;
		tbb::task_group m_Render;
		tbb::task_group m_Engine;
		tbb::task_group m_Game;
		tbb::task_group m_Physics;
		tbb::task_group m_Animation;
	};
}