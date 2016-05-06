// Fill out your copyright notice in the Description page of Project Settings.

#include "prepro2.h"
#include "XrayMode.h"
#include "Engine.h"
//bool Globals::XrayOn;
// Sets default values for this component's properties
UXrayMode::UXrayMode()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}



// Called when the game starts
void UXrayMode::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
	mParentMesh=Cast<UPrimitiveComponent>(GetOwner()->GetComponents()[0]);
}


// Called every frame
void UXrayMode::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	
	

		if(Globals::XrayOn)//GetOwner()->GetVelocity().Size() == 0)
		{
			//if (GetOwner()->GetVelocity().Size() == 0)
			{
				mParentMesh->SetRenderCustomDepth(true);
				mParentMesh->CustomDepthStencilValue = 100;
			}
		}
		else
		{
			mParentMesh->SetRenderCustomDepth(false);
		}
	
	// ...
}

