#include "AMSlot.h"
#include "UObject/NoExportTypes.h"



AAMItem* UAMCharacterSlot::GetCurrentItem()
{
	return Item;
}

bool UAMCharacterSlot::EquipItem(AAMItem* ItemToEquip)
{
	if (IsValid(ItemToEquip))
	{
		if(IsValid(Item))
		{ 
			if (!UnequipItem(Item))
			{
				return false;
			}
		}

		if (ItemToEquip->TryEquipingItem(CharacterOwner))
		{
			SlotState = ESlotState::Used;
			Item = ItemToEquip;
			return true;
		}
	}
	return false;
}

bool UAMCharacterSlot::UnequipItem(AAMItem* ItemToUnequip)
{
	if (IsValid(ItemToUnequip) && IsValid(Item) && ItemToUnequip == Item)
	{
		if (Item->TryUnequipingItem())
		{
			SlotState = ESlotState::Free;
			Item = nullptr;
			return true;
		}
	}
	return false;
}

void UAMCharacterSlot::SetCharacterOwner(TWeakObjectPtr<AAntiMatterTestCharacter> Character)
{
	CharacterOwner = Character;
}
