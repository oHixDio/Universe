// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlyRocketGameModeBase.generated.h"

class AAIController;


/**
 * 
 */
UCLASS(ABSTRACT)
class UNIVERSE_API AFlyRocketGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

	/*
		============================== Coreメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //

	// ===== メンバメソッド ===== //
public:
	AFlyRocketGameModeBase();

	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;




	/*
	============================== CreateControllersメンバ群 ==============================
	*/
	// ===== メンバフィールド ===== //
private:
	/*
	
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "4", AllowPrivateAccess = "true"))
	uint8 TotalPlayerNums{ 1 };

	/*

	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "4", AllowPrivateAccess = "true"))
	uint8 PlayerNums{ 1 };

	/*

	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<int32, APlayerController*> PCList;

	/*

	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<int32, AAIController*> AICList;

	/*

	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<int32, AController*> CList;

	// ===== メンバメソッド ===== //
private:
	/*
	*/
	UFUNCTION(BlueprintCallable)
	void CreateControllers();

	/*
	*/
	void ApplyPlayerController(const int32 ControllerID, APlayerController* PlayerController);

	/*
	*/
	void ApplyAIController(const int32 ControllerID, AAIController* AIController);

	/*
	*/
	void ApplyController(const int32 ControllerID, AController* Controller);

};
