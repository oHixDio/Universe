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
	TObjectPtr<UInputAction> MoveLeftAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveRightAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GearChangeAction{ nullptr };


	/*
		============================== Move�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving{ false };

	float DefaultRate{ 1.0f };

	float UnderRate{ 0.0f };

	// ===== �����o���\�b�h ===== //
private:
	/*
		���P�b�g�̈ړ���Ԃ�ύX���郁�\�b�h
	*/
	void SetIsMoving(bool IsMoving);


	/*
		============================== Forward�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardAccelerationAmount{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Forward", meta = (AllowPrivateAccess = "true"))
	float CurrentForwardAcceleration{ 0.0f };

	// ===== �����o���\�b�h ===== //
private:
	/*
		Z������O���Ƃ����O�i�ړ����s���B
		�O�i�ړ��ʂ�ForwardSpeed�Ɋi�[����Ă���l�ɂ�茈�肳���

		���̃��\�b�h��Tick�ɂď��������
	*/
	void MoveForward(const float DeltaTime);

	/*
		�O�i�ړ������ʉ���������B
		����������ʂ�ForwardAccelerationAmount�Ɋi�[����Ă���l�ɂ�茈�肷��B
	*/
	void ForwardAccelerate();

	/*
		�O�i�ړ��̉������v�l�����Z�b�g���A�������x�ɖ߂��B
	*/
	void ResetForwardAccelerate();

	/*
	
	*/
	float GetAffectForwardSpeedRate();

	/*
		============================== Horizontal�����o�Q ==============================
	*/
private:
	// ===== �����o�t�B�[���h ===== //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalDecelerationRate{ 0.5f };

	float LeftRate{ 0.0f };

	float RightRate{ 0.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	bool bIsLeftMoving{ false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	bool bIsRightMoving{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalMoveOffset{ 550.0f };

	// ===== �����o���\�b�h ===== //
	/*
		Y������Ƃ��������ړ����s���B
		���̊֐��́ATick�֐��ɂď�Ɏ��s����Ă��邪�A�����̒l�ɂ���Ă͈ړ����s��Ȃ��B
		���̊֐���Tick�ɂď��������
		SLAP:������
	*/
	void MoveHorizontal(const float DeltaTime);

	/*
	*/
	void MoveLeft(const float Value);

	/*
	*/
	void MoveRight(const float Value);

	/*
	*/
	void UpdateHorizontalRate(float DeltaTime);

	/*
	*/
	void ResetHorizontalRate();

	float GearAcceleration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearAccelerationVaryingtime{ 0.5f };

	EGearState CurrentGear{ LOW };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearAccelerationInLow{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearAccelerationInNormal{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Gear", meta = (AllowPrivateAccess = "true"))
	float GearAccelerationInHigh{ 1.2f };


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

	FTimerHandle StunTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsStun{ false };




public:
	AFlyRocket();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


private:
	


	/*
		PlayerController�̓��͂ɂ���āA�@�̂����Ɉړ�������B
	*/
	void NIMoveLeft(const FInputActionValue& Value);

	/*
		PlayerController�̓��͂ɂ���āA�@�̂��E�Ɉړ�������B
	*/
	void NIMoveRight(const FInputActionValue& Value);

	/*
		PlayerController�ɂ�鍶�ړ����͂��I�������Ƃ��ɔ��s�����
	*/
	void ComplatedMoveLeft();

	/*
		PlayerController�ɂ��E�ړ����͂��I�������Ƃ��ɔ��s�����
	*/
	void ComplatedMoveRight();

	/*
		Gear��ς���
		�A�b�v�_�E���́A���͒l�̐���(�^�U)�Ŕ��肳���B
		SLAP:������
	*/
	void NIGearChange(const FInputActionValue& Value);

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
		�����ړ����[�g��ϓ�������
		SLAP:������
	*/
	void VaryingAccelerations(const float DeltaTime);

	/*
		�M�A���x���[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingGearAcceleration(const float DeltaTime);

	/*
		�X�^�����x���[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingStunSpeedRate(const float DeltaTime);

	/*
		�R���W�����̃I�[�o�[���b�v�C�x���g�Ƀo�C���h����֐��B
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
