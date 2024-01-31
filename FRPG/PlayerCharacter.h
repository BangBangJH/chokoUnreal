// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FRPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CameraTimer = 0.0f;
	float CameraTimerWeightedValue = 0.0f;
	bool bCameraZoom = false; //Zoom 이벤트 bool
	bool TargetCameraIndex = 1; //Zoom 타겟 카메라 인덱스
	bool CurrentCameraIndex = 0; //Zoom 현재 카메라 인덱스

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CameraZoomIn();
	void CameraZoomOut();
	void CameraSmoothMove();

	void ReceiveCharacterData(FString StringData);
	void RC_Move();
	bool IsDoll = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	class UCameraComponent* PlayerCam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	class USpringArmComponent* PlayerCamSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector CameraZoomValue ; //위치아님, (스프링암 길이, 절대회전Y, 절대회전Z);
	FVector CameraValue; //위치아님, (스프링암 길이, 절대회전Y, 절대회전Z);

private:
	FVector Rc_Position = FVector::ZeroVector;
	FRotator Rc_Rotation = FRotator::ZeroRotator;
	FVector Rc_Velocity = FVector::ZeroVector;;

};
