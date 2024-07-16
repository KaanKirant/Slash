// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h" //Needed for Enhanced Input System
#include "EnhancedInputSubsystems.h" //Needed for Enhanced Input System
#include "EnhancedInputComponent.h" //Needed for Enhanced Input System
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}
void ABird::BeginPlay()
{
	Super::BeginPlay();
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController())) //Needed for Enhanced Input System
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) //Needed for Enhanced Input System
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0); //Needed for Enhanced Input System
		}
	}
}
void ABird::MoveForward(float Value) //Needed for Old Input System
{
	if(Controller && (Value != 0.f))
	{
		FVector Forward{ GetActorForwardVector() };
		AddMovementInput(Forward, Value);
	}
}
void ABird::Turn(float Value)
{
	AddControllerYawInput(Value);
}
void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
void ABird::Move(const FInputActionValue& Value) //Needed for Enhanced Input System
{
	const float DirectionValue = Value.Get<float>();
	if (Controller && (DirectionValue != 0.f))
	{
		FVector Forward{ GetActorForwardVector() }; 
		AddMovementInput(Forward, DirectionValue);
	}
}
void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) //Needed for Enhanced Input System
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move); //Needed for Enhanced Input System
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}

	//PlayerInputComponent->BindAxis(FName{ "MoveForward" }, this, &ABird::MoveForward); //Needed for Old Input System
	//PlayerInputComponent->BindAxis(FName{ "Turn" }, this, &ABird::Turn);
	//PlayerInputComponent->BindAxis(FName{ "LookUp" }, this, &ABird::LookUp);
}