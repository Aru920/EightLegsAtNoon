#include "Characters/ELNSpiderCharacter.h"

#include "AI/ELNSpiderAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/ELNGameMode.h"
#include "Kismet/GameplayStatics.h"

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
	bIsAttacking = false;
	bHasAppliedAttackDamage = false;
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

void AELNSpiderCharacter::StartAttack(AActor* AttackTarget, float DamageAmount, AController* EventInstigator)
{
	if (bIsDead || bIsAttacking || !AttackTarget || DamageAmount <= 0.f)
	{
		return;
	}

	bIsAttacking = true;
	bHasAppliedAttackDamage = false;
	CurrentAttackTarget = AttackTarget;
	PendingAttackDamage = DamageAmount;
	PendingAttackInstigator = EventInstigator;

	if (AController* CurrentController = GetController())
	{
		CurrentController->StopMovement();
	}

	GetCharacterMovement()->StopMovementImmediately();
	OnSpiderAttackStarted(AttackTarget);
}

bool AELNSpiderCharacter::ApplyAttackDamage()
{
	if (bIsDead || !bIsAttacking || bHasAppliedAttackDamage || !CurrentAttackTarget)
	{
		return false;
	}

	bHasAppliedAttackDamage = true;
	UGameplayStatics::ApplyDamage(CurrentAttackTarget, PendingAttackDamage, PendingAttackInstigator, this, nullptr);
	return true;
}

void AELNSpiderCharacter::FinishAttack(bool bApplyDamage)
{
	if (bIsDead || !bIsAttacking)
	{
		return;
	}

	if (bApplyDamage)
	{
		ApplyAttackDamage();
	}

	bIsAttacking = false;
	CurrentAttackTarget = nullptr;
	PendingAttackInstigator = nullptr;
	PendingAttackDamage = 0.f;
	bHasAppliedAttackDamage = false;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AELNSpiderCharacter::KillSpider(AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	bIsAttacking = false;
	CurrentAttackTarget = nullptr;
	PendingAttackInstigator = nullptr;
	PendingAttackDamage = 0.f;
	bHasAppliedAttackDamage = false;
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

void AELNSpiderCharacter::OnSpiderAttackStarted_Implementation(AActor* AttackTarget)
{
	FinishAttack(true);
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
