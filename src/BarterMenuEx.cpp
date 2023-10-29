#include "BarterMenuEx.h"
#include "MessageBox.h"

namespace CheckBeforeSelling
{
	void BarterMenuEx::Hook_PostCreate()
	{
		func(this);

		RE::GFxValue oldFunc, func;
		bool success = this->root.GetMember("onItemSelect", &oldFunc);
		assert(success);

		auto impl_1 = RE::make_gptr<ItemSelectHandler>(oldFunc);
		this->uiMovie->CreateFunction(&func, impl_1.get());
		this->root.SetMember("onItemSelect", func);  //Replace Vanilla Method

		auto impl_2 = RE::make_gptr<SellingConfirmHandler>(oldFunc);
		this->uiMovie->CreateFunction(&func, impl_2.get());
		this->root.SetMember("Maxsu_OnSellingConfirm", func);
	}

	void BarterMenuEx::SellingConfirmHandler::Call(RE::GFxFunctionHandler::Params& a_params)
	{
		oldFunc_.Invoke("call", a_params.retVal, a_params.argsWithThisRef, a_params.argCount + 1);
	}

	void BarterMenuEx::ItemSelectHandler::Call(RE::GFxFunctionHandler::Params& a_params)
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::BarterMenu>();
		if (!menu) {
			ERROR("BarterMenu Not Found!");
			return;
		}

		if (menu->IsViewingVendorItems()) {
			oldFunc_.Invoke("call", a_params.retVal, a_params.argsWithThisRef, a_params.argCount + 1);
			return;
		}

		auto selectedItem = menu->itemList->GetSelectedItem();
		auto itemType = GetItemType(selectedItem);
		if (selectedItem && itemType != ItemType::kNone && selectedItem->data.GetEnabled()) {
			RE::GFxValue keyboardOrMouse;
			a_params.args[0].GetMember("keyboardOrMouse", &keyboardOrMouse);
			const double keyboardOrMouseVal = keyboardOrMouse.GetNumber();

			SkyrimScripting::ShowMessageBox(
				GetConfirmMessageText(itemType),
				{ "Yes", "No" },
				[menu, keyboardOrMouseVal](unsigned int a_result) {
					if (a_result == 0) {
						RE::GFxValue obj;
						menu->uiMovie->CreateObject(&obj);
						obj.SetMember("entry", menu->itemList->GetSelectedItem()->obj);
						RE::GFxValue keyboardOrMouse;
						keyboardOrMouse.SetNumber(keyboardOrMouseVal);
						obj.SetMember("keyboardOrMouse", keyboardOrMouse);
						menu->root.Invoke("Maxsu_OnSellingConfirm", nullptr, &obj, 1);
					} else {
						menu->itemList->Update();
					}
				});
		} else {
			oldFunc_.Invoke("call", a_params.retVal, a_params.argsWithThisRef, a_params.argCount + 1);
		}
	}

	const std::string BarterMenuEx::GetConfirmMessageText(ItemType a_type)
	{
		switch (a_type) {
		case ItemType::kEquipped:
			return R"(Sell this item you are currently equipped?)";

		case ItemType::kFavorite:
			return R"(Sell this item you favorited?)";

		case ItemType::kArtifact:
			return R"(Sell this artifact item?)";

		default:
			return "Error Item Type!";
		}
	}

	BarterMenuEx::ItemType BarterMenuEx::GetItemType(RE::ItemList::Item* a_item)
	{
		if (!a_item) {
			return ItemType::kNone;
		}

		if (a_item->data.GetEquipState() > 0) {
			return ItemType::kEquipped;
		}

		if (a_item->data.GetFavorite()) {
			return ItemType::kFavorite;
		}

		return ItemType::kNone;
	}

}