#include "Game/ELNGameMode.h"

#include "EngineUtils.h"
#include "TimerManager.h"
#include "World/ELNSpiderSpawner.h"

void AELNGameMode::BeginPlay()
{
	Super::BeginPlay();

	CacheSpiderSpawners();
	StartNextWave();
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

	const int32 SpiderCountPerSpawner = GetSpiderCountForWave();
	PendingWaveSpawns = SpiderCountPerSpawner * SpiderSpawners.Num();
	CurrentWaveSpiderSpeed = GetSpiderSpeedForWave();

	UE_LOG(LogTemp, Log, TEXT("Starting wave %d: %d spiders per spawner across %d spawners."), CurrentWave, SpiderCountPerSpawner, SpiderSpawners.Num());

	OnWaveStarted(CurrentWave, PendingWaveSpawns);

	SpawnNextSpiderInWave();

	if (PendingWaveSpawns > 0)
	{
		GetWorldTimerManager().SetTimer(SpiderSpawnTimerHandle, this, &AELNGameMode::SpawnNextSpiderInWave, SpiderSpawnInterval, true);
	}
}

int32 AELNGameMode::GetSpiderCountForWave() const
{
	return BaseSpiderCount + FMath::Max(0, CurrentWave - 1) * ExtraSpidersPerWave;
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
		SpiderSpawner->SpawnSpider(CurrentWaveSpiderSpeed);
	}

	--PendingWaveSpawns;
	if (PendingWaveSpawns <= 0)
	{
		GetWorldTimerManager().ClearTimer(SpiderSpawnTimerHandle);

		if (WaveInterval > 0.f)
		{
			GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AELNGameMode::StartNextWave, WaveInterval, false);
		}
	}
}
