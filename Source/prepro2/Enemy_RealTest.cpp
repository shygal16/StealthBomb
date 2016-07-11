// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"

#include "Engine.h"
#include "Enemy_RealTest.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

#include "DetonateBomb.h"
#include "prepro2Character.h"

#include "EnemyController.h"
//#include "Runtime/Engine/Classes/GameFramework/Controller.h"



// Sets default values
AEnemy_RealTest::AEnemy_RealTest()
{
	PrimaryActorTick.bCanEverTick = true;
//	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
//	PawnSense->SetPeripheralVisionAngle(90.f);
	//AIControllerClass = AEnemyController::StaticClass();
	
}

/*
void AEnemy_RealTest::OnSeePlayer(APawn* pawn)
{
	AEnemyController* AIControll = Cast<AEnemyController>(GetController());
	if (AIControll)
	{
		age = 0;
		cleared = false;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you using Pawn Sense!");
		AIControll->SetTargetEnemy(pawn);
	}
}
*/

// Called when the game starts or when spawned
void AEnemy_RealTest::BeginPlay()
{
	Super::BeginPlay();
	

	/*sightConfig->SightRadius = 3000.0f;
	sightConfig->LoseSightRadius = 3500.f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	perceptionComponent->ConfigureSense(*sightConfig);

	soundConfig->HearingRange = 4000000.0f;
	soundConfig->bUseLoSHearing = false;
	soundConfig->DetectionByAffiliation.bDetectEnemies = true;
	soundConfig->DetectionByAffiliation.bDetectNeutrals = true;
	soundConfig->DetectionByAffiliation.bDetectFriendlies = true;

	perceptionComponent->ConfigureSense(*soundConfig);*/


	
//	if (PawnSense)
//	{
//		PawnSense->OnSeePawn.AddDynamic(this, &AEnemy_RealTest::OnSeePlayer);
//	}
	
	
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), this);
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, sightConfig->GetSenseImplementation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, soundConfig->GetSenseImplementation(),this);

}
// Called every frame
void AEnemy_RealTest::Tick(float DeltaTime)
{
	//if (mHealth > 0)
	{
	Super::Tick(DeltaTime);
	}
	FRotator rot = FRotator(0, 1, 0);
	
	//SetActorRotation(GetActorRotation() + rot);
}

float AEnemy_RealTest::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	mHealth -= DamageAmount;
	FString message = TEXT("Enemy took Damage. Health Remaining") + FString::FromInt(static_cast<int>(mHealth));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, message);
	if (mHealth <= 0)
	{
		Alive = false;
		Destroy();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Enemy Has Died");
	}
	return DamageAmount;
}
// Called to bind functionality to input
void AEnemy_RealTest::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);


}

//void AEnemy_RealTest::SenseStuff(TArray<AActor*> testActors)
//{
//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Detected");
//
//	AEnemyController* AIControll = Cast<AEnemyController>(GetController());
//
//	for (AActor* actors : testActors)
//	{
//		UAIPerceptionComponent* perceptionComponent = GetPerceptionComponent();
//		FActorPerceptionBlueprintInfo info;
//		perceptionComponent->GetActorsPerception(actors, info);
//		
//		for (int i = 0; i < info.LastSensedStimuli.Num(); ++i)
//		{
//			//info.LastSensedStimuli[i].SetStimulusAge(0.f);
//		}
//
//		if (actors->GetClass()->IsChildOf(ADetonateBomb::StaticClass()))
//		{
//			//Checking if Bomb is active
//			ADetonateBomb* temp = (ADetonateBomb*)actors;
//			if (!temp->IsActive())
//			{
//				return;
//			}
//		}
//		AIControll->SetTargetEnemy(Cast<APawn>(actors));
//	}



	//TArray<AActor*> heardActors;
	//TArray<AActor*> seenActors;

	//perceptionComponent->GetPerceivedActors(UAISense_Hearing::StaticClass(), heardActors);
	//perceptionComponent->GetPerceivedActors(UAISense_Hearing::StaticClass(), seenActors);
	//
	//if (AIControll)
	//{
	//	bool targetSet;

	//	// Checking all the actors updated by sight stimulus
	//	for (int i = 0; i < seenActors.Num(); ++i)
	//	{

	//		FString message = TEXT("seen Actor ") + testActors[i]->GetName();
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	//		// Checking if detected actor is player
	//		if (testActors[i]->GetClass()->IsChildOf(Aprepro2Character::StaticClass()))
	//		{
	//			Aprepro2Character* temp = (Aprepro2Character*)testActors[i];

	//			FActorPerceptionBlueprintInfo info;
	//			perceptionComponent->GetActorsPerception(temp, info);

	//			// Checking if the player exited or entered the AI sight
	//			if (info.LastSensedStimuli.Num() > 0)
	//			{
	//				// Player entered the sight
	//				if (info.LastSensedStimuli[0].WasSuccessfullySensed())
	//				{
	//					AIControll->SetTargetEnemy(Cast<APawn>(testActors[i]));
	//					targetSet = true;
	//					FString newmessage = info.LastSensedStimuli[0].Type.Name.ToString();
	//					if (info.LastSensedStimuli[0].Type.Name == "Default__AISense_Sight")
	//					{
	//						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "YES");
	//					}
	//					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, newmessage);
	//					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, UAISense_Sight::StaticClass()->GetName());
	//				}
	//				// Player exited the sight
	//				else
	//				{

	//				}
	//			}
	//							
	//			
	//		}
	//	}



		//for (int i = 0; i < seenActors.Num(); ++i)
		//{

		//	if (testActors[i]->GetClass()->IsChildOf(Aprepro2Character::StaticClass()))
		//	{
		//		Aprepro2Character* temp = (Aprepro2Character*)testActors[i];
		//		if (!playerInSight)
		//		{
		//			AIControll->SetTargetEnemy(Cast<APawn>(testActors[i]));
		//			targetSet = true;
		//		}
		//	}
		//	else if (testActors[i]->GetClass()->IsChildOf(AExplosive::StaticClass()))
		//	{
		//		AExplosive* temp = (AExplosive*)testActors[i];
		//		if (!playerInSight)
		//		{
		//			AIControll->SetTargetEnemy(Cast<APawn>(testActors[i]));
		//			targetSet = true;
		//		}
		//	}
		//}

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
		
	//}
//}