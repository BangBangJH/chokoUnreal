// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerCam = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCam"));
	PlayerCamSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));

	PlayerCamSpringArm->SetupAttachment(RootComponent);
	PlayerCamSpringArm->SetUsingAbsoluteRotation(true);
	PlayerCamSpringArm->bDoCollisionTest = false;

	PlayerCam->SetupAttachment(PlayerCamSpringArm);
	PlayerCam->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);



	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraValue = FVector(PlayerCamSpringArm->TargetArmLength, PlayerCamSpringArm->GetDesiredRotation().Pitch, PlayerCamSpringArm->GetDesiredRotation().Yaw);
	CameraZoomValue = CameraValue - FVector(350,-45,0);
	UE_LOG(LogTemp, Log, TEXT("Value = %s"), *CameraValue.ToString());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCameraZoom) //���콺 �� �̺�Ʈ�� true;
	{
		CameraSmoothMove();
	}
	if (IsDoll)
	{
		RC_Move();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &APlayerCharacter::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &APlayerCharacter::CameraZoomOut);
}


void APlayerCharacter::CameraZoomIn()
{
	//PlayerCamSpringArm->SetWorldRotation(FRotator(CameraZoomValue.Y, CameraZoomValue.Z, 0));
	//PlayerCamSpringArm->TargetArmLength = CameraZoomValue.X;
	if (!bCameraZoom)
	{
		TargetCameraIndex = 1;
		if (TargetCameraIndex != CurrentCameraIndex) //�̵���ų ī�޶� �ε����� ���� ī�޶� �ε����� �ٸ� ��쿡�� ī�޶� �̵�
		{
			bCameraZoom = true;
			CameraTimerWeightedValue = 0.0f;
		}

	}
}

void APlayerCharacter::CameraZoomOut()
{
	//PlayerCamSpringArm->SetWorldRotation(FRotator(CameraValue.Y, CameraValue.Z, 0));
	//PlayerCamSpringArm->TargetArmLength = CameraValue.X;
	if (!bCameraZoom)
	{
		TargetCameraIndex = 0;
		if (TargetCameraIndex != CurrentCameraIndex)
		{
			bCameraZoom = true;
			CameraTimerWeightedValue = 0.0f;
		}
	}
}

void APlayerCharacter::CameraSmoothMove()
{
	if (CameraTimerWeightedValue < 5.0f)
	{
		CameraTimerWeightedValue += 0.1f;
	}
	if (TargetCameraIndex) //ī�޶� �ε����� ���� ������ ����
	{
		CameraTimer += (0.02f / CameraTimerWeightedValue);
	}
	else
	{
		CameraTimer -= (0.02f / CameraTimerWeightedValue);
	}

	float SpringArmLength = UKismetMathLibrary::Lerp(CameraValue.X, CameraZoomValue.X, CameraTimer); // (A, B, ������) ������ = 0�̸� A, 1�̸� B
	float RotatorX = UKismetMathLibrary::Lerp(CameraValue.Y, CameraZoomValue.Y, CameraTimer);
	float RotatorY = UKismetMathLibrary::Lerp(CameraValue.Z, CameraZoomValue.Z, CameraTimer);

	PlayerCamSpringArm->SetWorldRotation(FRotator(RotatorX, RotatorY, 0));
	PlayerCamSpringArm->TargetArmLength = SpringArmLength;

	if (CameraTimer < 0) //�� ���� ������ �� Ǯ��, ���� ī�޶� �ε����� Ÿ�̸Ӱ� ����
	{
		bCameraZoom = false;
		CameraTimer = 0.0f;
		CurrentCameraIndex = 0;
	}
	else if (CameraTimer > 1) //�� ���� ������ �� Ǯ��, ���� ī�޶� �ε����� Ÿ�̸Ӱ� ����
	{
		bCameraZoom = false;
		CameraTimer = 1.0f;
		CurrentCameraIndex = 1;
	}
	
}

void APlayerCharacter::ReceiveCharacterData(FString StringData) // ������ �޾Ƽ� �� �� ����
{
	UE_LOG(LogTemp, Log, TEXT("Receive"));
	UE_LOG(LogTemp, Log, TEXT("Data: %s"), *StringData);
	TArray<FString> DataArray;
	StringData.ParseIntoArray(DataArray, TEXT(","));
	UE_LOG(LogTemp, Log, TEXT("VlcData: %s"), *DataArray[2]);
	Rc_Position.InitFromString(DataArray[0]);
	Rc_Rotation.InitFromString(DataArray[1]);
	Rc_Velocity.InitFromString(DataArray[2]);
}

void APlayerCharacter::RC_Move()
{
	this->AddMovementInput(Rc_Velocity);
}

