// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Enemy_RealTest.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Engine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "EnemyController.h"



AEnemyController::AEnemyController()
	:/* perceptionComponent(CreateDefaultSubobject< UAIPerceptionComponent >(TEXT("PerceptionComp")))
	, sightConfig(CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight")))
	, soundConfig(CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Hearing")))
	,*/mBlackboard (CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard")))
	,mBehaviortree (CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree")))
{
	AddInstanceComponent(mBlackboard);
	AddOwnedComponent(mBehaviortree);

	bWantsPlayerState = true;
	PrimaryActorTick.bCanEverTick = true;

	/*perceptionComponent->ConfigureSense(*sightConfig);
	perceptionComponent->ConfigureSense(*soundConfig);
	perceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());

	perceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::SenseStuff);
	*/
}



void AEnemyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

AEnemy_RealTest* Enemy=Cast<AEnemy_RealTest>(InPawn);
if (Enemy&&Enemy->BehaviorTree)
{
	mBlackboard->InitializeBlackboard(*Enemy->BehaviorTree->BlackboardAsset);
	
	TargetKeyID = mBlackboard->GetKeyID("Target");
	TargetLocationID = mBlackboard->GetKeyID("TargetLocation");

	mBehaviortree->StartTree(*Enemy->BehaviorTree);
	
}
//sightConfig->SightRadius = 3000.0f;
//sightConfig->LoseSightRadius = 3500.f;
//sightConfig->PeripheralVisionAngleDegrees = 90.0f;
//sightConfig->DetectionByAffiliation.bDetectEnemies = true;
//sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
//
//perceptionComponent->ConfigureSense(*sightConfig);
//
//soundConfig->HearingRange = 4000.0f;
//soundConfig->bUseLoSHearing = false;
//soundConfig->DetectionByAffiliation.bDetectEnemies = true;
//soundConfig->DetectionByAffiliation.bDetectNeutrals = true;
//soundConfig->DetectionByAffiliation.bDetectFriendlies = true;
//
//perceptionComponent->ConfigureSense(*soundConfig);

	// start behavior
	//if (Enemy)
	//{
	//	if (Enemy->BehaviorTree->BlackboardAsset)
	//	{
	//		mBlackboard->InitializeBlackboard(*Enemy->BehaviorTree->BlackboardAsset);
	//		//mBlackboard->SetValueAsString("Enemy1", Enemy->Type);
	//	}
	//	mBehaviortree->StartTree(*Enemy->BehaviorTree);
	//}
}

void AEnemyController::BeginInactiveState()
{
	Super::BeginInactiveState();
}


void AEnemyController::SetTargetEnemy(APawn* Target)
{
	if (mBlackboard)
	{
		mBlackboard->SetValueAsObject(TargetKeyID, Target);
		mBlackboard->SetValueAsVector(TargetLocationID, Target->GetActorLocation());

	}
}

/*
void AEnemyController::ClearTarget()
{
	if (mBlackboard)
	{
		mBlackboard->SetValueAsObject(TargetKeyID, nullptr);
		//mBlackboard->SetValueAsVector(TargetLocationID, NULL);

	}
}
*/
void AEnemyController::SenseStuff(TArray<AActor*> testActors)
{
	//	mController->MoveToActor(testActors[0]);
	//mTargetPos = testActors[0]->GetActorLocation();
	//UNavigationSystem::SimpleMoveToActor(GetController(), testActors[0]);
	/*TArray<AActor*> seenActors, heardActors;
	perceptionComponent->GetPerceivedActors(UAISense_Sight::StaticClass(), seenActors);
	perceptionComponent->GetPerceivedActors(UAISense_Hearing::StaticClass(), heardActors);
	if (seenActors.Num() > 0)
	{
		SetTargetEnemy(Cast<APawn>(seenActors[0]));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");

	}*/
	/*if (seenActors.Num() > 0)
	{
		if (seenActors.Max() > seenActors.Num())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I don't see you!");

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
		}
	}

	if (heardActors.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "I hear you!");
	}*/


	//UNavigationSystem::SimpleMoveToLocation(mController, testActors[0]->GetActorLocation());
	//FVector Movement = GetActorLocation() - testActors[0]->GetActorLocation();
	//Movement /= 3;
	//SetActorLocation(GetActorLocation() - Movement);
}