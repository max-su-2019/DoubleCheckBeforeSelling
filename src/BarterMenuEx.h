#pragma once

namespace CheckBeforeSelling
{
	class BarterMenuEx : public RE::BarterMenu
	{
	public:
		class ItemSelectHandler : public RE::GFxFunctionHandler
		{
		public:
			enum class ItemType : std::uint32_t
			{
				kNone = 0,
				kEquipped,
				kFavorite,
				kArtifact
			};

			ItemSelectHandler(const RE::GFxValue& a_oldFunc) :
				oldFunc_{ a_oldFunc } {}

			// override (RE::GFxFunctionHandler)
			void Call(RE::GFxFunctionHandler::Params& a_params) override;

		private:
			RE::GFxValue oldFunc_;
		};
		using ItemType = ItemSelectHandler::ItemType;

		class SellingConfirmHandler : public RE::GFxFunctionHandler
		{
		public:
			SellingConfirmHandler(const RE::GFxValue& a_oldFunc) :
				oldFunc_{ a_oldFunc } {}

			// override (RE::GFxFunctionHandler)
			void Call(RE::GFxFunctionHandler::Params& a_params) override;

		private:
			RE::GFxValue oldFunc_;
		};

		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> Vtbl{ VTABLE[0] };
			func = Vtbl.write_vfunc(0x2, &Hook_PostCreate);
			INFO("Hook BarterMenu!");
		}

		static ItemType GetItemType(RE::ItemList::Item* a_item);
		static const std::string GetConfirmMessageText(ItemType a_type);

	private:
		void Hook_PostCreate();

		static inline REL::Relocation<decltype(&RE::BarterMenu::PostCreate)> func;
	};
}