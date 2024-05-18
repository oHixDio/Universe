// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

class USphereComponent;

UCLASS(ABSTRACT)
class UNIVERSE_API AAsteroid : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UStaticMeshComponent> Mesh{ nullptr };

public:	
	AAsteroid();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
};
