#include "Characters/ELNDwarfCharacter.h"

#include "AI/ELNDwarfAIController.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"

AELNDwarfCharacter::AELNDwarfCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AIControllerClass = AELNDwarfAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AELNDwarfCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentLives = MaxLives;
	bIsDead = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float AELNDwarfCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	if (bIsDead || DamageAmount <= 0.f)
	{
		return 0.f;
	}

	const int32 DamageToApply = FMath::Max(1, FMath::RoundToInt(DamageAmount));
	const int32 OldLives = CurrentLives;
	CurrentLives = FMath::Clamp(CurrentLives - DamageToApply, 0, MaxLives);

	const int32 DamageTaken = OldLives - CurrentLives;
	if (DamageTaken <= 0)
	{
		return 0.f;
	}

	UE_LOG(LogTemp, Log, TEXT("Dwarfy took %d damage. Lives: %d/%d"), DamageTaken, CurrentLives, MaxLives);
	if (bShowDwarfDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("Dwarfy hit: %d/%d lives"), CurrentLives, MaxLives));
	}

	OnDwarfDamaged(CurrentLives, MaxLives, DamageTaken, DamageCauser);

	if (CurrentLives <= 0)
	{
		bIsDead = true;
		GetCharacterMovement()->DisableMovement();
		UE_LOG(LogTemp, Warning, TEXT("Dwarfy died."));
		if (bShowDwarfDebug && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Dwarfy died"));
		}
		OnDwarfDied(DamageCauser);
	}

	return static_cast<float>(DamageTaken);
}
