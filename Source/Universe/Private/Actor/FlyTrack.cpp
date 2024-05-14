// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FlyTrack.h"

#include "DataAsset/FlyRocketASPDataAsset.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AFlyTrack::AFlyTrack()
{
	StartPoint = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = StartPoint;

	PrimaryActorTick.bCanEverTick = true;
}

void AFlyTrack::BeginPlay()
{
	Super::BeginPlay();

	UpdateTrack();
}

void AFlyTrack::UpdateTrack()
{
	// スポーンクラスが指定されていない場合、何もせず終了する
	if (!IsValid(TrackUpdateInfo.SpawnAsteroidClass))
	{
		return;
	}

	// 0回目は抜きに考える。1～DefaultSpawnAmount回する
	// 縦方向のループ
	for (int32 z = 1; z <= TrackUpdateInfo.DefaultSpawnAmount; z++)
	{
		// パターンリストからパターンをランダムに指定する
		int PattenIdx = FMath::RandHelper(TrackUpdateInfo.AsteroidSpawnPattenData->ASPDataSize());
		// 指定されたパターンを保持
		const FAsteroidSpawnPattern& ASP = TrackUpdateInfo.AsteroidSpawnPattenData->ASPDataByIdx(PattenIdx);
		// 横へのスポーン回数を保持
		int32 HorizSpawnNum = ASP.bIsSpawnList.Num();
		// 横方向のループ
		for (int32 y = 0; y < HorizSpawnNum; y++)
		{
			if (ASP.bIsSpawnList[y])
			{
				// StartPointを中心とするため、Halfの値を引く
				int32 HorizNumber = y - HorizSpawnNum / 2;

				// StartPointのLocationにOffset分のLocationを加算
				FVector SpawnLocation = FVector(0, TrackUpdateInfo.SpawnOffsetY * HorizNumber, TrackUpdateInfo.SpawnOffsetZ * z) + GetActorLocation();

				// SpawnClassは関数上部でnullチェックしているため、安全に使用できる
				GetWorld()->SpawnActor<AActor>(TrackUpdateInfo.SpawnAsteroidClass, SpawnLocation, GetActorRotation());
			}
		}
		
	}
	
	
}

void AFlyTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(FlyRocket))
	{
		double Dist = FVector::Dist(GetActorLocation(), FlyRocket->GetActorLocation());
		// UKismetSystemLibrary::PrintString(this, FString::FromInt(Dist / 100), true, true, FColor::Red, 2.0f, FName("Dist"));
	}
}

