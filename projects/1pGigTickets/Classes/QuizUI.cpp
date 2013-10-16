//
//  File.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 16.10.13.
//
//

#include "QuizUI.h"
#include "GameScene.h"

#define kTemporalQuitBtnTag 121

QuizUI::QuizUI() {
//    shadowLayer = nullptr;
//    submitBtn = nullptr;
//    missOutItem = nullptr;
//    topScoreSprite = nullptr;
//    scoreLabel = nullptr;
}

QuizUI::~QuizUI() {
    
}

bool QuizUI::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // shadow
    shadowLayer = LayerColor::create({0, 0, 0, 220});
    shadowLayer->setPosition({0, 0});
    shadowLayer->setOpacity(0);
    this->addChild(shadowLayer);
    
    // back
    
    int backDisp = 0;
    if(visibleSize.height == 1136) {
        backDisp = 40;
    }
    
    back = Sprite::create("quiz/quizBack.png");
    back->setPosition({visibleSize.width / 2.0, visibleSize.height / 2.0 + backDisp});
    
    this->addChild(back);
    
    // menu
//    submitBtn = MenuItemImage::create("scores/scoreSubmitBtn.png", "scores/scoreSubmitBtnOn.png", "scores/scoreSubmitBtnOn.png",
//                                      CC_CALLBACK_1(ScoreUI::onSubmitBtnPressed, this));
//    
//    missOutItem = MenuItemImage::create("scores/scoreMissOutBtn.png", "scores/scoreMissOutBtnOn.png", "scores/scoreMissOutBtnOn.png",
//                                        CC_CALLBACK_1(ScoreUI::onMissOutBtnPressed, this));
//
    
    // temporal quit btn
    MenuItemImage *btn = MenuItemImage::create("quiz/quizBack.png", "quiz/quizBack.png", CC_CALLBACK_0(QuizUI::popOut, this));
    Menu *menu = Menu::create(btn, nullptr);

    menu->setPosition(back->getContentSize().width / 2.0, back->getContentSize().height / 2.0);
    menu->setTag(kTemporalQuitBtnTag);
    back->addChild(menu);
    menu->setEnabled(false);
    
    this->setPosition(-visibleSize.width, 0);
    
    return true;
}

#pragma mark - popup-out

void QuizUI::popUp() {
    this->setPosition({0, 0});
    
    shadowLayer->runAction(FadeTo::create(0.2, 140)); // 220
    
    Menu *menu = (Menu*)back->getChildByTag(kTemporalQuitBtnTag);
    menu->setEnabled(true);
    
    back->setScale(0);
    back->runAction(Sequence::create(DelayTime::create(0.15),
                                     EaseElasticOut::create(ScaleTo::create(0.5, 1.0f)),
                                     NULL));
}

void QuizUI::popOut() {
    Menu *menu = (Menu*)back->getChildByTag(kTemporalQuitBtnTag);
    menu->setEnabled(false);
    
    shadowLayer->runAction(FadeTo::create(0.2, 0)); // 220
    
    back->runAction(Sequence::create(//DelayTime::create(0.15),
                                     FadeOut::create(0.3),
                                     CallFunc::create([this](){
                                        Size visibleSize = Director::getInstance()->getVisibleSize();
        
                                        this->setPosition(-visibleSize.width, 0);
                                        gameLayer->onQuizUIPoppedOut();
                                     }),
                                     NULL));
}