// LeaveMeAlone Game by Netologiya. All rights are reserved


#include "LevelActors/Damage/LMADamageActor.h"

// Sets default values
ALMADamageActor::ALMADamageActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(SphereRadius);
	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
}
void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, false);

}

