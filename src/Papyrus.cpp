#include "Papyrus.h"
#include "hooks.h"
#include "DropMap.h"

namespace Papyrus
{
	// Give NPC dropped item list for the dropped items to work with mods using `GetDroppedInventory` like quickloot
	void AddDropItemToActor(RE::StaticFunctionTag*, RE::TESObjectREFR* a_item, RE::Actor* a_npc)
	{
		if (!a_npc->extraList.GetByType<RE::ExtraDroppedItemList>()) {
			auto extra = RE::BSExtraData::Create<RE::ExtraDroppedItemList>();
			a_npc->extraList.Add(extra);
		}
		a_npc->extraList.GetByType<RE::ExtraDroppedItemList>()->droppedItemList.emplace_front(a_item);

		DropMap::droppedItemMap.emplace(a_item, a_npc);
	}

	bool Bind(VM* a_vm)
	{
		if (!a_vm) {
			logger::critical("couldn't get VM State"sv);
			return false;
		}

		logger::info("Binding functions..."sv);

		BIND(AddDropItemToActor);

		logger::info("Registered BindFakeObject"sv);

		return true;
	}
}
// CLIB-NG doesn't have correct VTABLE for ExtraDroppedItemList, make sure to add it in if this assertion fails!
static_assert(RE::ExtraDroppedItemList::VTABLE.data() != RE::BSExtraData::VTABLE.data());
