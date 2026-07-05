#include "World/ELNDwarfSafeZone.h"

#include "Components/BoxComponent.h"

AELNDwarfSafeZone::AELNDwarfSafeZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneBox"));
	SetRootComponent(ZoneBox);

	ZoneBox->SetBoxExtent(FVector(600.f, 600.f, 120.f));
	ZoneBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZoneBox->SetHiddenInGame(false);
}
