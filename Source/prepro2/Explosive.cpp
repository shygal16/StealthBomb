// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "Explosive.h"
#include "XrayMode.h"

// Sets default values
AExplosive::AExplosive()
	:mRadForce(CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce")))
	, mParticleSystem(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem")))
	, mBombTriggered(false)
	, mExploded(false)
	, mExplosionDelay(5.0f)
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
		MakeNoise(1.f, this, GetActorLocation());
	}
}

void AExplosive::TriggerBomb()
{
	if(!mExploded && !mBombTriggered)
	{
		mBombTriggered = true;
	}
}

