#include "AbilitySystem/AbilityTask_BindAndListenToKey.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "Engine/EngineBaseTypes.h"
#include "AbilitySystem/AMAbilitySystemComponent.h"



UAbilityTask_BindAndListenToKey* UAbilityTask_BindAndListenToKey::BindAndListenToKey(UGameplayAbility* OwningAbility, FKey Key)
{
	UAbilityTask_BindAndListenToKey* AbilityTask = NewAbilityTask<UAbilityTask_BindAndListenToKey>(OwningAbility);
	AbilityTask->_Key = Key;
	return AbilityTask;
}

void UAbilityTask_BindAndListenToKey::Activate()
{
	Super::Activate();
	IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(GetAvatarActor());
	if (Interface != nullptr)
	{
		if (UAMAbilitySystemComponent* PASC = Cast<UAMAbilitySystemComponent>(Interface->GetAbilitySystemComponent()))
		{
			if (PASC->GetInputComponent() != nullptr)
			{
				PASC->GetInputComponent()->BindKey(_Key, EInputEvent::IE_Pressed, this, &UAbilityTask_BindAndListenToKey::OnInputKeyPressed).bConsumeInput = 0;
				PASC->GetInputComponent()->BindKey(_Key, EInputEvent::IE_Released, this, &UAbilityTask_BindAndListenToKey::OnInputKeyReleased).bConsumeInput = 0;
			}
		}
	}
}


void UAbilityTask_BindAndListenToKey::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);


}

void UAbilityTask_BindAndListenToKey::OnInputKeyPressed()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnKeyPressed.Broadcast();
	}
}

void UAbilityTask_BindAndListenToKey::OnInputKeyReleased()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnKeyReleased.Broadcast();
	}
}
