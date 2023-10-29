#include "LoadGame.h"
#include "BarterMenuEx.h"
#include "Settings.h"

namespace CheckBeforeSelling
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
			CBS_Settings::GetSingleton();
			SKSE::Translation::ParseTranslation(Plugin::NAME.data());
			BarterMenuEx::InstallHook();
		}
	}

}
