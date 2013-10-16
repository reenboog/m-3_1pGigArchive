//
//  AdScreen.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 16.10.13.
//
//

#ifndef ___pGigTickets__AdScreen__
#define ___pGigTickets__AdScreen__


#include "cocos2d.h"

USING_NS_CC;

class TicketScreen;

class AdScreen: public Layer {
public:
    ~AdScreen();
    AdScreen();
    // cocos2d stuff
    virtual bool init();
    static cocos2d::Scene * scene();
    CREATE_FUNC(AdScreen);
    
    void onContinueBtnPressed(Object *sender);
private:
    MenuItem *continueBtn;
    Sprite *back;
    LayerColor *purpleShadowLayer;
};


#endif /* defined(___pGigTickets__AdScreen__) */
