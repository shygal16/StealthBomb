// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "prepro2Character.h"
#include "Enemy_RealTest.h"
#include "MyLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class PREPRO2_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	//void BeginPlay() override;

public:

	AMyLevelScriptActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoints")
	TArray<FVector> mSpawnPointsSet1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	TArray<AEnemy_RealTest*> mEnemies;
	
	void FirstEnemyDeathEvent();
	
};
