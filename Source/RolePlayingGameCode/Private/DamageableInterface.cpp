// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableInterface.h"

// Add default functionality here for any IDamageableInterface functions that are not pure virtual.

float IDamageableInterface::CurHp()
{
	return 0.0f;
}

float IDamageableInterface::MaxHp()
{
	return 0.0f;
}

float IDamageableInterface::GetHeal(float HealAmount)
{
	return 0.0f;
}

float IDamageableInterface::TakeDamage(float DamageAmount)
{
	return 0.0f;
}
