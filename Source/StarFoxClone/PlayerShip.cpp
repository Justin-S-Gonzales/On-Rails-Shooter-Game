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

// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Comp"));
	RootComponent = CapsuleComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	Mesh->SetupAttachment(CapsuleComp);

	LeftProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(
		TEXT("Left Projectile Spawn Point"));
	LeftProjectileSpawnPoint->SetupAttachment(Mesh);

	RightProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(
		TEXT("Right Projectile Spawn Point"));
	RightProjectileSpawnPoint->SetupAttachment(Mesh);

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
	StartDirection = GetActorForwardVector();

	// Set camera rotation to start direction=
	Camera->SetActorRotation(FRotator(CameraAngle, StartDirection.Rotation().Yaw, StartDirection.Rotation().Roll));

	CameraStartRotation = Camera->GetActorRotation();
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

	// Move based on input
	
	if (bIsMoving)
	{
		VelocityVector += InputMovementAcceleration * MoveDirection;
		if (VelocityVector.Length() > InputMovementMaxSpeed)
		{
			VelocityVector = VelocityVector.GetSafeNormal() * InputMovementMaxSpeed;
		}

		// Camera->SetActorRotation(Mesh->GetRelativeRotation() * CameraRotationScale);
		Camera->SetActorRotation(FMath::RInterpTo(Camera->GetActorRotation(), Mesh->GetRelativeRotation() * CameraRotationScale, UGameplayStatics::GetWorldDeltaSeconds(this), CameraInterpSpeed));

	}
	if (!bIsMoving)
	{
		// Input movement deceleration
		// CurrentInputMovementVelocity = FMath::Max(CurrentInputMovementVelocity - InputMovementStopSpeed, 0.f);
		if (VelocityVector.Length() > 0)
		{
			VelocityVector *= (VelocityVector.Length() - InputMovementDeceleration) / VelocityVector.Length();
		}

		if (FVector::DotProduct(VelocityVector, MoveDirection) < 0)
		{
			VelocityVector = FVector(0.f);
			MoveDirection = FVector(0.f);
		}

		// Rotate camera to start
		Camera->SetActorRotation(FMath::RInterpTo(Camera->GetActorRotation(), CameraStartRotation, UGameplayStatics::GetWorldDeltaSeconds(this), CameraInterpSpeed));
	}

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
		FVector ForwardLocation = StartLocation + FVector::DotProduct(GetActorLocation() - StartLocation, StartDirection) * StartDirection;
		// Actor upward location
		FVector UpwardLocation = GetActorUpVector() * MaxVerticalDistanceFromStart * Sign;
		// Actor rightward location
		FVector RightwardLocation = FVector::DotProduct(GetActorRightVector(), GetActorLocation() - StartLocation) * GetActorRightVector();

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
		FVector ForwardLocation = StartLocation + FVector::DotProduct(GetActorLocation() - StartLocation, StartDirection) * StartDirection;
		// Actor upward location
		FVector UpwardLocation = FVector::DotProduct(GetActorUpVector(), GetActorLocation() - StartLocation) * GetActorUpVector();
		// Actor righward location
		FVector RightwardLocation = GetActorRightVector() * MaxHorizontalDistanceFromStart * Sign;

		SetActorLocation(ForwardLocation + UpwardLocation + RightwardLocation);
	}

	// Set mesh rotation
	FRotator NewMeshRotation = FRotator(0.f, 0.f, 0.f);
	NewMeshRotation.Yaw = (FVector::DotProduct(VelocityVector, GetActorRightVector()) / InputMovementMaxSpeed) * MaxMovementRotationAngle;
	NewMeshRotation.Roll = (FVector::DotProduct(VelocityVector, GetActorRightVector()) / InputMovementMaxSpeed) * MaxMovementRotationAngle * MeshRollMovementScale;
	NewMeshRotation.Pitch = (FVector::DotProduct(VelocityVector, GetActorUpVector()) / InputMovementMaxSpeed) * MaxMovementRotationAngle;
	Mesh->SetRelativeRotation(NewMeshRotation);

	// Set camera position
	// Move camera forward to where player ship is
	FVector NewCameraLocation = StartLocation + (FVector::DotProduct(StartDirection, GetActorLocation())) * StartDirection;
	// Move camera backward distance of camera distance
	NewCameraLocation -= GetActorForwardVector() * CameraDistance;

	// We can use the dot product to determine the distance the ship has traveled along the right and up vectors from the start both positively and negatively

	// Move camera right or left
	NewCameraLocation += GetActorRightVector() * FVector::DotProduct((GetActorLocation() - StartLocation), GetActorRightVector()) * CameraMovementScale;
	// Move camera up or down
	NewCameraLocation += GetActorUpVector() * FVector::DotProduct((GetActorLocation() - StartLocation), GetActorUpVector()) * CameraMovementScale + GetActorUpVector() * CameraHeight;

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
	// UE_LOG(LogTemp, Warning, TEXT("Moved"));

	if (!GetController())
	{
		UE_LOG(LogTemp, Warning, TEXT("No controller"));
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		return;
	}

	const FVector2D MoveValue = Value.Get<FVector2D>();

	MoveDirection = FVector(0.f);

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

	// Acceleration
	MoveDirection = MoveDirection.GetSafeNormal();

	bIsMoving = true;
}

void APlayerShip::StoppedMovement(const FInputActionValue& Value)
{
	bIsMoving = false;
}

void APlayerShip::Fire(const FInputActionValue& Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("Fired"));
}

