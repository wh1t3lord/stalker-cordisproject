#pragma once

#include "Script_GlobalHelper.h"

namespace Cordis
{
	namespace Scripts
	{
		struct ClassRegistrator
		{
		private:
			ClassRegistrator(void) = default;

		public:
			inline static ClassRegistrator& getInstance(void) noexcept
			{
				static ClassRegistrator instance;
				return instance;
			}


			inline void Register(void)
			{

			}
		};
	}
}