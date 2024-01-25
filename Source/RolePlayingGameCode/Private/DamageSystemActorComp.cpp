// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSystemActorComp.h"

// Sets default values for this component's properties
UDamageSystemActorComp::UDamageSystemActorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageSystemActorComp::BeginPlay()
{
	Super::BeginPlay();

	//Event_Dele_TargetDamaged.AddDynamic(this, &UDamageSystemActorComp::CallDeleFunc_Single);	
}


// Called every frame
void UDamageSystemActorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UDamageSystemActorComp::Heal(float HealAmount)
{ 
	if (!IsDead)
	{
		CurHp = FMath::Clamp(CurHp + HealAmount, 0.0f, MaxHp);
	}
	return CurHp;
}

float UDamageSystemActorComp::TakeDamage(float DamageAmount)
{
	CurHp = FMath::Clamp(CurHp - DamageAmount, 0.0f, MaxHp);

	if (Event_Dele_TargetDamaged.IsBound())	// delegate(Event Dispatcher) 호출
		Event_Dele_TargetDamaged.Broadcast();

	if (CurHp <= 0)
	{
		IsDead = true;
		if (Event_Dele_OnDeath.IsBound())	// delegate(Event Dispatcher) 호출
			Event_Dele_OnDeath.Broadcast();
	}
	return DamageAmount;
}




