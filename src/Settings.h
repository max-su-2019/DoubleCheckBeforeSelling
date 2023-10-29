#pragma once
#include "DKUtil/Config.hpp"
#include "DKUtil/Utility.hpp"

namespace CheckBeforeSelling
{
	using namespace DKUtil::Alias;

	class CBS_Settings : public DKUtil::model::Singleton<CBS_Settings>
	{
		friend DKUtil::model::Singleton<CBS_Settings>;

	private:
		CBS_Settings();

		template <class T>
		static void PrintSettingValue(const T& a_setting)
		{
			INFO("Setting:\"{}\" is {}"sv, a_setting.get_key(), a_setting.get_data());
		}

	public:
		Boolean EnableCheckForEquipped{ "EnableCheckForEquipped", "Features" };
		Boolean EnableCheckForFavourited{ "EnableCheckForFavourited", "Features" };
		Boolean EnableCheckForUnique{ "EnableCheckForUnique", "Features" };
	};
}