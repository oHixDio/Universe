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


// ================================================================================= Core�Q ================================================================================= //
// ========================================================================================================================================================================== //

/*
* �����܂Ńf�t�H���g�̐ݒ�
* ���̐ݒ���f�t�H���g�Ƃ��A�G�f�B�^�ŕύX���s����B
*/
AFlyRocket::AFlyRocket()
{
	// RootComponent���n�߂ɐ�������B
	if (RootComponent == nullptr) 
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	}

	// BodyRoot �����l�ݒ�
	BodyRoot = CreateDefaultSubobject<USceneComponent>("Body Root");
	BodyRoot->SetupAttachment(RootComponent);

	// BodyFirst �����ݒ�
	BodyFirst = CreateDefaultSubobject<UStaticMeshComponent>("Body First");
	BodyFirst->SetupAttachment(BodyRoot);

	// BodySecond �����ݒ�
	BodySecond = CreateDefaultSubobject<UStaticMeshComponent>("Body Second");
	BodySecond->SetupAttachment(BodyRoot);

	// BodyThird �����ݒ�
	BodyThird = CreateDefaultSubobject<UStaticMeshComponent>("Body Third");
	BodyThird->SetupAttachment(BodyRoot);

	// CollisionComponent  �����ݒ�
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("Capcule Collision");
	CollisionComponent->SetupAttachment(BodyRoot);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlyRocket::OnBeginOverlap);

	// SpringArm �����ݒ�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Camera�@�����ݒ�
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// DEBUG Arrow �����ݒ�
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


// ============================================================================= Input���\�b�h�Q ============================================================================= //
// ========================================================================================================================================================================== //

void AFlyRocket::NIMoveHorizontal(const FInputActionValue& Value)
{
	if (bIsStunned)
	{
		return;
	}

	// ���[�U�[���琳���������̓��͂��󂯎��
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

	// ���[�U�[���琳���������̓��͂��󂯎��
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


// ============================================================================== ���J���\�b�h�Q ============================================================================= //
// ========================================================================================================================================================================== //


// ============================================================================ Forward���\�b�h�Q ============================================================================ //
// ========================================================================================================================================================================== //

void AFlyRocket::SetIsMoving(bool IsMoving)
{
	bIsMoving = IsMoving;
}

void AFlyRocket::MoveForward(const float DeltaTime)
{
	float CurrentSpeed = (ForwardSpeed + GetAffectForwardSpeed()) * CurrentStunRate * DeltaTime;

	// ���݂̈ʒu����Z�����ɐi�s
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


// ========================================================================== Horizontal���\�b�h�Q =========================================================================== //
// ========================================================================================================================================================================== //

void AFlyRocket::MoveHorizontal(const float DeltaTime)
{
	// TODO �X�^�����͉��ړ����Ȃ��B

	// ���E�̊�������A�������x���Z�o(-Y�����̂��߁ALeftRate�� - �̉E���ɔz�u)
	float AddY = (RightRate - LeftRate) * HorizontalSpeed * DeltaTime;
	FVector AddLocation = FVector(0.0f, AddY, 0.0f);
	float MovedY = BodyRoot->GetRelativeLocation().Y + AddLocation.Y;

	const float LEFT = -1.0f;
	const float RIGHT = 1.0f;

	// �ړ��\�͈͓��Ȃ��
	if (LEFT * HorizontalMoveOffset < MovedY && MovedY < RIGHT * HorizontalMoveOffset)
	{
		BodyRoot->AddLocalOffset(AddLocation);
	}
}

void AFlyRocket::MoveLeft(const float Value)
{
	// Clamp  ���������A�����������O�����̊ԂŒ�������
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


// ============================================================================= Gear���\�b�h�Q ============================================================================== //
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
	// GearChange����̌o�ߎ���
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeElapsed = CurrentTime - GearChangeStartTime;

	// ���X�ɑ��x���グ�Ă���
	if (TimeElapsed < GearChangeDuration)
	{
		// ���x��
		const float SpeedDiff = TargetGearSpeed - CurrentGearSpeed;
		// ���`���
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


// ============================================================================= Stun���\�b�h�Q ============================================================================== //
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

	// �X�^����Ԃֈڍs
	float TargetDuration = StunDeclineDuration;

	if (TimeElapsed < TargetDuration)
	{
		// �X�^����Ԃւ̈ڍs
		CurrentStunRate = FMath::Lerp(TopRate, UnderRate, TimeElapsed / StunDeclineDuration);
		return;
	}

	// �X�^����Ԃ��p��
	TargetDuration = StunDeclineDuration + StunDuration;
	
	if (TimeElapsed < TargetDuration)
	{
		CurrentStunRate = UnderRate;
		return;
	}

	// �X�^����Ԃ���
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