#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ELNPlayerController.generated.h"

UCLASS()
class EIGHTLEGSATNOON_API AELNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetGameplayInputMode();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetMenuInputMode();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void TogglePause();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartCurrentLevel();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void QuitGame();

	UFUNCTION(BlueprintPure, Category = "Pause")
	bool IsGamePaused() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause")
	void OnPauseStateChanged(bool bIsPaused);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	FName MainMenuLevelName = "MainMenu";
};
