// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AMAbilitySystemComponent.h"
#include "AbilitySystem/AMAttributeSet.h"
#include "AMItems.h"
#include "AMSlot.h"
#include "AntiMatterTestCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, AAMItem*, Item);

/**
 * Our base character which can hold items on slots
 */
UCLASS(config=Game)
class AAntiMatterTestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	class UAMAbilitySystemComponent * AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	class UAMAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slots, meta = (AllowPrivateAccess = "true"))
	TMap<ESlotType, UAMCharacterSlot*> Slots;

	bool bHasAbilitySystemInputBound = false;

public:

	AAntiMatterTestCharacter(const class FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, Category = Attributes)
	TSubclassOf<class UGameplayEffect> Attributes;

	UPROPERTY(EditAnywhere, Category = Ability)
	TArray<TSubclassOf<class UAMBaseGameplayAbility>> StartingAbilities;

	UPROPERTY(EditAnywhere, Category = Items)
	TArray<AAMItem*> Inventory;

	UPROPERTY(EditAnywhere, Category = Slots)
	TMap<ESlotType,TSubclassOf<UAMCharacterSlot>> StartingSlots;

	UPROPERTY(BlueprintAssignable, Category = Items)
	FOnItemAdded OnItemAdded;

protected:

	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void InitializeAttributes();

	void GiveStartingAbilities();

	void BindAbilitySystemInputs(class UInputComponent* PlayerInputComponent);

	template<int index>
	void EquipUnequipItems() 
	{
		EquipUnequipItemsInternal(index);
	}

	void EquipUnequipItemsInternal(int index);

	void ProcessAbilityInputPressed();

	void ProcessAbilityInputReleased();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddItemToInventory(AAMItem* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void StartRocket();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void StopRocket();

	float GetJumpVelocity() const;

	float GetMaxMoveSpeed() const;

	float GetMaxFallSpeed() const;

	float GetFlySpeed() const;

	bool bRocketPressed = false;

};



