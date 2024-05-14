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
		============================== Moveメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving{ false };

	float DefaultRate{ 1.0f };

	float UnderRate{ 0.0f };

	// ===== メンバメソッド ===== //
private:
	/*
		ロケットの移動状態を変更するメソッド
	*/
	void SetIsMoving(bool IsMoving);


	/*
		============================== Forwardメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardAccelerationAmount{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Forward", meta = (AllowPrivateAccess = "true"))
	float CurrentForwardAcceleration{ 0.0f };

	// ===== メンバメソッド ===== //
private:
	/*
		Z方向を前方とした前進移動を行う。
		前進移動量はForwardSpeedに格納されている値により決定される

		このメソッドはTickにて処理される
	*/
	void MoveForward(const float DeltaTime);

	/*
		前進移動を一定量加速させる。
		加速させる量はForwardAccelerationAmountに格納されている値により決定する。
	*/
	void ForwardAccelerate();

	/*
		前進移動の加速合計値をリセットし、初期速度に戻す。
	*/
	void ResetForwardAccelerate();

	/*
	
	*/
	float GetAffectForwardSpeedRate();

	/*
		============================== Horizontalメンバ群 ==============================
	*/
private:
	// ===== メンバフィールド ===== //

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

	// ===== メンバメソッド ===== //
	/*
		Y軸を基準とした水平移動を行う。
		この関数は、Tick関数にて常に実行されているが、内部の値によっては移動を行わない。
		この関数はTickにて処理される
		SLAP:中水準
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
		PlayerControllerの入力によって、機体を左に移動させる。
	*/
	void NIMoveLeft(const FInputActionValue& Value);

	/*
		PlayerControllerの入力によって、機体を右に移動させる。
	*/
	void NIMoveRight(const FInputActionValue& Value);

	/*
		PlayerControllerによる左移動入力を終了したときに発行される
	*/
	void ComplatedMoveLeft();

	/*
		PlayerControllerによる右移動入力を終了したときに発行される
	*/
	void ComplatedMoveRight();

	/*
		Gearを変える
		アップダウンは、入力値の正負(真偽)で判定される。
		SLAP:高水準
	*/
	void NIGearChange(const FInputActionValue& Value);

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
		水平移動レートを変動させる
		SLAP:中水準
	*/
	void VaryingAccelerations(const float DeltaTime);

	/*
		ギア速度レートを変動させる
		SLAP:低水準
	*/
	void VaryingGearAcceleration(const float DeltaTime);

	/*
		スタン速度レートを変動させる
		SLAP:低水準
	*/
	void VaryingStunSpeedRate(const float DeltaTime);

	/*
		コリジョンのオーバーラップイベントにバインドする関数。
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
