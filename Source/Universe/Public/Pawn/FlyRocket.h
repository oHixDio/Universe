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

	/*
		============================== Core�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	/*
		���P�b�g�̋@�̂̑��Έʒu�̌��_
		BodyRoot�̑��Έʒu��ϓ������邱�Ƃō��E�ړ����s���B
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> BodyRoot{ nullptr };

	/*
		�@�̂̐�[
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyFirst{ nullptr };

	/*
		�@�̂̒���
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodySecond{ nullptr };

	/*
		�@�̂̌�[
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyThird{ nullptr };

	/*
		�@�̂̓����蔻��𓾂�ړI
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CollisionComponent{ nullptr };

	/*
		�f�o�b�O�p�́A���ړ��͈͕\���E�������
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> DebugRightArrow{ nullptr };

	/*
		�f�o�b�O�p�́A���ړ��͈͕\�����������
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> DebugLeftArrow{ nullptr };

	/*
		
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom{ nullptr };

	/*
	
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera{ nullptr };

	// ===== �����o���\�b�h ===== //
public:
	AFlyRocket();

	virtual void Tick(float DeltaTime) override;

	/*
		�v���C���[���͋@�\���o�C���h���邽�߂ɃI�[�o�[���C�h
	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/*
		�R���W�����̃I�[�o�[���b�v�C�x���g�Ƀo�C���h����֐��B
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/*
		============================== Input�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	/*
		
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> FlyRocketMappingContext{ nullptr };

	/*
		
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveHorizontalAction{ nullptr };

	/*

	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GearChangeAction{ nullptr };

	// ===== �����o���\�b�h ===== //
private:
	/*
		�v���C���[����̓��͂��󂯎��AFlyRocket�̍��E�ړ����s���B
		���͒l��float�ŁA�}�C�i�X���������A�v���X���E�����Ɉړ�����B
	*/
	void NIMoveHorizontal(const FInputActionValue& Value);

	/*
	* 
		�v���C���[����̓��͂��󂯎��AFlyRocket��Gear��ς���B
		���͒l��float�ŁA�}�C�i�X��GearDown()�A�v���X��GearUp()�����s����B
	*/
	void NIGearChange(const FInputActionValue& Value);


	/*
		============================== Move�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	/*
		���P�b�g�̈ړ���Ԃ�\��
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving{ false };

	/*
		�����l100%
	*/
	float TopRate{ 1.0f };

	/*
		�����l0��
	*/
	float UnderRate{ 0.0f };

	// ===== �����o���\�b�h ===== //
private:
	/*
		���P�b�g�̈ړ���Ԃ�ύX����B
	*/
	void SetIsMoving(bool IsMoving);


	/*
		============================== Forward�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	/*
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move|Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardSpeed{ 100.0f };

	/*
		������
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move|Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardAdditionalSpeedAmount{ 10.0f };

	/*
		���݂̑�������	
	*/
	float TotalForwardAdditionalSpeed{ 0.0f };

	// ===== �����o���\�b�h ===== //
private:
	/*
		Z������O���Ƃ����O�i�ړ����s���B
		�O�i�ړ��ʂ�ForwardSpeed�Ɋi�[����Ă���l�ɂ�茈�肳���

		Tick�ɂď��������
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
		�O�i���x�ɉe�����鑬�x���擾����
	*/
	float GetAffectForwardSpeed();


	/*
		============================== Horizontal�����o�Q ==============================
	*/
private:
	// ===== �����o�t�B�[���h ===== //

	/*
		���E�ړ����x�BY��
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalSpeed{ 100.0f };

	/*
		���E�ړ����x����������l
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalDecelerationRate{ 0.5f };

	/*
		���E�ړ����x���㏸����l
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalAccelelerationRate{ 0.5f };

	/*
	
	*/
	float LeftRate{ 0.0f };

	/*
	
	*/
	float RightRate{ 0.0f };

	/*
		���E�ɂǂꂾ���ړ��ł��邩�̃X�J���[�l
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalMoveOffset{ 550.0f };

	// ===== �����o���\�b�h ===== //
	/*
		Y������Ƃ��������ړ����s���B
		���̊֐��́ATick�֐��ɂď�Ɏ��s����Ă��邪�A�����̒l�ɂ���Ă͈ړ����s��Ȃ��B
		Tick�ɂď��������
		SLAP:������
	*/
	void MoveHorizontal(const float DeltaTime);

	/*
		���̈ړ��l���㏸������
	*/
	void MoveLeft(const float Value);

	/*
		�E�̈ړ��l���㏸������
	*/
	void MoveRight(const float Value);

	/*
		���E�ړ��̊������v�Z����
	*/
	void InertiaHorizontalRate(const float DeltaTime);

	/*
		���E�ړ��l��0�ɂ���B
	*/
	void ResetHorizontalRate();


	/*
	============================== Gear�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	/*
		���݂̃M�A��\��
	*/
	uint8 CurrentGear{ 0 };

	/*
	
	*/
	float CurrentGearSpeed{ 0.0f };

	/*
		�e�M�A�ɑΉ������A�ڕW�l
	*/
	float TargetGearSpeed{ 0.0f };

	/*
		�M�A���ύX���ꂽ�Ƃ��̃^�C��
	*/
	float GearChangeStartTime{ 0.0f };


	/*
		�M�A�ύX��A���x��ڕW�l�ɂ���܂łɊ|���鎞��
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Gear", meta = (AllowPrivateAccess = "true"))
	float GearChangeDuration{ 0.5f };

	/*
		�e�M�A�̑��x�̃��X�g
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Gear", meta = (AllowPrivateAccess = "true"), EditFixedSize)
	TArray<float> MaxGearSpeedList{ 0.0f, 200.0f, 400.0f };

	// ===== �����o���\�b�h ===== //
private:
	/*
		�����ɑΉ������M�A���x�ɕύX����
	*/
	void ChangeGear(const uint8 Gear);

	/*
		Gear���グ��
	*/
	void GearUp();

	/*
		Gear��������
	*/
	void GearDown();

	/*
		�M�A���x��ڕW�l�ɐ��`��ԂŕύX����
	*/
	void LerpGearSpeed(const float DeltaTime);

	/*
		�M�A���f�t�H���g�ɂ���
	*/
	void ResetGear();


	/*
	============================== Stun�����o�Q ==============================
	*/
	// ===== �����o�t�B�[���h ===== //
private:
	/*
		�X�^����Ԃ̎���
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunDuration{ 1.0f };

	/*
		�X�^����ԂɎ���܂ł̎���
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunDeclineDuration{ 0.2f };

	/*
		�X�^����Ԃ���񕜂���܂ł̎���	
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunRecoveryDuration{ 1.0f };

	/*
		
	*/
	float CurrentStunRate{ 1.0f };

	/*
	
	*/
	bool bIsStunned{ false };

	/*
	
	*/
	float StunStartTime{ 0.0f };

	// ===== �����o���\�b�h ===== //
private:
	/*
		�A�X�e���C�h�Փˎ��A�X�^���𔭐������A���b���ԃX�s�[�h��0�ɂȂ�B
		�X�^���^�C�����X�^�[�g������B
		SLAP:������
	*/
	void Stun();

	/*
		�X�^���l��ϓ�������
	*/
	void UpdateStunRate(const float DeltaTime);
};