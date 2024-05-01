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

	// ���݂̈ʒu����Z�����ɐi�s
	FVector AddLocation = FVector(0.0f, 0.0f, CurrentSpeed);
	
	AddActorWorldOffset(AddLocation);

	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(CurrentSpeed), true, true, FColor::Red,2.0f,FName("Speed"));
}

void AFlyRocket::MoveHorizontal(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// �p�����͂Ő����ړ����x�㏸
	float AddY = MovementVector.Y * HorizontalSpeed * UKismetMathLibrary::SafeDivide(HorizSpeedRateMax, HorizontalSpeedRate) * DeltaTime;

	FVector AddLocation = FVector(0.0f, AddY, 0.0f);

	FVector MovedLocation = BodyRoot->GetRelativeLocation() + AddLocation;

	// MoveLimit�͈͓��Ȃ�Γ���
	if (MovedLocation.Y < HorizontalOffsetMaxLimit && MovedLocation.Y > HorizontalOffsetMinLimit)
	{
		BodyRoot->AddLocalOffset(AddLocation);
		bIsMovingHorizontal = true;
	}
}

void AFlyRocket::GearChange(const FInputActionValue& Value)
{
	// ���[�U�[���琳�����̓��͂��󂯎��
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
		/* �����s��Ȃ� */
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
		/* �����s��Ȃ� */
		break;
	default:
		/* �����s��Ȃ� */
		break;
	}
}

void AFlyRocket::GearDown()
{
	switch (CurrentGear)
	{
	case AFlyRocket::LOW:
		/* �����s��Ȃ� */
		break;
	case AFlyRocket::NORMAL:
		CurrentGear = LOW;
		break;
	case AFlyRocket::HIGH:
		CurrentGear = NORMAL;
		break;
	default:
		/* �����s��Ȃ� */
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
	
	// �ڕW�̃��[�g�l�����߂�
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

	// �ڕW�̃��[�g�l�Ȃ�ΏI��
	if (GearSpeedRate == TargetRate)
	{
		UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(GearSpeedRate), true, true, FColor::Yellow, 2.0f, FName("GearSpeedRate"));
		return;
	}

	// �ڕW�̃��[�g�l�ɂȂ�悤�f���^�^�C���������Z
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
		// Min�l���Ⴍ�Ȃ�Ȃ��B
		HorizontalSpeedRate = FMath::Max(HorizontalSpeedRate - DeltaTime, HorizSpeedRateMin);
	}
	else
	{
		// Max�l��荂���Ȃ�Ȃ�
		HorizontalSpeedRate = FMath::Min(HorizontalSpeedRate + DeltaTime, HorizSpeedRateMax);
	}

	// UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(HorizontalSpeedRate), true, true, FColor::Black, 2.0f, FName("Rate"));
}



