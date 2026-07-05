#include "Animation/ELNDwarfAnimInstance.h"

#include "GameFramework/Pawn.h"

void UELNDwarfAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn)
	{
		Speed = 0.f;
		Direction = 0.f;
		bIsMoving = false;
		return;
	}

	const FVector Velocity = OwningPawn->GetVelocity();
	const FVector HorizontalVelocity(Velocity.X, Velocity.Y, 0.f);

	Speed = HorizontalVelocity.Size();
	bIsMoving = Speed > 3.f;

	if (bIsMoving)
	{
		const FVector LocalVelocity = OwningPawn->GetActorRotation().UnrotateVector(HorizontalVelocity);
		Direction = FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));
	}
	else
	{
		Direction = 0.f;
	}
}
