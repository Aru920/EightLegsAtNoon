#include "World/ELNSpiderSpawner.h"

#include "Characters/ELNSpiderCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/SkeletalMesh.h"

AELNSpiderSpawner::AELNSpiderSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpawnAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnAreaBox"));
	SpawnAreaBox->SetupAttachment(SceneRoot);
	SpawnAreaBox->SetBoxExtent(FVector(1200.f, 800.f, 100.f));
	SpawnAreaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnAreaBox->SetHiddenInGame(true);
}

AELNSpiderCharacter* AELNSpiderSpawner::SpawnSpider(float SpiderSpeed)
{
	if (!SpiderClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no SpiderClass set."), *GetName());
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AELNSpiderCharacter* SpawnedSpider = GetWorld()->SpawnActor<AELNSpiderCharacter>(SpiderClass, GetSpawnTransform(), SpawnParams);
	if (!SpawnedSpider)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to spawn spider."), *GetName());
		return nullptr;
	}

	if (USkeletalMesh* SpiderMesh = GetRandomSpiderMesh())
	{
		SpawnedSpider->SetVisualMesh(SpiderMesh);
	}

	if (SpiderSpeed >= 0.f)
	{
		SpawnedSpider->SetWalkSpeed(SpiderSpeed);
	}

	return SpawnedSpider;
}

void AELNSpiderSpawner::SpawnSpiders(int32 SpawnCount, float SpiderSpeed)
{
	for (int32 Index = 0; Index < SpawnCount; ++Index)
	{
		SpawnSpider(SpiderSpeed);
	}
}

USkeletalMesh* AELNSpiderSpawner::GetRandomSpiderMesh() const
{
	if (SpiderMeshes.Num() <= 0)
	{
		return nullptr;
	}

	const int32 RandomIndex = FMath::RandRange(0, SpiderMeshes.Num() - 1);
	return SpiderMeshes[RandomIndex];
}

FTransform AELNSpiderSpawner::GetSpawnTransform() const
{
	const FVector Center = SpawnAreaBox->GetComponentLocation();
	const FVector Extent = SpawnAreaBox->GetScaledBoxExtent();

	float LocalX = 0.f;
	float LocalY = 0.f;

	switch (FMath::RandRange(0, 2))
	{
	case 0:
		LocalX = FMath::RandRange(-Extent.X, Extent.X);
		LocalY = Extent.Y;
		break;
	case 1:
		LocalX = -Extent.X;
		LocalY = FMath::RandRange(-Extent.Y * 0.25f, Extent.Y);
		break;
	default:
		LocalX = Extent.X;
		LocalY = FMath::RandRange(-Extent.Y * 0.25f, Extent.Y);
		break;
	}

	FVector SpawnLocation = Center;
	SpawnLocation += SpawnAreaBox->GetForwardVector() * LocalX;
	SpawnLocation += SpawnAreaBox->GetRightVector() * LocalY;
	SpawnLocation.Z += SpawnHeightOffset;

	FVector LookDirection = Center - SpawnLocation;
	LookDirection.Z = 0.f;

	const FRotator SpawnRotation = LookDirection.IsNearlyZero()
		? GetActorRotation()
		: LookDirection.Rotation();

	return FTransform(SpawnRotation, SpawnLocation);
}
