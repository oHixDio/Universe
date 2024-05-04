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
	
	// ���̃Q�[����4�̂̃|�[�������������B
	// �������ꂽ�|�[����GMB���Ǘ�����
	// TrackNumber�ɑΉ�����Pawn��GMB����擾����

	// Rocket�擾���@�ɂ��čl����
	// �P�A�擾�ł���܂�GMB�ɖ₢���킹�𑱂���@�~
	// �Q�A��莞�Ԍ�A�擾��₢���킹��B�@��ԎG�Ȏ������@���H
	// �R�A�擾�t���[��g�ݗ��Ă�@�����x�����߂�

public:	
	AFlyTrack();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
};
