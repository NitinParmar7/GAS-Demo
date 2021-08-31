
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_BindAndListenToKey.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyReleased);

/**
 *Task to Bind Action to inputcomponent and listen to events in blueprint
 */
UCLASS(BlueprintType, Blueprintable)
class UAbilityTask_BindAndListenToKey : public UAbilityTask
{
	GENERATED_BODY()

public:

	/**
	 * Called when Key is Pressed
	 */
	UPROPERTY(BlueprintAssignable)
		FOnKeyPressed OnKeyPressed;

	/**
	 * Called when Key is Released
	 */
	UPROPERTY(BlueprintAssignable)
		FOnKeyReleased OnKeyReleased;

private:

	/**
	 * Reference to key
	 */
	FKey _Key;


public:

	/**
	 * Creates the AbilityTask
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UAbilityTask_BindAndListenToKey* BindAndListenToKey(UGameplayAbility* OwningAbility, FKey Key);


protected:

	/**
	 * Called when Task is activated
	 */
	virtual void Activate() override;



private:

	/**
	 * Called when task is destroyed
	 */
	virtual void OnDestroy(bool bInOwnerFinished) override;

	/**
	 * Called when Key is pressed
	 */
	UFUNCTION()
		void OnInputKeyPressed();

	/**
	 * Called when key is released
	 */
	UFUNCTION()
		void OnInputKeyReleased();


};




