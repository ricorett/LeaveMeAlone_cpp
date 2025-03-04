// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULMAWeaponComponent::Fire()
{
	if (Weapon && !AnimReloading)
	{
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::TickComponent(
	float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		
		/*Weapon->OnNeedReload.AddDynamic(this, &ULMAWeaponComponent::OnNeedReloadHandler);*/

		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
		}
	}
}

//void ULMAWeaponComponent::OnNeedReloadHandler()
//{
//	Reload();
//}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && Weapon && Weapon->CanReload();
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}



void ULMAWeaponComponent::Reload()
{
	if (!CanReload())
		return;
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}

//void ULMAWeaponComponent::StartFire()
//{
//	if (!Weapon || AnimReloading)
//		return;
//
//	if (!bIsFiring)
//	{
//		bIsFiring = true;
//		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ULMAWeaponComponent::FireTick, FireRate, true);
//	}
//}
//
//void ULMAWeaponComponent::StopFire()
//{
//	if (bIsFiring)
//	{
//		bIsFiring = false;
//		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
//	}
//}
//
//void ULMAWeaponComponent::FireTick()
//{
//	UE_LOG(LogTemp, Warning, TEXT("FireTick"));
//	if (Weapon && Weapon->CanFire())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Firing!"));
//		Weapon->Fire();
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Can't fire!"));
//		StopFire();
//	}
//}

