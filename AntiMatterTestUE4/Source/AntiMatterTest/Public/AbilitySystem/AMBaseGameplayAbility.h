
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "AMTypes.h"
#include "Abilities/GameplayAbility.h"
#include "AMBaseGameplayAbility.generated.h"

/**
 *Base Gameplay Ability
 * All ability should inherit this
 * Holds input id and activate on grant if flag is true
 */
UCLASS(BlueprintType, Blueprintable)
class UAMBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bCanBeActivatedOnGrant = false;

	UPROPERTY(EditAnywhere, Category = "Ability")
	EAMInputID InputId = EAMInputID::None;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;


};



