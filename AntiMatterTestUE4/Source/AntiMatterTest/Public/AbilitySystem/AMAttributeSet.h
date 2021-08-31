
#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AMAttributeSet.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attribute sets for Characters It will hold attribute like MoveSpeed and JumpSpeed etc
 * They can hold base value (original) and current value (modified)
 */
UCLASS()
class UAMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAMAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FlySpeed;
	ATTRIBUTE_ACCESSORS(UAMAttributeSet, FlySpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FallSpeed;
	ATTRIBUTE_ACCESSORS(UAMAttributeSet, FallSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData JumpZVelocity;
	ATTRIBUTE_ACCESSORS(UAMAttributeSet, JumpZVelocity);


};



