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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<USceneComponent> BodyRoot{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UStaticMeshComponent> BodyFirst{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UStaticMeshComponent> BodySecond{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UStaticMeshComponent> BodyThird{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UCapsuleComponent> CollisionComponent{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UArrowComponent> DebugRightArrow{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UArrowComponent> DebugLeftArrow{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FlyRocketMappingContext{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> HorizontalAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric)
	float AdvanceSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric)
	float HorizontalSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric)
	float HorizontalOffsetMaxLimit{ 550.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric)
	float HorizontalOffsetMinLimit{ -550.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GearSpeedRate{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HorizontalSpeedRate{ 1.0f };

	float HorizSpeedRateMax{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HorizSpeedRateMin{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Numeric, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float StunSpeedRate{ 1.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsMovingHorizontal{ false };



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
		�����ړ����[�g��ϓ�������
		SLAP:������
	*/
	void VaryingRates(const float DeltaTime);

	/*
		�����ړ����[�g��ϓ�������
		SLAP:�ᐅ��
	*/
	void VaryingHorizontalSpeedRate(const float DeltaTime);
};
