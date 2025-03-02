// LeaveMeAlone Game by Netologiya. All rights are reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LMAAnimInstance.generated.h"

UCLASS()
class LEAVEMEALONE_API ULMAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float StaminaPercent = 1.0f;
};