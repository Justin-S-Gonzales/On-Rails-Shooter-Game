// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
#include "Engine/DamageEvents.h"
#include "HitFlash.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystem.h"
#include "Projectile.h"

// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule Comp"));
	TriggerCapsuleComp->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerCapsuleComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	Mesh->SetupAttachment(TriggerCapsuleComp);

	LeftProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(
		TEXT("Left Projectile Spawn Point"));
	LeftProjectileSpawnPoint->SetupAttachment(Mesh);

	RightProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(
		TEXT("Right Projectile Spawn Point"));
	RightProjectileSpawnPoint->SetupAttachment(Mesh);

	HitFlashComp = CreateDefaultSubobject<UHitFlash>(TEXT("Hit Flash"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	if (Camera == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Camera!"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}

	StartLocation = GetActorLocation();

	// Set camera rotation to start direction
	Camera->SetActorRotation(GetActorRotation());

	CameraStartRotation = Camera->GetActorRotation();

	TriggerCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::OnOverlapBegin);
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Mesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Mesh!"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}
	
	if (Camera == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Camera!"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}

	// Move player ship
	FVector DeltaVector = GetActorForwardVector() * ForwardMovementSpeed * DeltaTime;
	SetActorLocation(GetActorLocation() + DeltaVector, true);

	if (bIsMoving && !bFreezeMovement)
	{
		
		VelocityVector = FMath::VInterpTo(VelocityVector, MoveDirection * InputMovementMaxSpeed, DeltaTime, InterpSpeed);
		
		Camera->SetActorRotation(FMath::RInterpTo(Camera->GetActorRotation(), CameraStartRotation + Mesh->GetRelativeRotation() * CameraRotationScale, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed));
	}
	if (!bIsMoving)
	{
		// Deceleration
		float Deceleration = InputMovementDeceleration;;
		if (bFreezeMovement)
		{
			Deceleration = KnockbackDeceleration;
		}

		if (VelocityVector.Length() - Deceleration > 0)
		{

			VelocityVector *= (VelocityVector.Length() - Deceleration) / VelocityVector.Length();
		}
		else
		{
			VelocityVector = FVector::ZeroVector;
		}

		// Rotate camera to start rotation
		if (Mesh->GetRelativeRotation() == FRotator(0.f))
		{
			Camera->SetActorRotation(FMath::RInterpTo(Camera->GetActorRotation(), CameraStartRotation, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed));
		}
		else
		{
			Camera->SetActorRotation(FMath::RInterpTo(Camera->GetActorRotation(), CameraStartRotation + Mesh->GetRelativeRotation() * CameraRotationScale, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed));
		}
	}
	
	// Set location to velocity
	SetActorLocation(GetActorLocation() + VelocityVector * DeltaTime, true);

	// If player ship is past max distances, we set it's position to those distances
	// Vertical max
	if (UKismetMathLibrary::Abs(FVector::DotProduct(GetActorLocation() - StartLocation, GetActorUpVector())) > MaxVerticalDistanceFromStart)
	{
		// If ship is downward we need to set it's distance to negative max vertical distance
		float Sign = 1.0f;

		if (FVector::DotProduct(GetActorLocation() - StartLocation, GetActorUpVector()) < 0.f)
		{
			Sign = -1.0f;
		}

		// Set actor forward, upward, and rightward location (is rightward even a word?)

		// Actor forward location
		FVector ForwardLocation = StartLocation + (GetActorLocation() - StartLocation).ProjectOnTo(GetActorForwardVector());
		// Actor upward location
		FVector UpwardLocation = GetActorUpVector() * MaxVerticalDistanceFromStart * Sign;
		// Actor rightward location
		FVector RightwardLocation = (GetActorLocation() - StartLocation).ProjectOnTo(GetActorRightVector());

		SetActorLocation(ForwardLocation + UpwardLocation + RightwardLocation);
	}

	// Horizontal max
	if (UKismetMathLibrary::Abs(FVector::DotProduct(GetActorLocation() - StartLocation, GetActorRightVector())) > MaxHorizontalDistanceFromStart)
	{
		// If ship is to left we need to set it's distance to negative max horizontal distance
		float Sign = 1.0f;

		if (FVector::DotProduct(GetActorLocation() - StartLocation, GetActorRightVector()) < 0.f)
		{
			Sign = -1.0f;
		}

		// Set actor forward, upward, and rightward location (is rightward even a word?)

		// Actor forward location
		FVector ForwardLocation = StartLocation + (GetActorLocation() - StartLocation).ProjectOnTo(GetActorForwardVector());
		// Actor upward location
		FVector UpwardLocation = (GetActorLocation() - StartLocation).ProjectOnTo(GetActorUpVector());
		// Actor righward location
		FVector RightwardLocation = GetActorRightVector() * MaxHorizontalDistanceFromStart * Sign;

		SetActorLocation(ForwardLocation + UpwardLocation + RightwardLocation);
	}

	if (!bFreezeMovement)
	{
		// Set mesh rotation
		FRotator NewMeshRotation = FRotator(0.f, 0.f, 0.f);
		NewMeshRotation.Yaw = (FVector::DotProduct(VelocityVector, GetActorRightVector()) / InputMovementMaxSpeed) * MaxMovementRotationAngle;
		NewMeshRotation.Roll = (FVector::DotProduct(VelocityVector, GetActorRightVector()) / InputMovementMaxSpeed) * MaxMovementRotationAngle * MeshRollMovementScale;
		NewMeshRotation.Pitch = (FVector::DotProduct(VelocityVector, GetActorUpVector()) / InputMovementMaxSpeed) * MaxMovementRotationAngle;
		Mesh->SetRelativeRotation(NewMeshRotation);
	}
	else
	{
		Mesh->SetRelativeRotation(FMath::RInterpTo(Mesh->GetRelativeRotation(), FRotator(0.f, 0.f, 0.f), DeltaTime, InterpSpeed));
	}
	
	// Set camera position
	// Move camera forward to where player ship is
	FVector NewCameraLocation = StartLocation.ProjectOnTo(GetActorRightVector()) + (GetActorLocation()).ProjectOnTo(GetActorForwardVector());
	// Move camera backward distance of camera distance
	NewCameraLocation -= GetActorForwardVector() * CameraDistance;

	// We can use the dot product to determine the distance the ship has traveled along the right and up vectors from the start both positively and negatively

	// Move camera right or left
	NewCameraLocation += (GetActorLocation() - StartLocation).ProjectOnTo(GetActorRightVector()) * CameraMovementScale;
	// Move camera up or down
	NewCameraLocation += StartLocation.ProjectOnTo(GetActorUpVector()) + (GetActorLocation() - StartLocation).ProjectOnTo(GetActorUpVector()) * CameraMovementScale;

	Camera->SetActorLocation(NewCameraLocation);
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Player Controller!"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()
		);

	if (Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No subsystem"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}

	if (InputMove == nullptr || InputFire == nullptr || PlayerInputMapping == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No input actions or mapping context!"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}


	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PlayerInputMapping, 0);

	// Bind input actions
	UEnhancedInputComponent* PlayerEnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PlayerEnhancedInput->BindAction(
		InputMove,
		ETriggerEvent::Triggered,
		this,
		&APlayerShip::Move
	);

	PlayerEnhancedInput->BindAction(
		InputMove,
		ETriggerEvent::Completed,
		this,
		&APlayerShip::StoppedMovement
	);

	PlayerEnhancedInput->BindAction(
		InputFire,
		ETriggerEvent::Triggered,
		this,
		&APlayerShip::Fire
	);
}

void APlayerShip::Move(const FInputActionValue& Value)
{
	if (!GetController())
	{
		UE_LOG(LogTemp, Warning, TEXT("No controller"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}

	MoveDirection = FVector(0.f);

	if (bFreezeMovement)
	{
		return;
	}

	const FVector2D MoveValue = Value.Get<FVector2D>();

	// Right/Left direction
	if (UKismetMathLibrary::Abs(MoveValue.X) > 0)
	{
		MoveDirection += GetActorRightVector() * MoveValue.X;
	}
	// Up/Down direction
	if (UKismetMathLibrary::Abs(MoveValue.Y) > 0)
	{
		// Get Up vector
		MoveDirection += GetActorUpVector() * MoveValue.Y;
	}

	// MoveDirection = MoveValue.X + MoveValue.Y / 

	bIsMoving = true;
}

void APlayerShip::StoppedMovement(const FInputActionValue& Value)
{
	bIsMoving = false;
}

void APlayerShip::UnFreezeMovement()
{
	bFreezeMovement = false;
	UE_LOG(LogTemp, Warning, TEXT("Called UnFreezeMovement"));
}

void APlayerShip::SetInvincibilityToFalse()
{
	bIsInvincible = false;
}

void APlayerShip::ResetFireability()
{
	bCanFire = true;
}

void APlayerShip::Fire(const FInputActionValue& Value)
{
	if (!bCanFire)
	{
		return;
	}

	// Spawn lasers
	AProjectile* RightProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, RightProjectileSpawnPoint->GetComponentLocation(), RightProjectileSpawnPoint->GetComponentRotation());

	AProjectile* LeftProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, LeftProjectileSpawnPoint->GetComponentLocation(), LeftProjectileSpawnPoint->GetComponentRotation());

	// To start lasers in sync, we start a timer for next tick
	FTimerDelegate LeftLaserStartMovementTimerDelegate = FTimerDelegate::CreateUObject(
		LeftProjectile,
		&AProjectile::SetMove,
		true
	);

	GetWorldTimerManager().SetTimerForNextTick(LeftLaserStartMovementTimerDelegate);

	FTimerDelegate RightLaserStartMovementTimerDelegate = FTimerDelegate::CreateUObject(
		RightProjectile,
		&AProjectile::SetMove,
		true
	);

	GetWorldTimerManager().SetTimerForNextTick(RightLaserStartMovementTimerDelegate);

	bCanFire = false;
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &APlayerShip::ResetFireability, FireRate / 2.0f, false, FireRate / 2.0f);
}

void APlayerShip::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("died"));

	if (DeathParticles == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No death particles!"));
		return;
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(
		this,
		DeathParticles,
		GetActorLocation(),
		GetActorRotation(),
		DeathParticlesScale
	);

	Destroy();
}

void APlayerShip::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping!"));
	// Send player in opposite direction at knockback speed
	FVector MiddleOfScreen = StartLocation.ProjectOnTo(GetActorRightVector()) + StartLocation.ProjectOnTo(GetActorUpVector()) + GetActorLocation().ProjectOnTo(GetActorForwardVector());

	// Knockback vector to middle of screen
	FVector KnockbackVector = MiddleOfScreen - GetActorLocation();
	VelocityVector = KnockbackVector.GetSafeNormal() * CollisionOverlapKnockbackSpeed;

	bFreezeMovement = true;
	bIsMoving = false;

	// Set freeze timer
	GetWorldTimerManager().SetTimer(FreezeTimerHandle, this, &APlayerShip::UnFreezeMovement, FreezeTime / 2.0f, false, FreezeTime / 2.0f);

	// Shake camera
	if (CollisionCameraShakeClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No camera shake class!"));
		return;
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(CollisionCameraShakeClass);

	// If invincible, we stop here
	if (bIsInvincible)
	{
		return;
	}

	FDamageEvent damageEvent;
	TakeDamage(CollisionOverlapDamage, damageEvent, nullptr, OtherActor);

	// Set invincibility to true
	bIsInvincible = true;

	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &APlayerShip::SetInvincibilityToFalse, CollisionInvincibilityTime / 2.0f, false, CollisionInvincibilityTime / 2.0f);
}

float APlayerShip::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Took Damage"));

	return 0.0f;
}

