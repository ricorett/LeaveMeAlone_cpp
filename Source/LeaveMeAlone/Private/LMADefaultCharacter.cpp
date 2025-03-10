#include "LMADefaultCharacter.h"

ALMADefaultCharacter::ALMADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("WeaponComponent");
	
}

void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
	

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	AnimInstance = Cast<ULMAAnimInstance>(GetMesh()->GetAnimInstance());
	CurrentStamina = MaxStamina;
}

void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArmComponent->TargetArmLength =
		FMath::FInterpTo(SpringArmComponent->TargetArmLength, ArmLength, DeltaTime, ZoomSpeed);

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	if (bWantsToSprint && GetVelocity().Size() > 0)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - StaminaConsumption * DeltaTime, 0.0f, MaxStamina);
		if (CurrentStamina <= 0)
			StopSprint();
	}
	else
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRecovery * DeltaTime, 0.0f, MaxStamina);
	}

	if (AnimInstance)
	{
		ULMAAnimInstance* LMAnimInstance = Cast<ULMAAnimInstance>(AnimInstance);
		if (LMAnimInstance)
		{
			LMAnimInstance->StaminaPercent = CurrentStamina / MaxStamina;
			LMAnimInstance->IsSprinting = bWantsToSprint && CurrentStamina > 0.0f;
		}
	}

	 GEngine->AddOnScreenDebugMessage(
		-1, 0.1f, FColor::Emerald, FString::Printf(TEXT("STAMINA: %.0f / %.0f"), CurrentStamina, MaxStamina));
}

void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location);
		float YawRotation = LookAtRotation.Yaw;

		SetActorRotation(FQuat(FRotator(0.0f, YawRotation, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}

}


void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ALMADefaultCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ALMADefaultCharacter::ZoomOut);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALMADefaultCharacter::Fire);

 /*   PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALMADefaultCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ALMADefaultCharacter::OnStopFire);*/

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ALMADefaultCharacter::Reload);
	
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::ZoomIn()
{
	ArmLength = FMath::Clamp(ArmLength - ZoomSpeed, MinArmLength, MaxArmLength);
}

void ALMADefaultCharacter::ZoomOut()
{
	ArmLength = FMath::Clamp(ArmLength + ZoomSpeed, MinArmLength, MaxArmLength);
}



void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();

	PlayAnimMontage(DeathMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

//void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
//}

void ALMADefaultCharacter::StartSprint()
{
	if (CanSprint())
	{
		bWantsToSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ALMADefaultCharacter::StopSprint()
{
	bWantsToSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

bool ALMADefaultCharacter::CanSprint() const
{
	return CurrentStamina > 10.0f && GetCharacterMovement()->IsMovingOnGround() && !GetCharacterMovement()->IsFalling();
}

void ALMADefaultCharacter::Fire() {
	if (WeaponComponent)
		WeaponComponent->Fire();
}

//void ALMADefaultCharacter::OnStartFire()
//{
//	if (WeaponComponent)
//	{
//		WeaponComponent->StartFire();
//	}
//}
//
//void ALMADefaultCharacter::OnStopFire()
//{
//	if (WeaponComponent)
//	{
//		WeaponComponent->StopFire();
//	}
//}

void ALMADefaultCharacter::Reload(){
	if (WeaponComponent){
		WeaponComponent->Reload();
	}
}