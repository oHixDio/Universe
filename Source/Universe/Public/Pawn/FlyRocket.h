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
		Gearを変える
		アップダウンは、入力値の正負(真偽)で判定される。
		SLAP:高水準
	*/
	void GearChange(const FInputActionValue& Value);

	/*
		Gearを上げる
		SLAP:中水準
	*/
	void GearUp();

	/*
		Gearを下げる
		SLAP:中水準
	*/
	void GearDown();

	/*
		アステロイド衝突時、スタンを発生させ、一定秒数間スピードが0になる。
		スタンタイムをスタートさせる。
		SLAP:中水準
	*/
	void Stun();

	/*
		スピードアップするかどうかを判定する
		SLAP:中水準
	*/
	void JudgeSpeedUp();

	/*
		スピードアップモードを解除する
		SLAP:中水準
	*/
	void ResetSpeedUpMode();

	/*
		水平移動レートを変動させる
		SLAP:中水準
	*/
	void VaryingRates(const float DeltaTime);

	/*
		ギア速度レートを変動させる
		SLAP:低水準
	*/
	void VaryingGearSpeedRate(const float DeltaTime);

	/*
		水平移動レートを変動させる
		SLAP:低水準
	*/
	void VaryingHorizontalSpeedRate(const float DeltaTime);

	/*
		スタン速度レートを変動させる
		SLAP:低水準
	*/
	void VaryingStunSpeedRate(const float DeltaTime);

	/*
		スピードアップレートを変動させる
		SLAP:低水準
	*/
	void VaryingSpeedUpRate(const float DeltaTime);

	/*
		コリジョンのオーバーラップイベントにバインドする関数。
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
