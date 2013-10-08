//
//  GameUI.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 08.10.13.
//
//

#ifndef ___pGigTickets__GameUI__
#define ___pGigTickets__GameUI__

#include "cocos2d.h"

USING_NS_CC;

class GameUI: public Layer {
public:
    ~GameUI();
    GameUI();
    // cocos2d stuff
    virtual bool init();
    CREATE_FUNC(GameUI);
    // callbacks
    void onPauseBtnPressed(Object *sender);
    void onBoostBtnPressed(Object *sender);
    
    void setBoostBtnEnabled(bool enabled);
    
    void setTime(int seconds);
    void setscore(int score);
    
    // setters
    void setBoost(float value);
    void setQuiz(float value);
    void setPlectrums(int value);
private:
    // stuff
    MenuItem *pauseBtn;
    Sprite *timeMount;
    Sprite *scoreMount;
    Sprite *plectrumsMount;
    
    MenuItem *boostBtn;
    
    Sprite *boostBack;
    Sprite *boostArrow;
    Sprite *boostTop;
    
    Sprite *quizBack;
    Sprite *quizArrow;
    Sprite *quizTop;
    
    LabelBMFont *timeLabel;
    LabelBMFont *scoreLabel;
    LabelBMFont *plectrumsLabel;
};

#endif /* defined(___pGigTickets__GameUI__) */
