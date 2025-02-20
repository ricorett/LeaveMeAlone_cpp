// LeaveMeAlone Game by Netologiya. All rights are reserved


#include "LMAPlayerController.h"

void ALMAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}