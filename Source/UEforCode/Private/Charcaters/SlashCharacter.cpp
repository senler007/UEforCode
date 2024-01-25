#include "Charcaters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components\AttributeComponent.h"
#include "Sword.h"
#include "Animation/AnimMontage.h"
#include "HUD\SlashHUD.h"
#include "SlashOverlay.h"
#include "Soul.h"
#include "Treasure.h"
#include "Bird.h"
#include "GameFramework/Actor.h"


ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCharacter::Jump()
{
	if (ActionState == EActionState::EAC_Unoccupied) {
		Super::Jump();
	}
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("EngageableTarget"));

	InitializeSlashOverlay();
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASlashCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
	PlayerInputComponent->BindAction(FName("ChangeToBird"), IE_Pressed, this, &ASlashCharacter::VKeyPressed);
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if(ActorAttribute&&ActorAttribute->GetHealthPercent()>0.f)
	ActionState = EActionState::EAC_HitReaction;
}

void ASlashCharacter::AddSouls(ASoul* Soul)
{
	if (ActorAttribute) {
		//UE_LOG(LogTemp, Warning, TEXT("ActorAttribute,%d",Soul->GetSoul()));
		UE_LOG(LogTemp, Warning, TEXT("ActorAttribute, %s"), *FString::Printf(TEXT("%d"), Soul->GetSoul()));
		ActorAttribute->AddSoul(Soul->GetSoul());
		SlashOverlay->SetSoul(ActorAttribute->GetSoul());
	}
}

void ASlashCharacter::AddGold(ATreasure* Gold)
{
	if (ActorAttribute) {
		ActorAttribute->AddGold(Gold->GetGold());
	}
}

void ASlashCharacter::SetOverlappingItem(AItems* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAC_Unoccupied) return;
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ASlashCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAC_Unoccupied) return;
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Right, Value);
}

void ASlashCharacter::Turn(float Value)
{
	if (Controller && Value != 0) {
		AddControllerYawInput(Value);
	}
}

void ASlashCharacter::LookUp(float Value)
{
	if (Controller && Value != 0) {
		AddControllerPitchInput(Value);
	}
}

void ASlashCharacter::EKeyPressed()
{
	ASword* OverlappingWeapon = Cast<ASword>(OverlappingItem);
	if (OverlappingWeapon) {
		EquipWeapon(OverlappingWeapon);
	}
	else {
		if (CanDisarm()) {
			DisArm();
		}
		else if (CanArm()) {
			Arm();
		}
	}
}

void ASlashCharacter::Attack()
{
	if (ActionState != EActionState::EAC_Unoccupied) return;
	if (CanAttack()) {
		PlayAttackMontage();
		ActionState = EActionState::EAC_Attacking;
	}
}

void ASlashCharacter::VKeyPressed() {
	FVector SpawnLocation = GetActorLocation();
	FRotator Rotator = GetActorRotation();
	SpawnLocation.Z += 100.f;
	ABird* MyBird = GetWorld()->SpawnActor<ABird>(BirdClass,SpawnLocation,Rotator);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->Possess(MyBird);
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance&&EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::EquipWeapon(ASword* Sword)
{
	Sword->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedSword = Sword;
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAC_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAC_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAC_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped &&
		EquipMontage;
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedSword) {
		EquippedSword->AttachMeshtoSocket(GetMesh(), FName("RightHandSocket"));
	}
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAC_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedSword;
}

void ASlashCharacter::DisArm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAC_Equipping;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAC_Equipping;
}

void ASlashCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAC_Dead;
	DisableMeshCollision();
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedSword) {
		EquippedSword->AttachMeshtoSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::FinishEquip()
{
	ActionState = EActionState::EAC_Unoccupied;
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAC_Unoccupied;
}

void ASlashCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD) {
			
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay&&ActorAttribute) {
				SlashOverlay->SetHealthBarPercent(ActorAttribute->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(ActorAttribute->GetStaminaPercent());
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSoul(0);
			}
		}
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && ActorAttribute) {
		SlashOverlay->SetHealthBarPercent(ActorAttribute->GetHealthPercent());
	}
}
