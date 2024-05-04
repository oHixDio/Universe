// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyTrack.generated.h"

USTRUCT(Blueprintable)
struct FAsteroidSpawnPattern
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFirst{ false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSecond{ false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bThird{ false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFourth{ false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFifth{ false };
};

UCLASS()
class UNIVERSE_API AFlyTrack : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> StartPoint{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> FlyRocket{ nullptr };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	uint8 TrackNumber{ 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	TArray<FAsteroidSpawnPattern> AsteroidSpawnPatternList;
	
	// このゲームは4体のポーンが生成される。
	// 生成されたポーンはGMBが管理する
	// TrackNumberに対応するPawnをGMBから取得する

	// Rocket取得方法について考える
	// １、取得できるまでGMBに問い合わせを続ける　×
	// ２、一定時間後、取得を問い合わせる。　一番雑な実装方法か？
	// ３、取得フローを組み立てる　結合度を高める

public:	
	AFlyTrack();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
};
