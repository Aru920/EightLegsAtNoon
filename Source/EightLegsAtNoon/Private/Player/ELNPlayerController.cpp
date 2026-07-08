#include "Player/ELNPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AELNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGameplayInputMode();
}

void AELNPlayerController::SetGameplayInputMode()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AELNPlayerController::SetMenuInputMode()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AELNPlayerController::PauseGame()
{
	if (IsGamePaused())
	{
		return;
	}

	UGameplayStatics::SetGamePaused(this, true);
	SetMenuInputMode();
	OnPauseStateChanged(true);
}

void AELNPlayerController::ResumeGame()
{
	if (!IsGamePaused())
	{
		return;
	}

	UGameplayStatics::SetGamePaused(this, false);
	SetGameplayInputMode();
	OnPauseStateChanged(false);
}

void AELNPlayerController::TogglePause()
{
	if (IsGamePaused())
	{
		ResumeGame();
	}
	else
	{
		PauseGame();
	}
}

void AELNPlayerController::RestartCurrentLevel()
{
	UGameplayStatics::SetGamePaused(this, false);

	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName), false);
}

void AELNPlayerController::ReturnToMainMenu()
{
	UGameplayStatics::SetGamePaused(this, false);

	if (!MainMenuLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName, false);
	}
}

void AELNPlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

bool AELNPlayerController::IsGamePaused() const
{
	return UGameplayStatics::IsGamePaused(this);
}
