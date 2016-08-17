// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "DetonateBomb.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


// Sets default values
ADetonateBomb::ADetonateBomb()
	: mBombModel(CreateDefaultSubobject<UDestructibleComponent>(TEXT("Model")))
	, mBox(CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box")))
	, mIsPlanted(false)
	, mDisappearDelay(2.0f)
	, ExplosionAudioComp(CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion Audio Comp")))
	, TickAudioComp(CreateDefaultSubobject<UAudioComponent>(TEXT("Tick Audio Comp")))
{
	//RangeTelegraph = (CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp")));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = mBombModel;
	
	mParticleSystem->AttachTo(mBombModel);
	mRadForce->AttachTo(mBombModel);
	mBox->AttachTo(mBombModel);
	
	mDisappearTimer = mDisappearDelay;

	if (ExplosionAudioComp)
	{
		ExplosionAudioComp->AttachParent = RootComponent;
		ExplosionAudioComp->bAutoActivate = false; // with this true the sounds play at spawn (game starts)
		ExplosionAudioComp->bStopWhenOwnerDestroyed = false;
	}
	if (TickAudioComp)
	{	
		TickAudioComp->AttachParent = RootComponent;
		TickAudioComp->bAutoActivate = false; // with this true the sounds play at spawn (game starts)
		//TickAudioComp->bStopWhenOwnerDestroyed = false;
	}
}

// Called when the game starts or when spawned
void ADetonateBomb::BeginPlay()
{
	Super::BeginPlay();
	
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Hearing::StaticClass(), this);

	if (RangeTelegraph)
	{
		RangeTelegraph->SetBoxExtent(FVector(mExplosionRadius, mExplosionRadius, 1));
		RangeTelegraph->SetVisibility(true);
	}

	
}

// Called every frame
void ADetonateBomb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (mTriggered)
	{
		mExplosionDelay -= DeltaTime;
		mTickTimer -= DeltaTime;
		if (mTickTimer <= 0.f)
		{
			mTickTimer = 1;
			TickAudioComp->Activate(true);
			TickAudioComp->Play(0.0f);
			TickAudioComp->Activate(false);

		}
		if (mExplosionDelay < 0.f)
		{
			ExplosionAudioComp->Activate(true);
			ExplosionAudioComp->Play(0.0f);
			Explode();
		}
	}


	if (mExploded)
	{
		mDisappearTimer -= DeltaTime;

		if (mDisappearTimer < 0.f)
		{
			Destroy();
		}
	}
	
}

void ADetonateBomb::SetActive(bool active)
{
	
	
	mActive = active;

	SetActorHiddenInGame(!active);
	if (active)
	{
		mBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		mBombModel->GetBodyInstance()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		mBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mBombModel->GetBodyInstance()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SetActorTickEnabled(active);
	mBombModel->GetBodyInstance()->SetInstanceSimulatePhysics(active);
	

	UDestructibleComponent* meshComp = Cast<UDestructibleComponent>(GetComponentByClass(UDestructibleComponent::StaticClass()));
	if (meshComp != nullptr)
	{
		if (active)
		{
			UNavigationSystem::OnComponentRegistered(meshComp);
		}
		else
		{
			UNavigationSystem::OnComponentUnregistered(meshComp);
		}
	}
	
}
void ADetonateBomb::PingNoise()
{
	UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1, this, PulseRange);
}
void ADetonateBomb::XRayBomb(bool On)
{
	return;
	mBombModel->SetRenderCustomDepth(On);
}

bool ADetonateBomb::IsPlanted()
{
	return mIsPlanted; 
}