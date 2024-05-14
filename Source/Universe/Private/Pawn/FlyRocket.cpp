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


// =========================================================================== デフォルトメソッド群 =========================================================================== //
// ========================================================================================================================================================================== //

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
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlyRocket::OnBeginOverlap);

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
	DebugRightArrow->SetArrowLength(HorizontalMoveOffset);
	DebugRightArrow->SetHiddenInGame(false);

	DebugLeftArrow = CreateDefaultSubobject<UArrowComponent>("Arrow Left");
	DebugLeftArrow->SetupAttachment(RootComponent);
	DebugLeftArrow->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	DebugLeftArrow->SetArrowLength(HorizontalMoveOffset);
	DebugLeftArrow->SetHiddenInGame(false);

	PrimaryActorTick.bCanEverTick = true;

}

void AFlyRocket::BeginPlay()
{
	Super::BeginPlay();
}

void AFlyRocket::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &AFlyRocket::NIMoveLeft);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &AFlyRocket::ComplatedMoveLeft);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AFlyRocket::NIMoveRight);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AFlyRocket::ComplatedMoveRight);

		EnhancedInputComponent->BindAction(GearChangeAction, ETriggerEvent::Triggered, this, &AFlyRocket::NIGearChange);
	}

	// MappingContext setting
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FlyRocketMappingContext, 0);
		}
	}
}

void AFlyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving)
	{
		MoveForward(DeltaTime);
		MoveHorizontal(DeltaTime);
	}
	
}


// ============================================================================== 公開メソッド群 ============================================================================= //
// ========================================================================================================================================================================== //


// ============================================================================ Forwardメソッド群 ============================================================================ //
// ========================================================================================================================================================================== //

void AFlyRocket::SetIsMoving(bool IsMoving)
{
	this->bIsMoving = IsMoving;
}

void AFlyRocket::MoveForward(const float DeltaTime)
{
	// 速度に影響する全ての割合値を算出
	float CurrentSpeed = ((ForwardSpeed + CurrentForwardAcceleration) * DeltaTime) * GetAffectForwardSpeedRate();

	// 現在の位置からZ方向に進行
	FVector AddLocation = FVector(0.0f, 0.0f, CurrentSpeed);
	
	AddActorWorldOffset(AddLocation);
}

void AFlyRocket::ForwardAccelerate()
{
	CurrentForwardAcceleration += ForwardAccelerationAmount;
}

void AFlyRocket::ResetForwardAccelerate()
{
	CurrentForwardAcceleration = 0.0f;
}

float AFlyRocket::GetAffectForwardSpeedRate()
{
	return 1.0f;
}


// =========================================================================== Horizontalメソッド群 =========================================================================== //
// ========================================================================================================================================================================== //

void AFlyRocket::MoveHorizontal(const float DeltaTime)
{
	// TODO スタン中は横移動しない。

	float AddY = (RightRate - LeftRate) * HorizontalSpeed * DeltaTime;
	FVector AddLocation = FVector(0.0f, AddY, 0.0f);
	float MovedY = BodyRoot->GetRelativeLocation().Y + AddLocation.Y;

	UpdateHorizontalRate(DeltaTime);

	const float LEFT = -1.0f;
	const float RIGHT = 1.0f;

	// 移動可能範囲内ならば
	if (LEFT * HorizontalMoveOffset < MovedY && MovedY < RIGHT * HorizontalMoveOffset)
	{
		BodyRoot->AddLocalOffset(AddLocation);
	}
}

void AFlyRocket::MoveLeft(const float Value)
{
	LeftRate = FMath::Clamp(LeftRate + Value * GetWorld()->DeltaTimeSeconds, UnderRate, DefaultRate);
}

void AFlyRocket::MoveRight(const float Value)
{
	RightRate = FMath::Clamp(RightRate + Value * GetWorld()->DeltaTimeSeconds, UnderRate, DefaultRate);
}

void AFlyRocket::UpdateHorizontalRate(float DeltaTime)
{
	LeftRate = FMath::Clamp(LeftRate - HorizontalDecelerationRate * GetWorld()->DeltaTimeSeconds, UnderRate, DefaultRate);
	RightRate = FMath::Clamp(RightRate - HorizontalDecelerationRate * GetWorld()->DeltaTimeSeconds, UnderRate, DefaultRate);
}

void AFlyRocket::ResetHorizontalRate()
{
	LeftRate = UnderRate;
	RightRate = UnderRate;
}

void AFlyRocket::NIMoveLeft(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	if (!bIsRightMoving)
	{
		bIsLeftMoving = true;
		MoveLeft(InputValue);
	}
}

void AFlyRocket::NIMoveRight(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	if (!bIsLeftMoving)
	{
		bIsRightMoving = true;
		MoveRight(InputValue);
	}
}

void AFlyRocket::ComplatedMoveLeft()
{
	bIsLeftMoving = false;
}

void AFlyRocket::ComplatedMoveRight()
{
	bIsRightMoving = false;
}

void AFlyRocket::NIGearChange(const FInputActionValue& Value)
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

void AFlyRocket::Stun()
{
	bIsStun = true;

	GetWorld()->GetTimerManager().ClearTimer(StunTimer);

	GetWorld()->GetTimerManager().SetTimer
	(
		StunTimer,
		[&]()
		{
			bIsStun = false;
		},
		StunDuration,
		false
	);
}

void AFlyRocket::VaryingAccelerations(const float DeltaTime)
{
	VaryingGearAcceleration(DeltaTime);

	VaryingStunSpeedRate(DeltaTime);
}

void AFlyRocket::VaryingGearAcceleration(const float DeltaTime)
{
	float TargetRate = 0.0f;
	
	// 目標のレート値を決める
	switch (CurrentGear)
	{
	case AFlyRocket::LOW:
		TargetRate = GearAccelerationInLow;
		break;
	case AFlyRocket::NORMAL:
		TargetRate = GearAccelerationInNormal;
		break;
	case AFlyRocket::HIGH:
		TargetRate = GearAccelerationInHigh;
		break;
	default:
		TargetRate = 0.0f;
		break;
	}

	// 目標のレート値ならば終了
	if (GearAcceleration == TargetRate)
	{
		return;
	}

	// 目標のレート値になるようデルタタイムを加減算
	const float VaryingAcceleration = UKismetMathLibrary::SafeDivide(DeltaTime, GearAccelerationVaryingtime);
	if (GearAcceleration < TargetRate)
	{
		GearAcceleration = FMath::Min(GearAcceleration + VaryingAcceleration, TargetRate);
	}
	else
	{
		GearAcceleration = FMath::Max(GearAcceleration - VaryingAcceleration, TargetRate);
	}
}

void AFlyRocket::VaryingStunSpeedRate(const float DeltaTime)
{
	if (!bIsStun && StunSpeedRate < DefaultRate)
	{
		StunSpeedRate = FMath::Min(StunSpeedRate + UKismetMathLibrary::SafeDivide(DeltaTime, StunRecoveryDuration), DefaultRate);
	}
	else if (bIsStun && StunSpeedRate > UnderRate)
	{
		StunSpeedRate = FMath::Max(StunSpeedRate - UKismetMathLibrary::SafeDivide(DeltaTime, StunDuration), UnderRate);
	}
}

void AFlyRocket::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsStun)
	{
		Stun();
	}
}



