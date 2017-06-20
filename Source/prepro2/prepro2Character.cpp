
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
//local declaration of the global tracker of xray mode
bool Globals::XrayOn;


Aprepro2Character::Aprepro2Character()
	: mInsideTriggerBox(false)
	, FootStepAudio(CreateDefaultSubobject<UAudioComponent>(TEXT("Footstep Audio Comp")))
	//, FootStepSound(CreateDefaultSubobject<USoundCue>(TEXT("Footstep sound Comp")))
	, Light(CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight Comp")))
{
	
	XrayOn = &Globals::XrayOn;
	FlashLightOn = false;
	SprintBar = SprintBarMax;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GamePaused = false;
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	sprintSpeed=WalkSpeed*SpeedMult;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);

	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

//	Target = CreateDefaultSubobject<ALightDetector>(TEXT("Target"));
	//Target->SetOnlyOwnerSee(false);
	//Target->AttachParent = FirstPersonCameraComponent;
	//Target->bCastDynamicShadow = false;
	//Target->CastShadow = false;
	

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;



	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	PrimaryActorTick.bCanEverTick = true;
	
	
	
	if (Light)
	{
		
		Light->ToggleVisibility();
	}
	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input


void Aprepro2Character::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	// set up gameplay key bindings
	check(inputComponent);

	inputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	inputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);



	inputComponent->BindAction("PauseGame", IE_Pressed, this, &Aprepro2Character::TogglePause);//.bExecuteWhenPaused = true;

	inputComponent->BindAction("Sprint", IE_Pressed, this, &Aprepro2Character::Sprint);
	inputComponent->BindAction("Sprint", IE_Released, this, &Aprepro2Character::StopSprint);

	inputComponent->BindAction("Xray", IE_Pressed, this, &Aprepro2Character::ToggleXray);
	//InputComponent->BindAction("Xray", IE_Released, this, &Aprepro2Character::TurnXrayOff);


	inputComponent->BindAction("Crouch", IE_Pressed, this, &Aprepro2Character::StartCrouch);
	inputComponent->BindAction("Crouch", IE_Released, this, &Aprepro2Character::EndCrouch);

	inputComponent->BindAction("FlashLight", IE_Pressed, this, &Aprepro2Character::TurnFlashLightOn);
	inputComponent->BindAction("FlashLight", IE_Released, this, &Aprepro2Character::TurnFlashLightOff);



	
	inputComponent->BindAxis("MoveForward", this, &Aprepro2Character::MoveForward);

	inputComponent->BindAxis("MoveRight", this, &Aprepro2Character::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	inputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	inputComponent->BindAxis("TurnRate", this, &Aprepro2Character::TurnAtRate);
	inputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	inputComponent->BindAxis("LookUpRate", this, &Aprepro2Character::LookUpAtRate);
}


void Aprepro2Character::TogglePause()
{
	//pauses the game and pins the pause screen widget to the screen
	GamePaused = true;//GamePaused ? false : true;
	UGameplayStatics::SetGamePaused(GetWorld(), GamePaused);
	UUserWidget* PauseWidget = CreateWidget<UUserWidget>(GetWorld(),PauseWidgetClass);
	PauseWidget->AddToViewport();

	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = GamePaused;
	MyController->bEnableClickEvents = GamePaused;
	MyController->bEnableMouseOverEvents = GamePaused;
	
}
void Aprepro2Character::GameOver()
{
	//pauses the game and pins the Game Over screen widget to the widget
	GamePaused = true;//GamePaused ? false : true;
	UGameplayStatics::SetGamePaused(GetWorld(), GamePaused);
	UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverClass);
	GameOverWidget->AddToViewport();

	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = GamePaused;
	MyController->bEnableClickEvents = GamePaused;
	MyController->bEnableMouseOverEvents = GamePaused;
	
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
{//used to action map crouching
	Crouch();
}
void Aprepro2Character::EndCrouch()
{//used to action map stop crouching
	UnCrouch();
}


float Aprepro2Character::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	//When you take damage from enemy dealing damage
	mHealth -= DamageAmount;
	FString message= TEXT("Player took Damage. Remaing HP: ")+ FString::FromInt(static_cast<int>(mHealth));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, message);
	if (mHealth <= 0)
	{
		GameOver();
	}
	return DamageAmount;
}


void Aprepro2Character::PlayFootStep()
{//shakes the camera for for headbob of a footstep
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShaker, 1);
}

void Aprepro2Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		
		// add movement in that direction
		PlayFootStep();
		AddMovementInput(GetActorForwardVector(), Value);
		
		float SoundMultiplier = GetCharacterMovement()->MaxWalkSpeed == sprintSpeed ? 1.0f : .2f;
		if (bIsCrouched)
		{
			SoundMultiplier = 0.05f;
			//return;
		}

		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), SoundMultiplier, this, 2000.f);
	}
}

void Aprepro2Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		PlayFootStep();
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);

		float SoundMultiplier = GetCharacterMovement()->MaxWalkSpeed == sprintSpeed ? 1.0f : .2f;
		if (bIsCrouched)
		{
			SoundMultiplier = 0.05f;
		//	return;
		}

		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), SoundMultiplier, this, 2000.f);
	}
}

void Aprepro2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	
}

void Aprepro2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	
}



void Aprepro2Character::TurnFlashLightOn()
{	//clicks the flash light on
	Light->ToggleVisibility();
	Target->SetActive(Light->IsVisible());
	UGameplayStatics::PlaySound2D(GetWorld(), LightOn);
	FlashLightOn = true;
}
void Aprepro2Character::TurnFlashLightOff()
{	//clicks the flash light off
	Light->ToggleVisibility();
	Target->SetActive(Light->IsVisible());
	UGameplayStatics::PlaySound2D(GetWorld(), LightOff);
	FlashLightOn = false;
}

void Aprepro2Character::ToggleXray()
{
	//if able to use xray, flips on and off, and changes the screen tint color
	if (UseXray)
	{
		*XrayOn = !*XrayOn;
		FLinearColor Tint = *XrayOn ? FColor::Black : FLinearColor::White;
		
		FirstPersonCameraComponent->PostProcessSettings.SceneColorTint.R = Tint.R;
		FirstPersonCameraComponent->PostProcessSettings.SceneColorTint.G = Tint.G;
		FirstPersonCameraComponent->PostProcessSettings.SceneColorTint.B = Tint.B;
		FirstPersonCameraComponent->PostProcessSettings.SceneColorTint.A=Tint.A;
		
	}
	
}



void Aprepro2Character::BeginPlay()
{
	Super::BeginPlay();
	mProgressBars = CreateWidget<UProgressBarWidget>(GetWorld(), mProgressBarsClass);
	mProgressBars->AddToViewport(0);
	UWorld* const World = GetWorld();

	const FVector tempLocation = GetActorLocation();
	const FRotator tempRotation = { 0, 0, 0 };

	VisionBar = VisionBarMax/2;
	Target = World->SpawnActor<ALightDetector>(LightDetectionClass, tempLocation, tempRotation);
	Target->SetActive(false);
	//UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISenseConfig_Sight::GetSenseImplementation(),)
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(),this);
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Hearing::StaticClass(), this);
	*XrayOn = false;
	FlashLightOn = false;

	FirstPersonCameraComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Mesh1P->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));

	FootStepAudio->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	Light->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

}


// Called every frame
void Aprepro2Character::Tick(float DeltaTime)
{
	 // Xray Regeneration
	if (VisionBar < VisionBarMax && !*XrayOn)
	{
		VisionBar+= XrayRegen*DeltaTime;
	}
	else if (*XrayOn)
	{
		VisionBar-=DeltaTime;
		if (VisionBar <= 0)
		{
			//TurnXrayOff();
			if (*XrayOn == true)
				ToggleXray();
		}
	}

	//sprint regeneration
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
	//updates the energy bar
	mProgressBars->mXrayPercentage = VisionBar / VisionBarMax;
	
	//footstep noises
	if (bIsCrouched)
	{
		FootStepTimer -= DeltaTime*0.75;
	}
	else
	FootStepTimer -= Sprinting ? DeltaTime * 2 : DeltaTime;
	//plays footstep sound
	if (FootStepTimer <= 0)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
		FootStepNoise();
		}
	}
	//casts a target at the end of the flashlight to attract the monsters attention
	if (FlashLightOn)
	{
		FVector CamLoc;
		FRotator CamRot;

		Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
		const FVector StartTrace = CamLoc; // trace start is the camera location
		const FVector Direction = CamRot.Vector();
		const FVector EndTrace = StartTrace + Direction * RayCastDistance;
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit(ForceInit);
		float length = RayCastDistance;
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Camera, TraceParams); // simple trace function
		if (Hit.bBlockingHit)
		{
			Target->SetActorLocation(Hit.Location);
			length = FVector::Dist(Hit.Location, StartTrace);
		}
		else
		{
			Target->SetActorLocation(EndTrace);
		}
		//DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 100.f, 20, FColor::Red);
	}
}

void Aprepro2Character::FootStepNoise()
{
	//playing the actual noise for the footsteps
		if (GetVelocity().Size() > 0)//is moving
		{
			float volume = Sprinting ? 2 : 1;
			volume = bIsCrouched ? 0.5f:volume;
			FootStepSound->VolumeMultiplier = volume;

			FootstepPitch= FootstepPitch>1 ? FootstepPitch - 0.25f : FootstepPitch + 0.25f;
			FootStepSound->PitchMultiplier = FootstepPitch;
			UGameplayStatics::PlaySound2D(GetWorld(), FootStepSound);
			
			
			FootStepTimer = footStepDelay;
		}
}


void Aprepro2Character::PickUpVisionBoost(float boost)
{//Picking up a xray bar battery
	VisionBar = (VisionBar + boost > VisionBarMax) ? VisionBarMax : VisionBar + boost;
}

void Aprepro2Character::GameExit()
{
	GamePaused = true;//GamePaused ? false : true;
	UGameplayStatics::SetGamePaused(GetWorld(), GamePaused);
	UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameWinClass);
	GameOverWidget->AddToViewport();

	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = GamePaused;
	MyController->bEnableClickEvents = GamePaused;
	MyController->bEnableMouseOverEvents = GamePaused;
}