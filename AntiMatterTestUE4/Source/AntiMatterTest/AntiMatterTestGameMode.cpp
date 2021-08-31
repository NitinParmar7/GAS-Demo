// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntiMatterTestGameMode.h"
#include "AntiMatterTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAntiMatterTestGameMode::AAntiMatterTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
