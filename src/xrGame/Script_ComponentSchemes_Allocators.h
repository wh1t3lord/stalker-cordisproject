#pragma once

namespace Cordis
{
	namespace Scripts
	{
		namespace DataBase
		{
			struct Script_AllocatorScheme_PHButton
			{
			private:
				Script_AllocatorScheme_PHButton(void) {}

			public:
				inline static Script_AllocatorScheme_PHButton& getInstance(void) noexcept { static Script_AllocatorScheme_PHButton instance; return instance; }
				~Script_AllocatorScheme_PHButton(void) {}

				inline void* allocate(const std::uint16_t npc_id) noexcept 
				{
					if (this->m_buffer.find(npc_id) != this->m_buffer.end())
					{
						Script_ComponentScheme_PHButton& reference = this->m_buffer.at(npc_id);
						reference.clear();
					}

					Script_ComponentScheme_PHButton& instance = this->m_buffer[npc_id];

					return static_cast<void*>(&instance);
				}

			private:
				xr_map<std::uint16_t, Script_ComponentScheme_PHButton> m_buffer;
			};
		}
	}
}