//
//  ScoreUI.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 16.10.13.
//
//

#ifndef ___pGigTickets__ScoreUI__
#define ___pGigTickets__ScoreUI__


#include "cocos2d.h"

USING_NS_CC;

class GameScene;

class ScoreUI: public Layer {
public:
    ~ScoreUI();
    ScoreUI();
    // cocos2d stuff
    virtual bool init();
    CREATE_FUNC(ScoreUI);

    // callbacks
    void onSubmitBtnPressed(Object *sender);
    void onMissOutBtnPressed(Object *sender);
    
    void setScore(int score);
    
    void popUp();
    void popOut();
private:
    LayerColor *shadowLayer;
    Sprite *back;
    MenuItem *submitBtn;
    MenuItem *missOutItem;

    Sprite *topScoreSprite;
    LabelBMFont *scoreLabel;
public:
    GameScene *gameLayer;
};

#endif /* defined(___pGigTickets__ScoreUI__) */
