// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player_Stat.generated.h"

/**
 * 
 */
class FRPG_API Player_Stat
{
public:
	Player_Stat();
	~Player_Stat();
};

USTRUCT(Atomic)
struct FPlayer_Stat
{
	GENERATED_USTRUCT_BODY();
public:
	float Stat_MaxHP;
	float Stat_HP;
	float Stat_MaxMP;
	float Stat_MP;

	float Stat_ATK;
	float Stat_DEF;
	float Stat_Critical;

	float Stat_MaxEXP;
	float Stat_EXP;
	int Level;

};