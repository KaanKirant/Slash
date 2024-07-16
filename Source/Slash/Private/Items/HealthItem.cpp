// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthItem.h"
#include "Interfaces/PickUpInterface.h"


void AHealthItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->AddHealth(this);

		SpawnPickUpSystem();
		SpawnPickUpSound();

		Destroy();
	}
}
