// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Fire()
{
	Shoot();
}

void ALMABaseWeapon::Shoot()
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}

	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets()
{
	
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %d"), CurrentAmmoWeapon.Bullets);

	if (IsCurrentClipEmpty())
	{
		/*OnNeedReload.Broadcast();*/
		ChangeClip();
	}
	CurrentAmmoWeapon.Bullets--;
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}


void ALMABaseWeapon::ChangeClip()
{
	if (!AmmoWeapon.Infinite)
	{
		AmmoWeapon.Clips--;
	}
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
	
}

void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALMABaseWeapon::CanReload() const
{
	return AmmoWeapon.Clips > 0 || AmmoWeapon.Infinite;
}

bool ALMABaseWeapon::CanFire() const
{
	return CurrentAmmoWeapon.Bullets > 0 || AmmoWeapon.Infinite;
}

//void ALMABaseWeapon::Fire()
//{
//	if (!CanFire())
//	{
//
//		
//		return;
//	}
//	Shoot();
//
//	if (!AmmoWeapon.Infinite)
//	{
//		DecrementBullets();
//	}
//}