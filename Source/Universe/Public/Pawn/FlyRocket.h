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
		============================== Coreメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	/*
		ロケットの機体の相対位置の減点
		BodyRootの相対位置を変動させることで左右移動を行う。
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> BodyRoot{ nullptr };

	/*
		機体の先端
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyFirst{ nullptr };

	/*
		機体の中部
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodySecond{ nullptr };

	/*
		機体の後端
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BodyThird{ nullptr };

	/*
		機体の当たり判定を得る目的
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CollisionComponent{ nullptr };

	/*
		デバッグ用の、横移動範囲表示右方向矢印
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> DebugRightArrow{ nullptr };

	/*
		デバッグ用の、横移動範囲表示左方向矢印
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

	// ===== メンバメソッド ===== //
public:
	AFlyRocket();

	virtual void Tick(float DeltaTime) override;

	/*
		プレイヤー入力機構をバインドするためにオーバーライド
	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/*
		コリジョンのオーバーラップイベントにバインドする関数。
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/*
		============================== Inputメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
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

	// ===== メンバメソッド ===== //
private:
	/*
		プレイヤーからの入力を受け取り、FlyRocketの左右移動を行う。
		入力値はfloatで、マイナスが左方向、プラスが右方向に移動する。
	*/
	void NIMoveHorizontal(const FInputActionValue& Value);

	/*
	* 
		プレイヤーからの入力を受け取り、FlyRocketのGearを変える。
		入力値はfloatで、マイナスがGearDown()、プラスがGearUp()を実行する。
	*/
	void NIGearChange(const FInputActionValue& Value);


	/*
		============================== Moveメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	/*
		ロケットの移動状態を表す
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving{ false };

	/*
		割合値100%
	*/
	float TopRate{ 1.0f };

	/*
		割合値0％
	*/
	float UnderRate{ 0.0f };

	// ===== メンバメソッド ===== //
private:
	/*
		ロケットの移動状態を変更する。
	*/
	void SetIsMoving(bool IsMoving);


	/*
		============================== Forwardメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	/*
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move|Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardSpeed{ 100.0f };

	/*
		加速量
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move|Forward", meta = (AllowPrivateAccess = "true"))
	float ForwardAdditionalSpeedAmount{ 10.0f };

	/*
		現在の総加速量	
	*/
	float TotalForwardAdditionalSpeed{ 0.0f };

	// ===== メンバメソッド ===== //
private:
	/*
		Z方向を前方とした前進移動を行う。
		前進移動量はForwardSpeedに格納されている値により決定される

		Tickにて処理される
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
		前進速度に影響する速度を取得する
	*/
	float GetAffectForwardSpeed();


	/*
		============================== Horizontalメンバ群 ==============================
	*/
private:
	// ===== メンバフィールド ===== //

	/*
		左右移動速度。Y軸
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalSpeed{ 100.0f };

	/*
		左右移動速度が減少する値
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalDecelerationRate{ 0.5f };

	/*
		左右移動速度が上昇する値
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
		左右にどれだけ移動できるかのスカラー値
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Horizontal", meta = (AllowPrivateAccess = "true"))
	float HorizontalMoveOffset{ 550.0f };

	// ===== メンバメソッド ===== //
	/*
		Y軸を基準とした水平移動を行う。
		この関数は、Tick関数にて常に実行されているが、内部の値によっては移動を行わない。
		Tickにて処理される
		SLAP:中水準
	*/
	void MoveHorizontal(const float DeltaTime);

	/*
		左の移動値を上昇させる
	*/
	void MoveLeft(const float Value);

	/*
		右の移動値を上昇させる
	*/
	void MoveRight(const float Value);

	/*
		左右移動の慣性を計算する
	*/
	void InertiaHorizontalRate(const float DeltaTime);

	/*
		左右移動値を0にする。
	*/
	void ResetHorizontalRate();


	/*
	============================== Gearメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	/*
		現在のギアを表す
	*/
	uint8 CurrentGear{ 0 };

	/*
	
	*/
	float CurrentGearSpeed{ 0.0f };

	/*
		各ギアに対応した、目標値
	*/
	float TargetGearSpeed{ 0.0f };

	/*
		ギアが変更されたときのタイム
	*/
	float GearChangeStartTime{ 0.0f };


	/*
		ギア変更後、速度を目標値にするまでに掛かる時間
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Gear", meta = (AllowPrivateAccess = "true"))
	float GearChangeDuration{ 0.5f };

	/*
		各ギアの速度のリスト
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move | Gear", meta = (AllowPrivateAccess = "true"), EditFixedSize)
	TArray<float> MaxGearSpeedList{ 0.0f, 200.0f, 400.0f };

	// ===== メンバメソッド ===== //
private:
	/*
		引数に対応したギア速度に変更する
	*/
	void ChangeGear(const uint8 Gear);

	/*
		Gearを上げる
	*/
	void GearUp();

	/*
		Gearを下げる
	*/
	void GearDown();

	/*
		ギア速度を目標値に線形補間で変更する
	*/
	void LerpGearSpeed(const float DeltaTime);

	/*
		ギアをデフォルトにする
	*/
	void ResetGear();


	/*
	============================== Stunメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	/*
		スタン状態の時間
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunDuration{ 1.0f };

	/*
		スタン状態に至るまでの時間
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric | Stun", meta = (AllowPrivateAccess = "true"))
	float StunDeclineDuration{ 0.2f };

	/*
		スタン状態から回復するまでの時間	
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

	// ===== メンバメソッド ===== //
private:
	/*
		アステロイド衝突時、スタンを発生させ、一定秒数間スピードが0になる。
		スタンタイムをスタートさせる。
		SLAP:中水準
	*/
	void Stun();

	/*
		スタン値を変動させる
	*/
	void UpdateStunRate(const float DeltaTime);
};