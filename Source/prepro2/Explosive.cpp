// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Explosive.h"
#include "Perception/AISense_Hearing.h"
#include "XrayMode.h"

// Sets default values
AExplosive::AExplosive()
	:mRadForce(CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce")))
	, mParticleSystem(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem")))
	, mBombTriggered(false)
	, mExploded(false)
	, mExplosionDelay(5.0f)
	,mExplosionRadius (200)
	//,RangeTelegraph(CreateDefaultSubobject<UBoxComponent>(TEXT("RangeTelegraph")))
{
	mParticleSystem->bAutoActivate = false;
	mRadForce->bAutoActivate = false;
	mExplosionTimer = mExplosionDelay;
}


void AExplosive::Explode()
{
	if(!mExploded && !mBombTriggered)
	{
		mParticleSystem->ActivateSystem();
		mRadForce->FireImpulse();
		mExploded = true;
		mExplosionTimer = mExplosionDelay;
		mBombTriggered = false;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 200, GetActorLocation(), mExplosionRadius, UDamageType::StaticClass(), TArray<AActor*>());
		MakeNoise(1.f, this, GetActorLocation(),800.0f);
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1, this, 4000.f);
	}
}

void AExplosive::TriggerBomb()
{
	if(!mExploded && !mBombTriggered)
	{
		mBombTriggered = true;
	}
}

