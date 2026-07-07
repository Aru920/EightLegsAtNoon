#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ELNGameMode.generated.h"

class AELNSpiderCharacter;
class AELNSpiderSpawner;

UCLASS()
class EIGHTLEGSATNOON_API AELNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartNextWave();

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void NotifySpiderKilled(AELNSpiderCharacter* Spider);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void OnWaveStarted(int32 WaveNumber, int32 SpiderCount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void OnWaveCleared(int32 WaveNumber);

	float GetSpiderSpeedForWave() const;
	void CacheSpiderSpawners();
	void SpawnNextSpiderInWave();
	void TryFinishWave();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0"))
	int32 FirstWaveSpiderCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0"))
	int32 MinSpidersAddedPerWave = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0"))
	int32 MaxSpidersAddedPerWave = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float BaseSpiderSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float SpiderSpeedIncreasePerWave = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float WaveInterval = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float FirstWaveDelay = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.01"))
	float SpiderSpawnInterval = 3.f;

	UPROPERTY(BlueprintReadOnly, Category = "Waves")
	int32 CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Waves")
	int32 CurrentWaveSpiderCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Waves")
	int32 AliveSpidersInWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	TArray<TObjectPtr<AELNSpiderSpawner>> SpiderSpawners;

	int32 PendingWaveSpawns = 0;
	float CurrentWaveSpiderSpeed = 0.f;

	FTimerHandle WaveTimerHandle;
	FTimerHandle SpiderSpawnTimerHandle;
};
