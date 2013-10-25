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
    timeProgressMount = nullptr;
    timeProgress = nullptr;
    currentTime = 0;
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
    
    timeProgressMount = Sprite::create("quiz/quizProgressMount.png");
    timeProgressMount->setPosition({back->getContentSize().width / 2 + 1, back->getContentSize().height * 0.72});
    
    timeProgress = ProgressTimer::create(Sprite::create("quiz/quizProgress.png"));
    timeProgress->setPosition({timeProgressMount->getContentSize().width / 2.0, timeProgressMount->getContentSize().height / 2.0});
    timeProgress->setType(ProgressTimer::Type::BAR);
    timeProgress->setBarChangeRate({1, 0});
    timeProgress->setMidpoint({0, 0});
    timeProgress->setPercentage(0);
    
    timeProgressMount->addChild(timeProgress);
    
    back->addChild(timeProgressMount);
    
    this->setPosition(-visibleSize.width, 0);
    
    return true;
}

#pragma mark - popup-out

void QuizUI::popUp() {
    Menu *menu = (Menu*)back->getChildByTag(kTemporalQuitBtnTag);
    
    if(menu->isEnabled()) {
        return;
    }
    
    menu->setEnabled(true);
    
    this->setPosition({0, 0});
    
    shadowLayer->runAction(FadeTo::create(0.2, 140)); // 220
    
    
    back->setScale(0);
    back->runAction(Sequence::create(DelayTime::create(0.15),
                                     EaseElasticOut::create(ScaleTo::create(0.5, 1.0f)),
                                     NULL));
 
    currentTime = kQuizTime;
    
    timeProgress->setPercentage(0);
    
    this->scheduleUpdate();
}

void QuizUI::popOut() {
    this->unscheduleUpdate();
    
    gameLayer->onCorrectQuizAnswer();
    
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

void QuizUI::onTimeOut() {
    this->popOut();
}

void QuizUI::update(float dt) {
    currentTime -= dt;
    
    if(currentTime <= 0) {
        currentTime = 0;
        
        this->onTimeOut();
    }
    
    timeProgress->setPercentage(currentTime / kQuizTime * 100);
}