// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	TriggerCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule Comp"));
	TriggerCapsuleComp->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentScale = StartScale;

	SetActorEnableCollision(false);

	TriggerCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::OnOverlapBegin);
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentScale += GrowthRate * DeltaTime;

	if (CurrentScale >= MaxScale)
	{
		Destroy();
	}

	if (CurrentScale >= StartCollisionScale)
	{
		SetActorEnableCollision(true);
	}

	SetActorScale3D(FVector(CurrentScale));
}

void AExplosion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Explosion Overlap Event"));

	if (Cast<ABaseEnemy>(OtherActor))
	{
		DamageOtherActor(OtherActor);

		FTimerHandle DamageActorTimerHandle;

		FTimerDelegate DamageActorDelegate = FTimerDelegate::CreateUObject(this, &AExplosion::DamageOtherActor, OtherActor);

		GetWorldTimerManager().SetTimer(DamageActorTimerHandle, DamageActorDelegate, DamageRate, true);
	}
}

void AExplosion::DamageOtherActor(AActor* OtherActor)
{
	if (OtherActor->IsActorBeingDestroyed())
	{
		return;
	}

	if (ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(BaseEnemy, Damage, nullptr, this, nullptr);
	}
}

