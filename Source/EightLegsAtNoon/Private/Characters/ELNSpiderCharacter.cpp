#include "Characters/ELNSpiderCharacter.h"

#include "AI/ELNSpiderAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/ELNGameMode.h"

AELNSpiderCharacter::AELNSpiderCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AIControllerClass = AELNSpiderAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 420.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AELNSpiderCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float AELNSpiderCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	const float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (bIsDead || AppliedDamage <= 0.f)
	{
		return AppliedDamage;
	}

	Health = FMath::Max(0.f, Health - AppliedDamage);
	if (Health <= 0.f)
	{
		KillSpider(EventInstigator, DamageCauser);
	}

	return AppliedDamage;
}

void AELNSpiderCharacter::KillSpider(AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	Health = 0.f;

	if (AController* CurrentController = GetController())
	{
		CurrentController->StopMovement();
	}

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AELNGameMode* GameMode = GetWorld()->GetAuthGameMode<AELNGameMode>())
	{
		GameMode->NotifySpiderKilled(this);
	}

	OnSpiderDied(EventInstigator, DamageCauser);
}

void AELNSpiderCharacter::SetWalkSpeed(float NewWalkSpeed)
{
	WalkSpeed = FMath::Max(0.f, NewWalkSpeed);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AELNSpiderCharacter::SetVisualMesh(USkeletalMesh* NewMesh)
{
	if (!NewMesh)
	{
		return;
	}

	GetMesh()->SetSkeletalMesh(NewMesh);
}
