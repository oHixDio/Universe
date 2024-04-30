// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/FlyRocket.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

/*
* あくまでデフォルトの設定
* この設定をデフォルトとし、エディタで変更が行われる。
*/
AFlyRocket::AFlyRocket()
{
	// RootComponentを始めに生成する。
	if (RootComponent == nullptr) 
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	}

	// BodyRoot 初期値設定
	BodyRoot = CreateDefaultSubobject<USceneComponent>("Body Root");
	BodyRoot->SetupAttachment(RootComponent);

	// BodyFirst 初期設定
	BodyFirst = CreateDefaultSubobject<UStaticMeshComponent>("Body First");
	BodyFirst->SetupAttachment(BodyRoot);

	// BodySecond 初期設定
	BodySecond = CreateDefaultSubobject<UStaticMeshComponent>("Body Second");
	BodySecond->SetupAttachment(BodyRoot);

	// BodyThird 初期設定
	BodyThird = CreateDefaultSubobject<UStaticMeshComponent>("Body Third");
	BodyThird->SetupAttachment(BodyRoot);

	// CollisionComponent  初期設定
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("Capcule Collision");
	CollisionComponent->SetupAttachment(BodyRoot);

	// SpringArm 初期設定
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Camera　初期設定
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// DEBUG Arrow 初期設定
	DebugRightArrow = CreateDefaultSubobject<UArrowComponent>("Arrow Right");
	DebugRightArrow->SetupAttachment(RootComponent);
	DebugRightArrow->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	DebugRightArrow->SetArrowLength(HorizontalOffsetMaxLimit);
	DebugRightArrow->SetHiddenInGame(false);

	DebugLeftArrow = CreateDefaultSubobject<UArrowComponent>("Arrow Left");
	DebugLeftArrow->SetupAttachment(RootComponent);
	DebugLeftArrow->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	DebugLeftArrow->SetArrowLength(HorizontalOffsetMaxLimit);
	DebugLeftArrow->SetHiddenInGame(false);

	PrimaryActorTick.bCanEverTick = true;

}

void AFlyRocket::BeginPlay()
{
	Super::BeginPlay();
	
	// MappingContext setting
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FlyRocketMappingContext, 0);
		}
	}
}

void AFlyRocket::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(HorizontalAction, ETriggerEvent::Triggered, this, &AFlyRocket::MoveHorizontal);
		EnhancedInputComponent->BindAction(HorizontalAction, ETriggerEvent::Completed, this, &AFlyRocket::ComplatedHorizontal);
	}
}

void AFlyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAdvance(DeltaTime);

	VaryingRates(DeltaTime);
}

void AFlyRocket::MoveAdvance(const float DeltaTime)
{
	float CurrentSpeed = AdvanceSpeed * GearSpeedRate * HorizontalSpeedRate * StunSpeedRate * DeltaTime;
	// 現在の位置からZ方向に進行
	FVector MovedLocation = FVector(0.0f, 0.0f, CurrentSpeed) + GetActorLocation();
	
	SetActorLocation(MovedLocation);
}

void AFlyRocket::MoveHorizontal(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	// 継続入力で水平移動速度上昇
	float AddY = MovementVector.Y * HorizontalSpeed * UKismetMathLibrary::SafeDivide(HorizSpeedRateMax, HorizontalSpeedRate) * DeltaTime;
	FVector AddLocation = FVector(0.0f, AddY, 0.0f);
	FVector MovedLocation = BodyRoot->GetRelativeLocation() + AddLocation;

	// MoveLimit範囲内ならば動く
	if (MovedLocation.Y < HorizontalOffsetMaxLimit && MovedLocation.Y > HorizontalOffsetMinLimit)
	{
		BodyRoot->AddLocalOffset(AddLocation);
		bIsMovingHorizontal = true;
	}

	// UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat((HorizSpeedRateMax / HorizontalSpeedRate)), true, true, FColor::Red,2.0f,FName("Speed"));
}

void AFlyRocket::ComplatedHorizontal()
{
	bIsMovingHorizontal = false;
}

void AFlyRocket::VaryingRates(const float DeltaTime)
{
	VaryingHorizontalSpeedRate(DeltaTime);
}

void AFlyRocket::VaryingHorizontalSpeedRate(const float DeltaTime)
{
	if (bIsMovingHorizontal)
	{
		// Min値より低くならない。
		HorizontalSpeedRate = FMath::Max(HorizontalSpeedRate - DeltaTime, HorizSpeedRateMin);
	}
	else
	{
		// Max値より高くならない
		HorizontalSpeedRate = FMath::Min(HorizontalSpeedRate + DeltaTime, HorizSpeedRateMax);
	}

	// UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(HorizontalSpeedRate), true, true, FColor::Black, 2.0f, FName("Rate"));
}



