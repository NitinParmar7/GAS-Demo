#include "AMItems.h"
#include "../AntiMatterTestCharacter.h"


AAMItem::AAMItem()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

bool AAMItem::TryEquipingItem(TWeakObjectPtr<AAntiMatterTestCharacter> CharacterOwner)
{
	bool bEquipped = false;
	if (CharacterOwner.IsValid())
	{
		
			if (UAbilitySystemComponent* AbilitySystem = Cast<UAbilitySystemComponent>(CharacterOwner.Get()->GetAbilitySystemComponent()))
			{
				if (IsValid(ItemAbility))
				{
					AbilitySpecHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(ItemAbility, 1, static_cast<int32>(ItemAbility.GetDefaultObject()->InputId), CharacterOwner.Get()));
					if (AbilitySpecHandle.IsValid())
					{
						ItemState = EItemState::Equipped;
						ItemOwner = CharacterOwner;
						OnItemStateChanged.Broadcast(ItemID, ItemState);
						bEquipped = true;
					}
				}

				if (IsValid(ItemEffect))
				{
					FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
					FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(ItemEffect, 1, EffectContext);
					ActiveEffect = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					bEquipped = true;
				}
			}
	}
	return bEquipped;
}

bool AAMItem::TryUnequipingItem()
{
	bool bUnequipped = false;
	if (ItemOwner.IsValid())
	{
		if (UAbilitySystemComponent* AbilitySystem = Cast<UAbilitySystemComponent>(ItemOwner.Get()->GetAbilitySystemComponent()))
		{
			if (AbilitySpecHandle.IsValid())
			{
				AbilitySystem->ClearAbility(AbilitySpecHandle);
					ItemOwner.Reset();
					ItemState = EItemState::Unequipped;
					OnItemStateChanged.Broadcast(ItemID, ItemState);
					bUnequipped = true;
			}

			if (ActiveEffect.IsValid())
			{
				if (AbilitySystem->RemoveActiveGameplayEffect(ActiveEffect))
				{
					bUnequipped = true;
				}
			}
		}
	}
	return bUnequipped;
}

ACharacter* AAMItem::GetItemOwner() const
{
	return ItemOwner.Get();
}

void AAMItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsValid(ItemWidget) && ActorToLookAt.IsValid())
	{
		FVector Target = ActorToLookAt.Get()->GetActorLocation();
		FVector Start = this->GetActorLocation();
		FRotator Rotator = FRotationMatrix::MakeFromX(Target - Start).Rotator();
		ItemWidget->SetWorldRotation(FRotator(0, Rotator.Yaw, 0));
	}
}

void AAMItem::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentHit.AddUniqueDynamic(this, &AAMItem::OnItemHit);
	ItemWidget = Cast<UWidgetComponent>(this->GetComponentByClass(UWidgetComponent::StaticClass()));
	if (GetWorld() != nullptr)
	{
		ActorToLookAt = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void AAMItem::OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AAntiMatterTestCharacter* Character = Cast<AAntiMatterTestCharacter>(OtherActor))
	{
		Character->AddItemToInventory(this);
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
	}
}
