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

		EnhancedInputComponent->BindAction(GearChangeAction, ETriggerEvent::Triggered, this, &AFlyRocket::GearChange);
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
	FVector AddLocation = FVector(0.0f, 0.0f, CurrentSpeed);
	
	AddActorWorldOffset(AddLocation);

	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(CurrentSpeed), true, true, FColor::Red,2.0f,FName("Speed"));
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
}

void AFlyRocket::GearChange(const FInputActionValue& Value)
{
	// ユーザーから正か負の入力を受け取る
	float PositiveOrNegative = Value.Get<float>();

	if (PositiveOrNegative > 0)
	{
		GearUp();
	}
	else
	{
		GearDown();
	}

	UKismetSystemLibrary::PrintString(this, PositiveOrNegative > 0 ? TEXT("Up") : TEXT("Down"), true, true, FColor::Black, 2.0f, FName("Action"));

	switch (CurrentGear)
	{
	case AFlyRocket::LOW:
		UKismetSystemLibrary::PrintString(this, TEXT("LOW"), true, true, FColor::Blue, 2.0f, FName("Gear"));
		break;
	case AFlyRocket::NORMAL:
		UKismetSystemLibrary::PrintString(this, TEXT("NORMAL"), true, true, FColor::Blue, 2.0f, FName("Gear"));
		break;
	case AFlyRocket::HIGH:
		UKismetSystemLibrary::PrintString(this, TEXT("HIGH"), true, true, FColor::Blue, 2.0f, FName("Gear"));
		break;
	default:
		/* 何も行わない */
		break;
	}
}

void AFlyRocket::GearUp()
{
	switch (CurrentGear)
	{
	case AFlyRocket::LOW:
		CurrentGear = NORMAL;
		break;
	case AFlyRocket::NORMAL:
		CurrentGear = HIGH;
		break;
	case AFlyRocket::HIGH:
		/* 何も行わない */
		break;
	default:
		/* 何も行わない */
		break;
	}
}

void AFlyRocket::GearDown()
{
	switch (CurrentGear)
	{
	case AFlyRocket::LOW:
		/* 何も行わない */
		break;
	case AFlyRocket::NORMAL:
		CurrentGear = LOW;
		break;
	case AFlyRocket::HIGH:
		CurrentGear = NORMAL;
		break;
	default:
		/* 何も行わない */
		break;
	}
}

void AFlyRocket::ComplatedHorizontal()
{
	bIsMovingHorizontal = false;
}

void AFlyRocket::VaryingRates(const float DeltaTime)
{
	VaryingHorizontalSpeedRate(DeltaTime);

	VaryingGearSpeedRate(DeltaTime);
}

void AFlyRocket::VaryingGearSpeedRate(const float DeltaTime)
{
	float TargetRate = 0.0f;
	
	// 目標のレート値を決める
	switch (CurrentGear)
	{
	case AFlyRocket::LOW:
		TargetRate = GearSpeedRateInLow;
		break;
	case AFlyRocket::NORMAL:
		TargetRate = GearSpeedRateInNormal;
		break;
	case AFlyRocket::HIGH:
		TargetRate = GearSpeedRateInHigh;
		break;
	default:
		UKismetSystemLibrary::PrintString(this, TEXT("Warning! The current state of the gear is out of whack. "), true, true, FColor::Red, 2.0f, FName("None"));
		TargetRate = 0.0f;
		break;
	}

	// 目標のレート値ならば終了
	if (GearSpeedRate == TargetRate)
	{
		UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(GearSpeedRate), true, true, FColor::Yellow, 2.0f, FName("GearSpeedRate"));
		return;
	}

	// 目標のレート値になるようデルタタイムを加減算
	if (GearSpeedRate < TargetRate)
	{
		GearSpeedRate = FMath::Min(GearSpeedRate + DeltaTime, TargetRate);
	}
	else
	{
		GearSpeedRate = FMath::Max(GearSpeedRate - DeltaTime, TargetRate);
	}

	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(GearSpeedRate), true, true, FColor::Yellow, 2.0f, FName("GearSpeedRate"));
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



