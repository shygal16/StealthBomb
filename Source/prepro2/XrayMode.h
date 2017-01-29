// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "XrayMode.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPRO2_API UXrayMode : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXrayMode();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//a primitive component is the holder of static meshes
	UPrimitiveComponent* mParentMesh;  	
private:
	//controls if the xray is on
	bool mXrayOn=false;
};
