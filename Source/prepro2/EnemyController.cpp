// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Enemy_RealTest.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Engine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "prepro2Character.h"
#include "DetonateBomb.h"
#include "EnemyController.h"



AEnemyController::AEnemyController()
	: mPerceptionComponent(CreateDefaultSubobject< UAIPerceptionComponent >(TEXT("PerceptionComp")))
	, sightConfig(CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight")))
	, soundConfig(CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Hearing")))
	, mBlackboard (CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard")))
	,mBehaviortree (CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree")))
	, VisionRangeMin(1000.0f)
	, VisionRangeMax(1500.0f)
	, HearRange(12000.0f)
{
	AddInstanceComponent(mBlackboard);
	AddOwnedComponent(mBehaviortree);

	bWantsPlayerState = true;
	PrimaryActorTick.bCanEverTick = true;

	mPerceptionComponent->ConfigureSense(*sightConfig);
	mPerceptionComponent->ConfigureSense(*soundConfig);
	mPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());

	mPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::SenseStuff);
	
}



void AEnemyController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	

	mOwner =Cast<AEnemy_RealTest>(InPawn);

	if (mOwner&&mOwner->BehaviorTree)
	{
		mBlackboard->InitializeBlackboard(*mOwner->BehaviorTree->BlackboardAsset);
		
		 PlayerID = mBlackboard->GetKeyID("Player");
		 PlayerLastSeenLocationID = mBlackboard->GetKeyID("PlayerLastSeenLocation");
		 BombLocationID = mBlackboard->GetKeyID("BombLocation");
		 BombHeardLocationID = mBlackboard->GetKeyID("BombHeardLocation");
		 PlayerHeardLocationID = mBlackboard->GetKeyID("PlayerHeardLocation");	
		 SecondaryLocationID = mBlackboard->GetKeyID("SecondaryLocation"); 
		 PlayerMovementDirectionID = mBlackboard->GetKeyID("PlayerMovementDirection");
		 PatrolLocationID = mBlackboard->GetKeyID("PatrolLocation");
		 PatrolIndexID = mBlackboard->GetKeyID("PatrolIndex");
		 AlertedID = mBlackboard->GetKeyID("Alerted");
		 isAliveID = mBlackboard->GetKeyID("Alive");

		mBlackboard->SetValue<UBlackboardKeyType_Bool>(isAliveID, mOwner->isAlive);
		mBehaviortree->StartTree(*mOwner->BehaviorTree);
	
	}
	sightConfig->SightRadius = VisionRangeMin;
	sightConfig->LoseSightRadius = VisionRangeMax;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	mPerceptionComponent->ConfigureSense(*sightConfig);
	
	soundConfig->HearingRange = HearRange;
	soundConfig->bUseLoSHearing = false;
	soundConfig->DetectionByAffiliation.bDetectEnemies = true;
	soundConfig->DetectionByAffiliation.bDetectNeutrals = true;
	soundConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	mPerceptionComponent->ConfigureSense(*soundConfig);

	mBlackboard->SetValue<UBlackboardKeyType_Vector>(PatrolLocationID, mOwner->GetActorLocation());
	mBlackboard->SetValue<UBlackboardKeyType_Int>(PatrolIndexID, 0);
	mBlackboard->SetValue<UBlackboardKeyType_Bool>(AlertedID, false);

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
		mBlackboard->SetValue<UBlackboardKeyType_Bool>(AlertedID, true);
		FActorPerceptionBlueprintInfo info;
		mPerceptionComponent->GetActorsPerception(Target, info);

		// Checking if detected Target is player
		if (Target->GetClass()->IsChildOf(Aprepro2Character::StaticClass()))
		{
			for (int i = 0; i < info.LastSensedStimuli.Num(); ++i)
			{
				// Checking if it was detected by sight
				if (info.LastSensedStimuli[i].Type.Name == "Default__AISense_Sight")
				{
					// Player entered the sight
					if (info.LastSensedStimuli[i].WasSuccessfullySensed() )
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Player Detected. Following him");
						mBlackboard->SetValue<UBlackboardKeyType_Object>(PlayerID, Target);									
					}
					// Player exited the sight
					else
					{
						if (info.LastSensedStimuli[i].GetAge() == 0)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player Lost. Updating its last seen Location");
							mBlackboard->ClearValue(PlayerID);					
							if (mBlackboard->IsVectorValueSet(PlayerHeardLocationID))
							{
								mBlackboard->ClearValue(PlayerHeardLocationID);
							}
							if (mBlackboard->IsVectorValueSet(BombHeardLocationID))
							{
								mBlackboard->ClearValue(BombHeardLocationID);
							}
						}
						mBlackboard->SetValue<UBlackboardKeyType_Vector>(PlayerLastSeenLocationID, Target->GetActorLocation());
						info.LastSensedStimuli[i].AgeStimulus(1.f);
					}
				}

				// Checking if it was detected by sound
				else if (info.LastSensedStimuli[i].Type.Name == "Default__AISense_Hearing" && info.LastSensedStimuli[i].GetAge() == 0.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Player Heard. Updating its last Heard Location");
					mBlackboard->SetValue<UBlackboardKeyType_Vector>(PlayerHeardLocationID, Target->GetActorLocation());
					info.LastSensedStimuli[i].AgeStimulus(1.f);
					if(mBlackboard->IsVectorValueSet(PlayerLastSeenLocationID))
					{
						mBlackboard->ClearValue(PlayerLastSeenLocationID);
					}
					if (mBlackboard->IsVectorValueSet(BombHeardLocationID))
					{
						mBlackboard->ClearValue(BombHeardLocationID);
					}
				}
			}
		}

		// Checking if Detected Target is Bomb
		else if (Target->GetClass()->IsChildOf(ADetonateBomb::StaticClass()))
		{
			for (int i = 0; i < info.LastSensedStimuli.Num(); ++i)
			{
				// Checking if it was detected by sight
				if (info.LastSensedStimuli[i].Type.Name == "Default__AISense_Sight")
				{
					mBlackboard->SetValue<UBlackboardKeyType_Vector>(BombLocationID, Target->GetActorLocation());
				}
				// Checking if it was detected by sound
				else if (info.LastSensedStimuli[i].Type.Name == "Default__AISense_Hearing")
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Bomb Heard. Updating its last Heard Location");
					if (mBlackboard->IsVectorValueSet(PlayerHeardLocationID))
					{
						mBlackboard->ClearValue(PlayerHeardLocationID);
					}
					if (mBlackboard->IsVectorValueSet(PlayerLastSeenLocationID))
					{
						mBlackboard->ClearValue(PlayerLastSeenLocationID);
					}
					mBlackboard->SetValue<UBlackboardKeyType_Vector>(BombHeardLocationID, Target->GetActorLocation());
				}
			}
		}
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
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Detected");

	for (AActor* actors : testActors)
	{
		FActorPerceptionBlueprintInfo info;
		mPerceptionComponent->GetActorsPerception(actors, info);

		for (int i = 0; i < info.LastSensedStimuli.Num(); ++i)
		{
			//info.LastSensedStimuli[i].SetStimulusAge(0.f);
		}

		if (actors->GetClass()->IsChildOf(ADetonateBomb::StaticClass()))
		{
			//Checking if Bomb is active
			ADetonateBomb* temp = (ADetonateBomb*)actors;
			if (!temp->IsPlanted())
			{
				return;
			}
		}
		SetTargetEnemy(Cast<APawn>(actors));
	}
}
void AEnemyController::UpdateStatus()
{
	if (mOwner)
	{
	mBlackboard->SetValue<UBlackboardKeyType_Bool>(isAliveID, mOwner->isAlive);
	}
}