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
		Z方向を前方とした、前進移動を行う。
		SLAP:中水準
	*/
	void MoveAdvance(const float DeltaTime);

	/*
		Y軸アナログ移動を入力で行う。
		移動時、割合で減速する。
	*/
	void MoveHorizontal(const FInputActionValue& Value);

	/*
		MoveHorizontalトリガー終了時の処理
	*/
	void ComplatedHorizontal();

	/*
		水平移動レートを変動させる
		SLAP:中水準
	*/
	void VaryingRates(const float DeltaTime);

	/*
		水平移動レートを変動させる
		SLAP:低水準
	*/
	void VaryingHorizontalSpeedRate(const float DeltaTime);
};
