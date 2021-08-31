
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "AMPlayerController.generated.h"


/**
 * Player Controller to spawn HUD
 */
UCLASS(BlueprintType, Blueprintable)
class AAMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	UUserWidget* HUDWidget;

	void CreateHUD();

protected:

private:

};


