//
//  AdScreen.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 16.10.13.
//
//

#include "AdScreen.h"
#include "TicketScreen.h"

Scene* AdScreen::scene() {
    Scene *scene = Scene::create();
    
    AdScreen *layer = AdScreen::create();
    
    scene->addChild(layer);
    
    return scene;
}

AdScreen::~AdScreen() {
    
}

AdScreen::AdScreen() {
    continueBtn = nullptr;
    purpleShadowLayer = nullptr;
    back = nullptr;
}

bool AdScreen::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    purpleShadowLayer = LayerColor::create({65, 38, 102, 255});
    purpleShadowLayer->setPosition({0, 0});
    
    this->addChild(purpleShadowLayer);
    
    back = Sprite::create("ads/adsBack.png");
    back->setPosition({visibleSize.width / 2.0, visibleSize.height / 2.0});
    this->addChild(back);
    
    // continue btn
    continueBtn = MenuItemImage::create("ads/adsContinueBtn.png", "ads/adsContinueBtnOn.png", "ads/adsContinueBtnOn.png",
                                      CC_CALLBACK_1(AdScreen::onContinueBtnPressed, this));
    
    Menu *menu = Menu::create(continueBtn, NULL);
    menu->setPosition({back->getContentSize().width / 2.0, 70});
    
    back->addChild(menu);

    return true;
}

void AdScreen::onContinueBtnPressed(cocos2d::Object *sender) {
    Director::getInstance()->replaceScene(TransitionFade::create(0.3, TicketScreen::scene()));
}