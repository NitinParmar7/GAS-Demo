#include "AMCharacterMovementComponent.h"
#include "Gameframework/Character.h"
#include "../AntiMatterTestCharacter.h"

float UAMCharacterMovementComponent::GetMaxSpeed() const
{
	if (CharacterOwner)
	{
		if (AAntiMatterTestCharacter* Character = Cast<AAntiMatterTestCharacter>(CharacterOwner))
		{
			switch (MovementMode)
			{
			case MOVE_Walking:
				return Character->GetMaxMoveSpeed();
			case MOVE_Falling:
				return Character->GetMaxFallSpeed();
			case MOVE_Swimming:
				return MaxSwimSpeed;
			case MOVE_Flying:
				return Character->GetFlySpeed();
			case MOVE_Custom:
				return MaxCustomMovementSpeed;
			case MOVE_None:
			default:
				return 0.f;
			}
		}
	}
	return 0.f;
}

bool UAMCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	if (CharacterOwner && CharacterOwner->CanJump())
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			if (AAntiMatterTestCharacter* Character = Cast<AAntiMatterTestCharacter>(CharacterOwner))
			{
				Velocity.Z = FMath::Max(Velocity.Z, Character->GetJumpVelocity());
				SetMovementMode(MOVE_Falling);
				return true;
			}
		}
	}

	return false;
}

void UAMCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (AAntiMatterTestCharacter* Character = Cast<AAntiMatterTestCharacter>(CharacterOwner))
	{
		if (Character->bRocketPressed)
		{
			AddForce(FVector(0, 0, RocketVelocity * Mass));
		}
	}
}
