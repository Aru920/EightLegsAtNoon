#include "AI/ELNDwarfAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AELNDwarfAIController::AELNDwarfAIController()
{
	DwarfPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("DwarfPerception"));
	SetPerceptionComponent(*DwarfPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 900.f;
	SightConfig->LoseSightRadius = 1100.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DwarfPerceptionComponent->ConfigureSense(*SightConfig);
	DwarfPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}
