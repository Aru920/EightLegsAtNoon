#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ELNSpiderSpawner.generated.h"

class AELNSpiderCharacter;
class UBoxComponent;
class USceneComponent;
class USkeletalMesh;

UCLASS()
class EIGHTLEGSATNOON_API AELNSpiderSpawner : public AActor
{
	GENERATED_BODY()

public:
	AELNSpiderSpawner();

	UFUNCTION(BlueprintCallable, Category = "Spider Spawner")
	AELNSpiderCharacter* SpawnSpider(float SpiderSpeed = -1.f);

	UFUNCTION(BlueprintCallable, Category = "Spider Spawner")
	void SpawnSpiders(int32 SpawnCount, float SpiderSpeed = -1.f);

protected:
	USkeletalMesh* GetRandomSpiderMesh() const;
	FTransform GetSpawnTransform() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spider Spawner")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spider Spawner")
	TObjectPtr<UBoxComponent> SpawnAreaBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider Spawner")
	TSubclassOf<AELNSpiderCharacter> SpiderClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider Spawner")
	TArray<TObjectPtr<USkeletalMesh>> SpiderMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider Spawner")
	float SpawnHeightOffset = 20.f;
};
