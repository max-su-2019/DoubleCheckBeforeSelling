#include "Settings.h"

namespace CheckBeforeSelling
{

	static auto MainConfig = COMPILE_PROXY("DoubleCheckBeforeSelling.ini"sv);

	CBS_Settings::CBS_Settings()
	{
		MainConfig.Bind(EnableCheckForEquipped, true);
		MainConfig.Bind(EnableCheckForFavourited, true);
		MainConfig.Bind(EnableCheckForUnique, true);

		MainConfig.Load();

		PrintSettingValue(EnableCheckForEquipped);
		PrintSettingValue(EnableCheckForFavourited);
		PrintSettingValue(EnableCheckForUnique);
	}

}