// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameNetworkManager.generated.h"

/**
 * 
 */
UCLASS()
class AMyGameNetworkManager : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AMyGameNetworkManager();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    FString ServerIP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network")
    int32 ServerPort;

    // Destructor
    virtual ~AMyGameNetworkManager();

    // Function to connect to the server
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ConnectToServer();

    // Function to send data
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SendData(const FString& DataToSend);

    // Function to close the connection
    UFUNCTION(BlueprintCallable, Category = "Network")
    void CloseConnection();

private:
    // Client socket
    FSocket* ClientSocket;
};
