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


// ================================================================================= Core群 ================================================================================= //
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
		EnhancedInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &AFlyRocket::NIMoveHorizontal);

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

	if (bIsStunned)
	{
		UpdateStunRate(DeltaTime);
	}

	if (LeftRate + RightRate != UnderRate)
	{
		InertiaHorizontalRate(DeltaTime);
	}

	if (CurrentGearSpeed != TargetGearSpeed)
	{
		LerpGearSpeed(DeltaTime);
	}
}

void AFlyRocket::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsStunned && OtherActor->ActorHasTag("Asteroid"))
	{
		Stun();
		OtherActor->Destroy();
	}
}


// ============================================================================= Inputメソッド群 ============================================================================= //
// ========================================================================================================================================================================== //

void AFlyRocket::NIMoveHorizontal(const FInputActionValue& Value)
{
	if (bIsStunned)
	{
		return;
	}

	// ユーザーから正数か負数の入力を受け取る
	const float PositiveOrNegative = Value.Get<float>();

	const float AbsInputValue = FMath::Abs(PositiveOrNegative);

	if (PositiveOrNegative > 0)
	{
		MoveRight(AbsInputValue);
	}
	else
	{
		MoveLeft(AbsInputValue);
	}
}

void AFlyRocket::NIGearChange(const FInputActionValue& Value)
{
	if (bIsStunned)
	{
		return;
	}

	// ユーザーから正数か負数の入力を受け取る
	const float PositiveOrNegative = Value.Get<float>();

	if (PositiveOrNegative > 0)
	{
		GearUp();
	}
	else
	{
		GearDown();
	}
}


// ============================================================================== 公開メソッド群 ============================================================================= //
// ========================================================================================================================================================================== //


// ============================================================================ Forwardメソッド群 ============================================================================ //
// ========================================================================================================================================================================== //

void AFlyRocket::SetIsMoving(bool IsMoving)
{
	bIsMoving = IsMoving;
}

void AFlyRocket::MoveForward(const float DeltaTime)
{
	float CurrentSpeed = (ForwardSpeed + GetAffectForwardSpeed()) * CurrentStunRate * DeltaTime;

	// 現在の位置からZ方向に進行
	FVector AddLocation = FVector(0.0f, 0.0f, CurrentSpeed);
	
	AddActorWorldOffset(AddLocation);
}

void AFlyRocket::ForwardAccelerate()
{
	TotalForwardAdditionalSpeed += ForwardAdditionalSpeedAmount;
}

void AFlyRocket::ResetForwardAccelerate()
{
	TotalForwardAdditionalSpeed = 0.0f;
}

float AFlyRocket::GetAffectForwardSpeed()
{
	float AffectSpeed = CurrentGearSpeed + TotalForwardAdditionalSpeed;
	return AffectSpeed;
}


// ========================================================================== Horizontalメソッド群 =========================================================================== //
// ========================================================================================================================================================================== //

void AFlyRocket::MoveHorizontal(const float DeltaTime)
{
	// TODO スタン中は横移動しない。

	// 左右の割合から、水平速度を算出(-Yが左のため、LeftRateを - の右側に配置)
	float AddY = (RightRate - LeftRate) * HorizontalSpeed * DeltaTime;
	FVector AddLocation = FVector(0.0f, AddY, 0.0f);
	float MovedY = BodyRoot->GetRelativeLocation().Y + AddLocation.Y;

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
	// Clamp  第一引数を、第二引数から第三引数の間で調整する
	LeftRate = FMath::Clamp(LeftRate + Value * HorizontalAccelelerationRate * GetWorld()->DeltaTimeSeconds, UnderRate, TopRate);
}

void AFlyRocket::MoveRight(const float Value)
{
	RightRate = FMath::Clamp(RightRate + Value * HorizontalAccelelerationRate * GetWorld()->DeltaTimeSeconds, UnderRate, TopRate);
}

void AFlyRocket::InertiaHorizontalRate(const float DeltaTime)
{
	LeftRate = FMath::Clamp(LeftRate - HorizontalDecelerationRate * GetWorld()->DeltaTimeSeconds, UnderRate, TopRate);
	RightRate = FMath::Clamp(RightRate - HorizontalDecelerationRate * GetWorld()->DeltaTimeSeconds, UnderRate, TopRate);
}

void AFlyRocket::ResetHorizontalRate()
{
	LeftRate = UnderRate;
	RightRate = UnderRate;
}


// ============================================================================= Gearメソッド群 ============================================================================== //
// ========================================================================================================================================================================== //

void AFlyRocket::ChangeGear(const uint8 Gear)
{
	if (Gear >= 0 && Gear < MaxGearSpeedList.Num())
	{
		CurrentGear = Gear;
		TargetGearSpeed = MaxGearSpeedList[Gear];
		GearChangeStartTime = GetWorld()->GetTimeSeconds();
	}
}

void AFlyRocket::GearUp()
{
	ChangeGear(CurrentGear + 1);
}

void AFlyRocket::GearDown()
{
	ChangeGear(CurrentGear - 1);
}

void AFlyRocket::LerpGearSpeed(const float DeltaTime)
{
	// GearChangeからの経過時間
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeElapsed = CurrentTime - GearChangeStartTime;

	// 徐々に速度を上げていく
	if (TimeElapsed < GearChangeDuration)
	{
		// 速度差
		const float SpeedDiff = TargetGearSpeed - CurrentGearSpeed;
		// 線形補間
		const float SpeedChange = (SpeedDiff / GearChangeDuration) * DeltaTime;
		CurrentGearSpeed += SpeedChange;
	}
	else
	{
		CurrentGearSpeed = TargetGearSpeed;
	}
}

void AFlyRocket::ResetGear()
{
	CurrentGear = 0;
	TargetGearSpeed = MaxGearSpeedList[CurrentGear];
	CurrentGearSpeed = TargetGearSpeed;
}


// ============================================================================= Stunメソッド群 ============================================================================== //
// ========================================================================================================================================================================== //

void AFlyRocket::Stun()
{
	bIsStunned = true;
	StunStartTime = GetWorld()->GetTimeSeconds();
	ResetForwardAccelerate();
	ResetHorizontalRate();
	ResetGear();
}

void AFlyRocket::UpdateStunRate(const float DeltaTime)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeElapsed = CurrentTime - StunStartTime;

	// スタン状態へ移行
	float TargetDuration = StunDeclineDuration;

	if (TimeElapsed < TargetDuration)
	{
		// スタン状態への移行
		CurrentStunRate = FMath::Lerp(TopRate, UnderRate, TimeElapsed / StunDeclineDuration);
		return;
	}

	// スタン状態を継続
	TargetDuration = StunDeclineDuration + StunDuration;
	
	if (TimeElapsed < TargetDuration)
	{
		CurrentStunRate = UnderRate;
		return;
	}

	// スタン状態を回復
	TargetDuration = StunDeclineDuration + StunDuration + StunRecoveryDuration;

	if (TimeElapsed < TargetDuration)
	{
		float RecoveryTime = TimeElapsed - StunDeclineDuration - StunDuration;
		CurrentStunRate = FMath::Lerp(UnderRate, TopRate, RecoveryTime / StunRecoveryDuration);
		return;
	}

	CurrentStunRate = TopRate;
	bIsStunned = false;
}