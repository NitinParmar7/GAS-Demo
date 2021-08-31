
#pragma once

#include "CoreMinimal.h"
#include "AMTypes.generated.h"

// All extra enums which needed in multiple files should be here

UENUM(BlueprintType)
enum class EAMInputID : uint8
{
	None UMETA(DisplayName="None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Ability1 UMETA(DisplayName = "Ability1"),
	Ability2 UMETA(DisplayName = "Ability2"),
	Jump UMETA(DisplayName = "Jump")
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	None UMETA(DisplayName = "None"),
	Back UMETA(DisplayName="Back"),
	Foot UMETA(DisplayName="Foot")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Free UMETA(DisplayName = "Free"),
	Equipped UMETA(DisplayName = "Equipped"),
	Unequipped UMETA(DisplayName = "Unequipped")
};

UENUM(BlueprintType)
enum class ESlotState : uint8
{
	Free UMETA(DisplayName = "Free"),
	Used UMETA(DisplayName = "Used")
};
