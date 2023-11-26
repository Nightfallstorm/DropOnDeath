#pragma once

#include "DropMap.h"

namespace hooks
{

	static void handlePickUpDroppedObject(RE::TESObjectREFR* a_object)
	{
		if (!DropMap::droppedItemMap.contains(a_object)) {
			return;
		}

		auto& list = DropMap::droppedItemMap.at(a_object)->extraList.GetByType<RE::ExtraDroppedItemList>()->droppedItemList;
		auto newList = RE::BSSimpleList<RE::ObjectRefHandle>();
		for (auto& entry : list) {
			if (entry.get()->formID == a_object->formID) {
				continue;
			}
			newList.emplace_front(entry);
		}

		list.clear();
		DropMap::droppedItemMap.at(a_object)->extraList.GetByType<RE::ExtraDroppedItemList>()->droppedItemList = newList;
		DropMap::droppedItemMap.erase(a_object);
	}

	struct PickupHook
	{
		static void thunk(RE::Actor* a_self, RE::TESObjectREFR* a_object, std::int32_t a_count, bool a_arg3, bool a_playSound)
		{
			handlePickUpDroppedObject(a_object);
			return func(a_self, a_object, a_count, a_arg3, a_playSound);
		}

		static inline std::uint32_t idx = 0xCC;

		static inline REL::Relocation<decltype(thunk)> func;

		// Install our hook at the specified address
		static inline void Install()
		{
			if (REL::Module::IsVR()) {
				idx = 0xCE;
			}
			stl::write_vfunc<RE::Character, PickupHook>();

			logger::info("PickupHook set!");
		}
	};

	struct PickupPlayerHook
	{
		static void thunk(RE::Actor* a_self, RE::TESObjectREFR* a_object, std::int32_t a_count, bool a_arg3, bool a_playSound)
		{
			handlePickUpDroppedObject(a_object);
			return func(a_self, a_object, a_count, a_arg3, a_playSound);
		}

		static inline std::uint32_t idx = 0xCC;

		static inline REL::Relocation<decltype(thunk)> func;

		// Install our hook at the specified address
		static inline void Install()
		{
			if (REL::Module::IsVR()) {
				idx = 0xCE;
			}
			stl::write_vfunc<RE::PlayerCharacter, PickupHook>();

			logger::info("PickupHook set!");
		}
	};

	static inline void Install()
	{
		PickupHook::Install();
		PickupPlayerHook::Install();
	}
}
