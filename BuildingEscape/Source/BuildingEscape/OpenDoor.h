// Andy Montoya 2017

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

// blueprint assignable creation (new class) - this is declaring a new delegate class called FOnOpenRequest
DECLARE_DYNAMIC_MULTICAST_DELEGATE (FDoorEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// assign the class as Blueprint Assignable
	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnCloseRequest;

private:

	//leaving things where it is in memory by using pointers
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 60.f;

	
	AActor* Owner = nullptr; // the owning door

	float GetTotalMassOfActorsOnPlate();
	
	
};
