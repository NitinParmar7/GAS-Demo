#include "AbilitySystem/AMAbilitySystemComponent.h"


void UAMAbilitySystemComponent::SetHasStartingAbilities(bool bStartingAbilties)
{
	bhasStartingAbilities = bStartingAbilties;
}

bool UAMAbilitySystemComponent::HasStartingAbilities()
{
	return bhasStartingAbilities;
}

UInputComponent* UAMAbilitySystemComponent::GetInputComponent()
{
	return ASCInputComponent;
}

void UAMAbilitySystemComponent::BindToInputComponent(UInputComponent* InputComponent)
{
	Super::BindToInputComponent(InputComponent);
	this->ASCInputComponent = InputComponent;
}
