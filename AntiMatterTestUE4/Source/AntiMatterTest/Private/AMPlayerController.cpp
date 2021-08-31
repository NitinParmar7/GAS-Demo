#include "AMPlayerController.h"




void AAMPlayerController::CreateHUD()
{
	if (HUDWidget == nullptr && HUDClass == nullptr)
	{
		return;
	}

	HUDWidget = CreateWidget<UUserWidget>(this, HUDClass);
	HUDWidget->AddToViewport();
}
