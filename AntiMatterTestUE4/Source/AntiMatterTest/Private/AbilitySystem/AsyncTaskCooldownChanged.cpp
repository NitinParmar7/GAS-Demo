#include "AbilitySystem/AsyncTaskCooldownChanged.h"


UAsyncTaskCooldownChanged * UAsyncTaskCooldownChanged::ListenForCooldownChange(UAbilitySystemComponent * AbilitySystemComponent, FGameplayTag InCooldownTag, bool InUseServerCooldown, float UpdateInterval)
{
	UAsyncTaskCooldownChanged* ListenForCooldownChange = NewObject<UAsyncTaskCooldownChanged>();
	ListenForCooldownChange->ASC = AbilitySystemComponent;
	ListenForCooldownChange->CooldownTag = InCooldownTag;
	ListenForCooldownChange->UseServerCooldown = InUseServerCooldown;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		ListenForCooldownChange->EndTask();
		return nullptr;
	}


	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(ListenForCooldownChange, &UAsyncTaskCooldownChanged::CooldownTagChanged, UpdateInterval);

	FGameplayTagContainer ActiveTags;
	AbilitySystemComponent->GetOwnedGameplayTags(ActiveTags);
	if (ActiveTags.HasTagExact(InCooldownTag))
	{
		ListenForCooldownChange->CooldownTagChanged(InCooldownTag, 1.f, UpdateInterval);
	}

	return ListenForCooldownChange;
}

void UAsyncTaskCooldownChanged::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	SetReadyToDestroy();
	MarkPendingKill();
	if (ASC != nullptr)
	{
		if (ASC->GetWorld() != nullptr)
		{
			ASC->GetWorld()->GetTimerManager().ClearTimer(_TimerHandle);
		}
	}
}

void UAsyncTaskCooldownChanged::UpdateCooldown()
{
	float TimerRemaining = 0.f;
	float CooldownDuration = 0.f;
	if (GetCooldownRemainingForTag(CooldownTag, TimerRemaining, CooldownDuration))
	{
		OnCooldownBegin.Broadcast(CooldownTag, TimerRemaining, CooldownDuration);
	}
}



void UAsyncTaskCooldownChanged::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount, float UpdateInterval)
{
	if (NewCount == 0)
	{
		OnCooldownEnd.Broadcast(InCooldownTag, -1.0f, -1.0f);
	}
	else
	{
		if (ASC != nullptr)
		{
			if (ASC->GetWorld() != nullptr)
			{
				ASC->GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &UAsyncTaskCooldownChanged::UpdateCooldown, UpdateInterval, true);
			}
		}
	}
}

bool UAsyncTaskCooldownChanged::GetCooldownRemainingForTag(FGameplayTag InCooldownTag, float & TimeRemaining, float & CooldownDuration)
{
	if (IsValid(ASC) && InCooldownTag.IsValid())
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(InCooldownTag));
		TArray< TPair<float, float> > DurationAndTimeRemaining = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}

