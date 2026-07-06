#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ELNGameMode.generated.h"

class AELNSpiderSpawner;

UCLASS()
class EIGHTLEGSATNOON_API AELNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartNextWave();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void OnWaveStarted(int32 WaveNumber, int32 SpiderCount);

	int32 GetSpiderCountForWave() const;
	float GetSpiderSpeedForWave() const;
	void CacheSpiderSpawners();
	void SpawnNextSpiderInWave();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0"))
	int32 BaseSpiderCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0"))
	int32 ExtraSpidersPerWave = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float BaseSpiderSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float SpiderSpeedIncreasePerWave = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.0"))
	float WaveInterval = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.01"))
	float SpiderSpawnInterval = 3.f;

	UPROPERTY(BlueprintReadOnly, Category = "Waves")
	int32 CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	TArray<TObjectPtr<AELNSpiderSpawner>> SpiderSpawners;

	int32 PendingWaveSpawns = 0;
	float CurrentWaveSpiderSpeed = 0.f;

	FTimerHandle WaveTimerHandle;
	FTimerHandle SpiderSpawnTimerHandle;
};
