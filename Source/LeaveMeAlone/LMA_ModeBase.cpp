// LeaveMeAlone Game by Netologiya. All rights are reserved


#include "LMA_ModeBase.h"
#include "LMADefaultCharacter.h"
#include "LMAPLayerController.h"

ALMA_ModeBase::ALMA_ModeBase() {
	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();
}
