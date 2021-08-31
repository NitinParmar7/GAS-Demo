
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AMCharacterMovementComponent.generated.h"

/**
 *Custom Characters Movement Component
 * It use attributes instead of using inbuilt values
 * so that way we can affect our movement with abilities
 */
UCLASS(BlueprintType, Blueprintable)
class UAMCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = CharacterMovementComponent)
	float RocketVelocity;

	virtual float GetMaxSpeed() const override;

	virtual bool DoJump(bool bReplayingMoves) override;


	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};


