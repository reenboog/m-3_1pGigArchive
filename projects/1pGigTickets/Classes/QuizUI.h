//
//  File.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 16.10.13.
//
//

#ifndef ___pGigTickets__File__
#define ___pGigTickets__File__

#include "cocos2d.h"

USING_NS_CC;

class GameScene;

class QuizUI: public Layer {
public:
    ~QuizUI();
    QuizUI();
    // cocos2d stuff
    virtual bool init();
    CREATE_FUNC(QuizUI);
    
    // callbacks
    
    void onTimeOut();
    void update(float dt);
    
    void popUp();
    void popOut();
private:
    LayerColor *shadowLayer;
    Sprite *back;
    Sprite *timeProgressMount;
    ProgressTimer *timeProgress;
    
    float currentTime;
public:
    GameScene *gameLayer;
};


#endif /* defined(___pGigTickets__File__) */
