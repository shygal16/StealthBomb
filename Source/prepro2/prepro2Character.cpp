// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "prepro2.h"
#include "prepro2Character.h"
#include "Engine.h"
#include "prepro2Projectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// Aprepro2Character

bool Globals::XrayOn;

Aprepro2Character::Aprepro2Character()
	: mBombsIndex(0)
	, mMaxBombs(5)
	, mBombSelected(-1)
{
	XrayOn = &Globals::XrayOn;
	SprintBar = SprintBarMax;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	sprintSpeed=WalkSpeed*SpeedMult;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);


	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	PrimaryActorTick.bCanEverTick = true;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Aprepro2Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Bomb", IE_Pressed, this, &Aprepro2Character::BombPlant);
	InputComponent->BindAction("Bomb", IE_Released, this, &Aprepro2Character::BombStopPlant);

	InputComponent->BindAction("TriggerAllBombs", IE_Pressed, this, &Aprepro2Character::TriggerAllBombs);
	InputComponent->BindAction("DetonateAllBombs", IE_Pressed, this, &Aprepro2Character::DetonateAllBombs);

	InputComponent->BindAction("TriggerBomb", IE_Pressed, this, &Aprepro2Character::TriggerBomb);
	InputComponent->BindAction("DetonateBomb", IE_Pressed, this, &Aprepro2Character::DetonateBomb);
	InputComponent->BindAction("SelectBomb", IE_Pressed, this, &Aprepro2Character::SelectBomb);
	
	InputComponent->BindAction("Sprint", IE_Pressed, this, &Aprepro2Character::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &Aprepro2Character::StopSprint);

	InputComponent->BindAction("Xray", IE_Pressed, this, &Aprepro2Character::ToggleXray);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &Aprepro2Character::StartCrouch);
    InputComponent->BindAction("Crouch", IE_Released, this, &Aprepro2Character::EndCrouch);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &Aprepro2Character::TouchStarted);
	if( EnableTouchscreenMovement(InputComponent) == false )
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &Aprepro2Character::OnFire);
	}
	
	InputComponent->BindAxis("MoveForward", this, &Aprepro2Character::MoveForward);

	InputComponent->BindAxis("MoveRight", this, &Aprepro2Character::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &Aprepro2Character::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &Aprepro2Character::LookUpAtRate);
}

void Aprepro2Character::OnFire()
{ 
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<Aprepro2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
			//mBombs[0]->SetActive(true);
			//mBombs[0]->SetActorLocation(SpawnLocation);
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}


}
void Aprepro2Character::Sprint()
{
	Sprinting = true;
	GetCharacterMovement()->MaxWalkSpeed=sprintSpeed;
}
void Aprepro2Character::StopSprint()
{
	Sprinting = false;
	GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;
}
void Aprepro2Character::StartCrouch()
{
	Crouch();
}
void Aprepro2Character::EndCrouch()
{
	UnCrouch();
}
void Aprepro2Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if( TouchItem.bIsPressed == true )
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void Aprepro2Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if( ( FingerIndex == TouchItem.FingerIndex ) && (TouchItem.bMoved == false) )
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void Aprepro2Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && ( TouchItem.FingerIndex==FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D( MoveDelta.X, MoveDelta.Y) / ScreenSize;									
					if (ScaledDelta.X != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (ScaledDelta.Y != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y* BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void Aprepro2Character::MoveForward(float Value)
{
	if (Value != 0.0f && !PlantingBomb)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		if (GetCharacterMovement()->MaxWalkSpeed == sprintSpeed)
		{
			MakeNoise(1.0f, this, GetActorLocation());
		}
	}
}

void Aprepro2Character::MoveRight(float Value)
{
	if (Value != 0.0f && !PlantingBomb)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		if (GetCharacterMovement()->MaxWalkSpeed == sprintSpeed)
		{
			MakeNoise(1.0f, this, GetActorLocation());
		}
	}
}

void Aprepro2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	float ModifiedTurn = PlantingBomb ? 0 : BaseTurnRate;
	AddControllerYawInput(Rate * ModifiedTurn * GetWorld()->GetDeltaSeconds());
	
}

void Aprepro2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	float ModifiedTurn = PlantingBomb ? 0 : BaseTurnRate;
		AddControllerPitchInput(Rate * ModifiedTurn * GetWorld()->GetDeltaSeconds());
	
}

bool Aprepro2Character::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if(FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch )
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &Aprepro2Character::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &Aprepro2Character::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &Aprepro2Character::TouchUpdate);
	}
	return bResult;
}
void Aprepro2Character::ToggleXray()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, XrayOn ? "True" : "False");
	if (*XrayOn )
	{
		*XrayOn = false;
	}
	else
	{
		*XrayOn = true;
	}


}
void Aprepro2Character::Bomb()
{
	if (mBombsIndex != mMaxBombs)
	{
		int curr = mBombsIndex++;
		mBombs[curr]->SetActive(true);
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
		mBombs[curr]->SetActorLocation(SpawnLocation);

		mBombSelected = curr;
	}
}

void Aprepro2Character::BombPlant()
{
	PlantingBomb = true;
	StartCrouch();

}
void Aprepro2Character::BombStopPlant()
{
	PlantingBomb = false;
	PlantProgress = 0;
	EndCrouch();
}

void Aprepro2Character::InitBombs()
{
	mBombs = new ADetonateBomb* [mMaxBombs];
	const FVector tempLocation = GetActorLocation();
	const FRotator tempRotation = { 0, 0, 0 };
	verify(BombClass != NULL && "bomb class doesnt exist");
	for (int i = 0; i < mMaxBombs; i++)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			
			// spawn the projectile at the muzzle
			mBombs[i] = World->SpawnActor<ADetonateBomb>(BombClass, tempLocation, tempRotation);
			
			mBombs[i]->SetActive(false);
	
		}
	}
}

void Aprepro2Character::BeginPlay()
{
	Super::BeginPlay();
	VisionBar = VisionBarMax;
	InitBombs();
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISenseConfig_Sight::GetSenseImplementation(),)
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(),this);
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Hearing::StaticClass(), this);

}

// Called every frame
void Aprepro2Character::Tick(float DeltaTime)
{

	//*XrayOn = Globals::XrayOn;

	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::FromInt(VisionBar));
	if (VisionBar < VisionBarMax && !*XrayOn)
	{
		VisionBar+=DeltaTime;
	}
	else if (*XrayOn)
	{
		VisionBar-=DeltaTime;
		if (VisionBar <= 0)
		{
			ToggleXray();
		}
	}

	if (!Sprinting && SprintBar<SprintBarMax)
	{
		SprintBar += DeltaTime;
	}
	else if(Sprinting)
	{
		SprintBar -= DeltaTime;
		if (SprintBar <= 0)
		{
			StopSprint();
		}
	}

	if (PlantingBomb)
	{
		PlantProgress += DeltaTime;
		if (PlantProgress >= PlantTime)
		{
			Bomb();
			PlantingBomb = false;
			EndCrouch();
			PlantProgress = 0;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::FromInt(VisionBar));

	for (int i = mBombsIndex - 1; i >= 0; --i)
	{
		if (!mBombs[i]->IsActive())
		{
			mBombsIndex--;
			ADetonateBomb* temp = mBombs[i];
			for (int j = i; j < mBombsIndex; ++j)
			{
				mBombs[j] = mBombs[j + 1];
			}
			mBombs[mBombsIndex] = temp;
		}
	}
}

void Aprepro2Character::TriggerAllBombs()
{
	for (int i = 0; i < mBombsIndex; ++i)
	{
		
			mBombs[i]->TriggerBomb();
		
	}
}
void Aprepro2Character::DetonateAllBombs()
{
	for (int i = 0; i < mBombsIndex; ++i)
	{
			mBombs[i]->Explode();
	}
}

void Aprepro2Character::TriggerBomb()
{
	if (mBombSelected != -1)
	{
		mBombs[mBombSelected]->TriggerBomb();
		//mBombSelected = (mBombSelected + 1 == mBombsIndex) ? 0 : mBombSelected + 1;
	}
}
void Aprepro2Character::DetonateBomb()
{
	if (mBombSelected != -1)
	{
		mBombs[mBombSelected]->Explode();
		SelectBomb();		
	}
}

void Aprepro2Character::SelectBomb()
{
	if (mBombsIndex != 0)
	{
		mBombs[mBombSelected]->XRayBomb(false);
			mBombSelected = (mBombSelected + 1 == mBombsIndex) ? 0 : mBombSelected + 1;
		mBombs[mBombSelected]->XRayBomb(true);
	}
}