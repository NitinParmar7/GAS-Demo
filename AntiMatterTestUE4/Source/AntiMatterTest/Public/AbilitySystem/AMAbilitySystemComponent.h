
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AMAbilitySystemComponent.generated.h"

/**
 *AbilitySystemComponent for Characters so we can modify as per project needs
 */
UCLASS(BlueprintType, Blueprintable)
class UAMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	/**
	 * Set Starting ability flag to true
	 */
	void SetHasStartingAbilities(bool bStartingAbilties);

	/**
	 * Return true if starting abilities are given
	 */
	bool HasStartingAbilities();

	/*
	* Get Input component associated  with ASC
	*/
	UInputComponent* GetInputComponent();

	/**
	 * Binds Input Actions and store input component reference so we can use it in ability task
	 */
	virtual void BindToInputComponent(UInputComponent* InputComponent) override;

protected:

private:

	UPROPERTY()
	bool bhasStartingAbilities = false;


	UPROPERTY()
	UInputComponent* ASCInputComponent;

};





