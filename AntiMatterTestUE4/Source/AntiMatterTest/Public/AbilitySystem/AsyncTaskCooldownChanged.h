#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AsyncTaskCooldownChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChanged, FGameplayTag, CooldownTag, float, TimeRemaining, float, Duration);

/**
 * Blueprint node to automatically register a listener for changes (Begin and End) to an array of Cooldown tags.
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class UAsyncTaskCooldownChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/**
	 * Called when cool down begins
	 */
	UPROPERTY(BlueprintAssignable)
		FOnCooldownChanged OnCooldownBegin;

	/**
	 * Called when cool down ends
	 */
	UPROPERTY(BlueprintAssignable)
		FOnCooldownChanged OnCooldownEnd;

	// Listens for changes (Begin and End) to cooldown GameplayEffects based on the cooldown tag.
	// UseServerCooldown determines if the Sever's cooldown is returned in addition to the local predicted cooldown.
	// If using ServerCooldown, TimeRemaining and Duration will return -1 to signal local predicted cooldown has begun.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncTaskCooldownChanged* ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag CooldownTag, bool UseServerCooldown, float UpdateInterval = 1.f);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
		void EndTask();

protected:

	/**
	 * Reference to AbilitySystem component
	 */
	UPROPERTY()
		UAbilitySystemComponent* ASC;

	/**
	 * Cooldown of tag to check
	 */
	FGameplayTag CooldownTag;

	/**
	 * true if show server cooldown
	 */
	bool UseServerCooldown;

	FTimerHandle _TimerHandle;

	void UpdateCooldown();

	/**
	 * called when cooldown change
	 *
	 * @param		CooldownTag
	 * @param		NewCount
	 * @return		void
	 *
	 */
	virtual void CooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount, float UpdateInterval);

	/**
	 * get remaining time for cooldown
	 *
	 * @param		CooldownTags
	 * @param		TimeRemaining
	 * @param		CooldownDuration
	 * @return		bool
	 *
	 */
	bool GetCooldownRemainingForTag(FGameplayTag CooldownTag, float& TimeRemaining, float& CooldownDuration);
};