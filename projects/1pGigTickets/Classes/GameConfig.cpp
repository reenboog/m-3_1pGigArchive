//
//  GameConfig.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 13.09.13.
//
//

#include "GameConfig.h"

GameConfig * GameConfig::__sharedInstance = nullptr;

GameConfig::~GameConfig() {
    
}

GameConfig::GameConfig() {
    
}

GameConfig * GameConfig::sharedInstance() {
    if(__sharedInstance == nullptr) {
        __sharedInstance = new GameConfig();
    }
    
    return __sharedInstance;
}

void GameConfig::load() {
    version = kAppVersion;
    
    gameTimer = kGameTimer;
    quizTimer = kQuizTimer;
    qMultiplier = kQMultiplier;
    boostMultiplier = kBoostMultiplier;
    
    baseIconValue = kBaseIconValue;
    fourInRowIconValue = kFourInRowIconValue;
    noteIconValue = kNoteIconValue;
    
    // load defaults
    currentPlectrums = kInitialPlectrums;
}

void GameConfig::save() {
    // use user defaults here
    
}