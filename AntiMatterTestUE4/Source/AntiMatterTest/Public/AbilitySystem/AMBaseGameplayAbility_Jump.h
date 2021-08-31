
#pragma once

#include "CoreMinimal.h"
#include "AMBaseGameplayAbility.h"
#include "AMBaseGameplayAbility_Jump.generated.h"

/**
 * Our base jump ability 
 * Reference taken from GameplayAbility_CharacterJump
 * this one hold input id associated with it
 */

UCLASS()
class UAMBaseGameplayAbility_Jump : public UAMBaseGameplayAbility
{
	GENERATED_BODY()

public:

	UAMBaseGameplayAbility_Jump();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
};
