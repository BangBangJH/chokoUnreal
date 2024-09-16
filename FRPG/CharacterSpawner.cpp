// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawner.h"
#include "PlayerCharacter.h"

// Sets default values
ACharacterSpawner::ACharacterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnCharacter();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterSpawner::SendData, 1.0f, true, 5.0f);
}

// Called every frame
void ACharacterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterSpawner::SpawnCharacter() //스포너 좌표로 캐릭터 스폰
{
	if (TargetCharacter)
	{
		FVector Pos = GetActorLocation();
		DollCharacter = Cast<APlayerCharacter>(GetWorld()->SpawnActor(TargetCharacter, &Pos));
		DollCharacter->IsDoll = true;
	}
}

void ACharacterSpawner::SendData() //데이터 보내기
{
	if (DollCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Send"));
		FVector Pos = FVector::ZeroVector;
		FRotator Rot = FRotator::ZeroRotator;
		FVector Vlc = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), 0);
		FString CharacterData = (Pos.ToString() + "," + Rot.ToString() + "," + Vlc.ToString());
		DollCharacter->ReceiveCharacterData(CharacterData);
	}
}

