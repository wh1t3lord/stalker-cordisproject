#pragma once

namespace Cordis
{
	namespace Scripts
	{
		namespace DataBase
		{
			template<class ComponentType>
			struct Script_AllocatorScheme
			{
			public:
				Script_AllocatorScheme(void) {}
				~Script_AllocatorScheme(void) {}
				Script_AllocatorScheme(const Script_AllocatorScheme&) = delete;
				Script_AllocatorScheme& operator=(const Script_AllocatorScheme&) = delete;
				Script_AllocatorScheme(Script_AllocatorScheme&&) = delete;
				Script_AllocatorScheme& operator=(Script_AllocatorScheme&&) = delete;

				inline void* allocate(const std::uint16_t npc_id) noexcept 
				{
					if (this->m_buffer.find(npc_id) != this->m_buffer.end())
					{
						ComponentType& reference = this->m_buffer.at(npc_id);
						reference.clear();
					}

					ComponentType& instance = this->m_buffer[npc_id];

					return static_cast<void*>(&instance);
				}

			private:
				xr_map<std::uint16_t, ComponentType> m_buffer;
			};
		}
	}
}