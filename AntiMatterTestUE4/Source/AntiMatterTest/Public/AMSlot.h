
#pragma once

#include "CoreMinimal.h"
#include "AMItems.h"
#include "AMSlot.generated.h"

/**
 *Slots to hold Item 
 * We are only allowing on type of slot
 */
UCLASS(BlueprintType, Blueprintable)
class UAMCharacterSlot : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Slot)
	ESlotType SlotType;

	UFUNCTION(BlueprintPure, Category = Slot)
	AAMItem* GetCurrentItem();

	UFUNCTION(BlueprintCallable, Category = Slot)
	bool EquipItem(AAMItem* ItemToEquip);

	UFUNCTION(BlueprintCallable, Category = Slot)
	bool UnequipItem(AAMItem* ItemToUnequip);

	void SetCharacterOwner(TWeakObjectPtr<AAntiMatterTestCharacter> Character);

protected:

	UPROPERTY(VisibleAnywhere, Category = Slots)
	AAMItem* Item;

	UPROPERTY(VisibleAnywhere, Category = Slots)
	ESlotState SlotState = ESlotState::Free;

	UPROPERTY(VisibleAnywhere, Category = Slots)
	TWeakObjectPtr<AAntiMatterTestCharacter> CharacterOwner;

};


