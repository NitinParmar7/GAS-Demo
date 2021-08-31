// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntiMatterTestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <GameplayAbilitySpec.h>
#include "AbilitySystem/AMBaseGameplayAbility.h"
#include "AMPlayerController.h"
#include "AMCharacterMovementComponent.h"

//////////////////////////////////////////////////////////////////////////
// AAntiMatterTestCharacter


AAntiMatterTestCharacter::AAntiMatterTestCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UAMCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AbilitySystem = CreateDefaultSubobject<UAMAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UAMAttributeSet>(TEXT("AttributeSet"));
}

void AAntiMatterTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
	GiveStartingAbilities();
	for (auto& Element : StartingSlots)
	{
		UAMCharacterSlot* Slot = NewObject<UAMCharacterSlot>(this, Element.Value);
		Slot->SetCharacterOwner(this);
		Slots.Add(Element.Key, Slot);
	}
	if (AAMPlayerController* PC = Cast<AAMPlayerController>(GetController()))
	{
		PC->CreateHUD();
	}
	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

void AAntiMatterTestCharacter::EquipUnequipItemsInternal(int index)
{
	auto ItemPtrPtr = Inventory.FindByPredicate([index](AAMItem* Item) { return Item->ItemID == index; });
	if (ItemPtrPtr != nullptr)
	{
		AAMItem* ItemToUse = *ItemPtrPtr;
		ESlotType SlotType = ItemToUse->ItemAllowedOnSlot;
		if (Slots.Contains(SlotType))
		{
			UAMCharacterSlot* Slot = Slots[SlotType];
			if (Slot != nullptr)
			{
				if (ItemToUse->ItemState == EItemState::Unequipped)
				{
					Slot->EquipItem(ItemToUse);
				}
				else if(ItemToUse->ItemState == EItemState::Equipped)
				{
					Slot->UnequipItem(ItemToUse);
				}
			}
		}
	}
}

void AAntiMatterTestCharacter::ProcessAbilityInputPressed()
{
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->AbilityLocalInputPressed(static_cast<int32>(EAMInputID::Jump));
	}
}

void AAntiMatterTestCharacter::ProcessAbilityInputReleased()
{
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->AbilityLocalInputReleased(static_cast<int32>(EAMInputID::Jump));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAntiMatterTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	/*PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);*/

	PlayerInputComponent->BindAxis("MoveForward", this, &AAntiMatterTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAntiMatterTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAntiMatterTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAntiMatterTestCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("ToggleItem1", IE_Pressed, this, &AAntiMatterTestCharacter::EquipUnequipItems<1>);
	PlayerInputComponent->BindAction("ToggleItem2", IE_Pressed, this, &AAntiMatterTestCharacter::EquipUnequipItems<2>);
	PlayerInputComponent->BindAction("ToggleItem3", IE_Pressed, this, &AAntiMatterTestCharacter::EquipUnequipItems<3>);
	PlayerInputComponent->BindAction("ToggleItem4", IE_Pressed, this, &AAntiMatterTestCharacter::EquipUnequipItems<4>);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAntiMatterTestCharacter::ProcessAbilityInputPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAntiMatterTestCharacter::ProcessAbilityInputReleased);


	if (AbilitySystem != nullptr)
	{
		AbilitySystem->BindToInputComponent(PlayerInputComponent);
	}

	BindAbilitySystemInputs(PlayerInputComponent);
}


UAbilitySystemComponent* AAntiMatterTestCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}


bool AAntiMatterTestCharacter::AddItemToInventory(AAMItem* Item)
{
	if (Inventory.AddUnique(Item) != INDEX_NONE)
	{
		Item->ItemState = EItemState::Unequipped;
		OnItemAdded.Broadcast(Item);
		return true;
	}
	return false;
}


void AAntiMatterTestCharacter::StartRocket()
{
	bRocketPressed = true;

}

void AAntiMatterTestCharacter::StopRocket()
{
	bRocketPressed = false;

}

float AAntiMatterTestCharacter::GetJumpVelocity() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetJumpZVelocity();
	}
	return 0;
}

float AAntiMatterTestCharacter::GetMaxMoveSpeed() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMoveSpeed();
	}
	return 0;
}

float AAntiMatterTestCharacter::GetMaxFallSpeed() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetFallSpeed();
	}
	return 0;
}

float AAntiMatterTestCharacter::GetFlySpeed() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetFlySpeed();
	}
	return 0;
}

void AAntiMatterTestCharacter::InitializeAttributes()
{
	if (AbilitySystem == nullptr || Attributes == nullptr)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(Attributes, 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

}

void AAntiMatterTestCharacter::GiveStartingAbilities()
{
	if (!IsValid(AbilitySystem) || AbilitySystem->HasStartingAbilities())
	{
		return;
	}

	for (TSubclassOf<UAMBaseGameplayAbility>& Ability : StartingAbilities)
	{
		AbilitySystem->GiveAbility(
			FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->InputId), this));
	}

}

void AAntiMatterTestCharacter::BindAbilitySystemInputs(class UInputComponent* PlayerInputComponent)
{
	if (!bHasAbilitySystemInputBound && IsValid(AbilitySystem) && IsValid(PlayerInputComponent))
	{
		AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EAMInputID"), static_cast<int32>(EAMInputID::Confirm), static_cast<int32>(EAMInputID::Cancel)));
		bHasAbilitySystemInputBound = true;
	}
}

void AAntiMatterTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAntiMatterTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAntiMatterTestCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAntiMatterTestCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
