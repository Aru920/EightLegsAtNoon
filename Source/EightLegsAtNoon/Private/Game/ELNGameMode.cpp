#include "Game/ELNGameMode.h"

#include "EngineUtils.h"
#include "TimerManager.h"
#include "World/ELNSpiderSpawner.h"

void AELNGameMode::BeginPlay()
{
	Super::BeginPlay();

	CacheSpiderSpawners();

	if (FirstWaveDelay > 0.f)
	{
		GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AELNGameMode::StartNextWave, FirstWaveDelay, false);
	}
	else
	{
		StartNextWave();
	}
}

void AELNGameMode::StartNextWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(SpiderSpawnTimerHandle);

	if (SpiderSpawners.Num() <= 0)
	{
		CacheSpiderSpawners();
	}

	if (SpiderSpawners.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spider spawners found."));
		return;
	}

	++CurrentWave;
	if (CurrentWave == 1)
	{
		CurrentWaveSpiderCount = FirstWaveSpiderCount;
	}
	else
	{
		const int32 MinIncrease = FMath::Min(MinSpidersAddedPerWave, MaxSpidersAddedPerWave);
		const int32 MaxIncrease = FMath::Max(MinSpidersAddedPerWave, MaxSpidersAddedPerWave);
		CurrentWaveSpiderCount += FMath::RandRange(MinIncrease, MaxIncrease);
	}

	PendingWaveSpawns = CurrentWaveSpiderCount;
	AliveSpidersInWave = 0;
	CurrentWaveSpiderSpeed = GetSpiderSpeedForWave();

	UE_LOG(LogTemp, Log, TEXT("Starting wave %d: %d spiders."), CurrentWave, CurrentWaveSpiderCount);

	OnWaveStarted(CurrentWave, CurrentWaveSpiderCount);

	if (PendingWaveSpawns <= 0)
	{
		TryFinishWave();
		return;
	}

	SpawnNextSpiderInWave();

	if (PendingWaveSpawns > 0)
	{
		GetWorldTimerManager().SetTimer(SpiderSpawnTimerHandle, this, &AELNGameMode::SpawnNextSpiderInWave, SpiderSpawnInterval, true);
	}
}

void AELNGameMode::NotifySpiderKilled(AELNSpiderCharacter* Spider)
{
	if (!Spider || AliveSpidersInWave <= 0)
	{
		return;
	}

	--AliveSpidersInWave;
	TryFinishWave();
}

float AELNGameMode::GetSpiderSpeedForWave() const
{
	return BaseSpiderSpeed + FMath::Max(0, CurrentWave - 1) * SpiderSpeedIncreasePerWave;
}

void AELNGameMode::CacheSpiderSpawners()
{
	SpiderSpawners.Reset();

	for (TActorIterator<AELNSpiderSpawner> It(GetWorld()); It; ++It)
	{
		SpiderSpawners.Add(*It);
	}
}

void AELNGameMode::SpawnNextSpiderInWave()
{
	if (PendingWaveSpawns <= 0)
	{
		return;
	}

	if (SpiderSpawners.Num() <= 0)
	{
		CacheSpiderSpawners();
	}

	if (SpiderSpawners.Num() <= 0)
	{
		PendingWaveSpawns = 0;
		return;
	}

	const int32 RandomSpawnerIndex = FMath::RandRange(0, SpiderSpawners.Num() - 1);
	if (AELNSpiderSpawner* SpiderSpawner = SpiderSpawners[RandomSpawnerIndex])
	{
		if (SpiderSpawner->SpawnSpider(CurrentWaveSpiderSpeed))
		{
			++AliveSpidersInWave;
		}
	}

	--PendingWaveSpawns;
	if (PendingWaveSpawns <= 0)
	{
		GetWorldTimerManager().ClearTimer(SpiderSpawnTimerHandle);
		TryFinishWave();
	}
}

void AELNGameMode::TryFinishWave()
{
	if (PendingWaveSpawns > 0 || AliveSpidersInWave > 0)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(SpiderSpawnTimerHandle);

	UE_LOG(LogTemp, Log, TEXT("Wave %d cleared."), CurrentWave);
	OnWaveCleared(CurrentWave);

	if (WaveInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AELNGameMode::StartNextWave, WaveInterval, false);
	}
	else
	{
		StartNextWave();
	}
}
