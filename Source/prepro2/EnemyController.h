// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class PREPRO2_API AEnemyController : public AAIController
{
	GENERATED_BODY()
		AEnemyController();
	UBlackboardComponent* mBlackboard;

	
	UBehaviorTreeComponent* mBehaviortree;

	virtual void Possess(class APawn* InPawn) override;

	virtual void BeginInactiveState() override;
	
	

	UPROPERTY(EditAnywhere)
		UAIPerceptionComponent* mPerceptionComponent;
	UPROPERTY(EditAnywhere)
		UAISenseConfig_Sight* sightConfig;
	UPROPERTY(EditAnywhere)
		UAISenseConfig_Hearing* soundConfig;

	//sensing stuff
	UFUNCTION()
		void SenseStuff(TArray<AActor*> testActors);

	uint8 PlayerID;
	uint8 PlayerLastSeenLocationID;
	uint8 BombLocationID;
	uint8 BombHeardLocationID;
	uint8 PlayerHeardLocationID;
	uint8 PlayerSeenID;
	uint8 SecondaryLocationID;
	uint8 PlayerMovementDirectionID;
	uint8 isAliveID;

	AEnemy_RealTest* mOwner;
	
public:
	void SetTargetEnemy(APawn * Target);
	void UpdateStatus();
	//void ClearTarget();
};
