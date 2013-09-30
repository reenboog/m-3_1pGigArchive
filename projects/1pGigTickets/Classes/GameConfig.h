//
//  GameConfig.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 13.09.13.
//
//

#ifndef ___pGigTickets__GameConfig__
#define ___pGigTickets__GameConfig__

#define kAppVersion         1
#define kgameTimer          180
#define kQuizTimer          5
#define kQMultiplier        5

#define kBaseIconValue      50
#define kFourInRowIconValue 500
#define kNoteIconValue      100

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
    
    float baseIconValue;
    float fourInRowIconValue;
    float noteIconValue;
};

#endif /* defined(___pGigTickets__GameConfig__) */
