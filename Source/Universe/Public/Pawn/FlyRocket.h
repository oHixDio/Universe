// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyRocket.generated.h"


class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(ABSTRACT)
class UNIVERSE_API AFlyRocket : public APawn
{
	GENERATED_BODY()

private:
	enum EGearState : uint8
	{
		LOW,NORMAL,HIGH,
	};

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> BodyRoot{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyFirst{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodySecond{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyThird{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CollisionComponent{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> DebugRightArrow{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> DebugLeftArrow{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> FlyRocketMappingContext{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HorizontalAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GearChangeAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Advance", meta = (AllowPrivateAccess = "true"))
	float AdvanceSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalSpeed{ 100.0f };

	float HorizontalSpeedRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (ClampMin = "0.0", ClampMax = "1.0"), meta = (AllowPrivateAccess = "true"))
	float HorizSpeedRateMin{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalOffsetMaxLimit{ 550.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalOffsetMinLimit{ -550.0f };

	float GearSpeedRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearSpeedRateInLow{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearSpeedRateInNormal{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearSpeedRateInHigh{ 1.2f };

	float StunSpeedRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunDuration{ 0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunRecoveryDuration{ 2.0f };

	float SpeedUpRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | SpeedUp", meta = (AllowPrivateAccess = "true"))
	float SpeedUpRateMax{ 1.2f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | SpeedUp", meta = (AllowPrivateAccess = "true"))
	float SpeedUpRequiredTime{ 2.0f };

	float SpeedUpJudgeTime{ 0.0f };

	float DefaultRate{ 1.0f };

	float UnderRate{ 0.0f };

	FTimerHandle StunTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsMovingHorizontal{ false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsStun{ false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsSpeedUpMode{ false };

	EGearState CurrentGear{ LOW };



public:
	AFlyRocket();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


private:
	/*
		Z������O���Ƃ����A�O�i�ړ����s���B
		SLAP:������
	*/
	void MoveAdvance(const float DeltaTime);

	/*
		Y���A�i���O�ړ�����͂ōs���B
		�ړ����A�����Ō�������B
	*/
	void MoveHorizontal(const FInputActionValue& Value);

	/*
		MoveHorizontal�g���K�[�I�����̏���
	*/
	void ComplatedHorizontal();

	/*
		Gear��ς���
		�A�b�v�_�E���́A���͒l�̐���(�^�U)�Ŕ��肳���B
		SLAP:������
	*/
	void GearChange(const FInputActionValue& Value);

	/*
		Gear���グ��
		SLAP:������
	*/
	void GearUp();

	/*
		Gear��������
		SLAP:������
	*/
	void GearDown();

	/*
		�A�X�e���C�h�Փˎ��A�X�^���𔭐������A���b���ԃX�s�[�h��0�ɂȂ�B
		�X�^���^�C�����X�^�[�g������B
		SLAP:������
	*/
	void Stun();

	/*
		�X�s�[�h�A�b�v���邩�ǂ����𔻒肷��
		SLAP:������
	*/
	void JudgeSpeedUp();

	/*
		�X�s�[�h�A�b�v���[�h����������
		SLAP:������
	*/
	void ResetSpeedUpMode();

	/*
		�����ړ����[�g��ϓ�������
		SLAP:������
	*/
	void VaryingRates(const float DeltaTime);

	/*
		�M�A���x���[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingGearSpeedRate(const float DeltaTime);

	/*
		�����ړ����[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingHorizontalSpeedRate(const float DeltaTime);

	/*
		�X�^�����x���[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingStunSpeedRate(const float DeltaTime);

	/*
		�X�s�[�h�A�b�v���[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingSpeedUpRate(const float DeltaTime);

	/*
		�R���W�����̃I�[�o�[���b�v�C�x���g�Ƀo�C���h����֐��B
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
