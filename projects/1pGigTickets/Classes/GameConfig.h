//
//  GameConfig.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 13.09.13.
//
//

#ifndef ___pGigTickets__GameConfig__
#define ___pGigTickets__GameConfig__

#include "cocos2d.h"

USING_NS_CC;

#define kAppVersion         1
#define kGameTimer          30
#define kQuizTimer          5
#define kQMultiplier        5
#define kBoostMultiplier    2

#define kBaseIconValue      50
#define kFourInRowIconValue 500
#define kNoteIconValue      1000

#define kInitialPlectrums   100;

class GameConfig {
private:
    GameConfig();
public:
    ~GameConfig();
    
    void load();
    void save();
    
    static GameConfig * sharedInstance();
private:
    static GameConfig *__sharedInstance;
public:
    int version;
    
    float gameTimer;
    float quizTimer;
    float qMultiplier;
    float boostMultiplier;
    
    float baseIconValue;
    float fourInRowIconValue;
    float noteIconValue;
    
    int currentPlectrums;
    
    // forgive me for this(
    Point iOSFieldDisplacement;
};

#endif /* defined(___pGigTickets__GameConfig__) */
