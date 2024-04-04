// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Comp"));
	RootComponent = TriggerCapsuleComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TriggerCapsuleComp);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();

	TriggerCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetActorLocation() - StartLocation).Length() > MaxDistance)
	{
		Destroy();
	}

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime, true);
	
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If other actor is also a projectile, we don't do anything
	if (OtherActor->GetClass() == this->GetClass())
	{
		return;
	}

	if (!ImpactParticles)
	{
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		UE_LOG(LogTemp, Warning, TEXT("No impact particles!"))
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ImpactParticles,
		GetActorLocation(),
		(-GetActorRotation().Vector()).Rotation(),
		ImpactParticlesScale
	);

	Destroy();
}

