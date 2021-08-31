
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/AMBaseGameplayAbility.h"
#include "AMTypes.h"
#include "Components/WidgetComponent.h"
#include "AMItems.generated.h"

class AAntiMatterTestCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemStateChanged, int, ItemID, EItemState, NewState);


/**
 * Class to hold Items 
 * Each items can provide single ability and single gameplay effect (buffs debuffs)
 */
UCLASS(BlueprintType, Blueprintable)
class AAMItem : public AActor
{
	GENERATED_BODY()

public:

	AAMItem();

	UPROPERTY(BlueprintAssignable, Category = Items)
	FOnItemStateChanged OnItemStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	FName ItemName;

	UPROPERTY(EditAnywhere, Category = Items)
	TSubclassOf<UAMBaseGameplayAbility> ItemAbility;

	UPROPERTY(EditAnywhere, Category = Items)
	TSubclassOf<UGameplayEffect> ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items)
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items)
	ESlotType ItemAllowedOnSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Items)
	EItemState ItemState = EItemState::Free;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Items)
	FGameplayTag ItemAbilityCooldownTag;

	/*
	* Equip Item to a slot
	*/
	bool TryEquipingItem(TWeakObjectPtr<AAntiMatterTestCharacter> Owner);

	/*
	* UnEquip Item to a slot
	*/
	bool TryUnequipingItem();

	/*
	* Returns Item Owner
	*/
	UFUNCTION(BlueprintPure, Category = Items)
	ACharacter* GetItemOwner() const;


	virtual void Tick(float DeltaSeconds) override;

protected:


	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	UWidgetComponent* ItemWidget;

	UPROPERTY()
	TWeakObjectPtr<AActor> ActorToLookAt;

	UFUNCTION()
	void OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	TWeakObjectPtr<AAntiMatterTestCharacter> ItemOwner;

	UPROPERTY()
	FGameplayAbilitySpecHandle	AbilitySpecHandle;

	UPROPERTY()
		FActiveGameplayEffectHandle ActiveEffect;

};



