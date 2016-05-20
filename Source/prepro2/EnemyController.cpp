// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Enemy_RealTest.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.h"



AEnemyController::AEnemyController()
{
	mBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
	AddInstanceComponent(mBlackboard);
	mBehaviortree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	AddOwnedComponent(mBehaviortree);

	bWantsPlayerState = true;
	PrimaryActorTick.bCanEverTick = true;
}



void AEnemyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

AEnemy_RealTest* Enemy=Cast<AEnemy_RealTest>(InPawn);

	// start behavior
	if (Enemy)
	{
		if (Enemy->BehaviorTree->BlackboardAsset)
		{
			mBlackboard->InitializeBlackboard(*Enemy->BehaviorTree->BlackboardAsset);
			//mBlackboard->SetValueAsString("Enemy1", Enemy->Type);
		}
		mBehaviortree->StartTree(*Enemy->BehaviorTree);
	}
}

void AEnemyController::BeginInactiveState()
{
	Super::BeginInactiveState();
}


void AEnemyController::SetTargetEnemy(APawn* Target)
{
	if (mBlackboard)
	{
		mBlackboard->SetValueAsObject("Player", Target);
	}
}