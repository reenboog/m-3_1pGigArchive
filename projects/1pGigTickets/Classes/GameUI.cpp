//
//  GameUI.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 08.10.13.
//
//

#include "GameUI.h"
#include "Constants.h"
#include "TicketScreen.h"
#include "GameScene.h"

#define kIndicatorStartAngle 66

GameUI::~GameUI() {
}

GameUI::GameUI(): Layer() {
    pauseBtn = nullptr;
    timeMount = nullptr;
    scoreMount = nullptr;
    plectrumsMount = nullptr;

    boostBtn = nullptr;
    
    boostBack = nullptr;
    boostArrow = nullptr;
    boostTop = nullptr;
    
    quizBack = nullptr;
    quizArrow = nullptr;
    quizTop = nullptr;
    
    timeLabel = nullptr;
    scoreLabel = nullptr;
    plectrumsLabel = nullptr;
    
    gameLayer = nullptr;
}

#pragma mark - init

bool GameUI::init() {
    if(!Layer::init()) {
        return false;
    }
    
    // pause
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    float pauseMenuDisp = 0.971;
    float timeMountDisp = 0.968;
    float scoreMountDisp = 0.968;
    float plectrumDisp = 0.95;
    float boostMenuDisp = 0.84;
    float boostBackDisp = 0.86;
    float quizBackDisp = 0.86;
    
    if(visibleSize.height == 1136) {
        pauseMenuDisp = 0.97;
        timeMountDisp = 0.972;
        scoreMountDisp = 0.972;
        plectrumDisp = 0.958;
        boostMenuDisp = 0.858;
        boostBackDisp = 0.868;
        quizBackDisp = 0.868;
    }
    
    pauseBtn = MenuItemImage::create("ui/pauseBtn.png", "ui/pauseBtnOn.png", "ui/pauseBtnOn.png", CC_CALLBACK_1(GameUI::onPauseBtnPressed, this));
    pauseBtn->setPosition({0, 0});
    
    Menu *pauseMenu = Menu::create(pauseBtn, NULL);
    pauseMenu->setPosition({visibleSize.width * 0.053, visibleSize.height * pauseMenuDisp});
    
    this->addChild(pauseMenu);
    
    // time mount
    timeMount = Sprite::create("ui/timeMount.png");
    timeMount->setPosition({visibleSize.width * 0.23, visibleSize.height * timeMountDisp});
    
    this->addChild(timeMount);
    
    // score mount
    scoreMount = Sprite::create("ui/scoreMount.png");
    scoreMount->setPosition({visibleSize.width * 0.79, visibleSize.height * scoreMountDisp});
    
    this->addChild(scoreMount);
    
    // plectrums mount
    plectrumsMount = Sprite::create("ui/plectrumsMount.png");
    plectrumsMount->setPosition({visibleSize.width * 0.5, visibleSize.height * plectrumDisp});
    
    this->addChild(plectrumsMount);

    // boost btn;
    
    boostBtn = MenuItemImage::create("ui/boostBtn.png", "ui/boostBtnOn.png", "ui/boostBtnOn.png", CC_CALLBACK_1(GameUI::onBoostBtnPressed, this));
    boostBtn->setPosition({0, 0});
    
    Menu *boostMenu = Menu::create(boostBtn, NULL);
    boostMenu->setPosition({visibleSize.width * 0.5, visibleSize.height * boostMenuDisp});
    
    this->addChild(boostMenu);
    
    // boost mount
    
    boostBack = Sprite::create("ui/indicatorBoost.png");
    boostBack->setPosition({visibleSize.width * 0.21, visibleSize.height * boostBackDisp});
    
    this->addChild(boostBack);
    
    boostArrow = Sprite::create("ui/indicatorArrow.png");
    boostArrow->setAnchorPoint({0.5, 0});
    boostArrow->setRotation(-kIndicatorStartAngle);
    boostArrow->setPosition({boostBack->getContentSize().width * 0.5, boostBack->getContentSize().height * 0.15});
    boostArrow->setScaleY(0.9);
    
    boostBack->addChild(boostArrow);
    
    boostTop = Sprite::create("ui/indicatorTop.png");
    boostTop->setPosition({boostBack->getContentSize().width * 0.5, boostBack->getContentSize().height * 0.5});
    
    boostBack->addChild(boostTop);
    
    // quiz mount
    
    quizBack = Sprite::create("ui/indicatorQuiz.png");
    quizBack->setPosition({visibleSize.width * 0.79, visibleSize.height * quizBackDisp});
    
    this->addChild(quizBack);
    
    quizArrow = Sprite::create("ui/indicatorArrow.png");
    quizArrow->setAnchorPoint({0.5, 0});
    quizArrow->setRotation(-kIndicatorStartAngle);
    quizArrow->setPosition({quizBack->getContentSize().width * 0.5, quizBack->getContentSize().height * 0.15});
    quizArrow->setScaleY(0.9);
    
    quizBack->addChild(quizArrow);
    
    quizTop = Sprite::create("ui/indicatorTop.png");
    quizTop->setPosition({quizBack->getContentSize().width * 0.5, quizBack->getContentSize().height * 0.5});
    
    quizBack->addChild(quizTop);

    // labels
    
    timeLabel = LabelBMFont::create("00:00", "allerYellowTime.fnt");
    timeLabel->setPosition({timeMount->getContentSize().width * 0.5, timeMount->getContentSize().height * 0.5});
    timeMount->addChild(timeLabel);
    
    scoreLabel = LabelBMFont::create("0", "allerWhiteScores.fnt");
    scoreLabel->setAnchorPoint({1, 0.5});
    scoreLabel->setPosition({scoreMount->getContentSize().width * 0.9, scoreMount->getContentSize().height * 0.5});
    scoreMount->addChild(scoreLabel);
    
    plectrumsLabel = LabelBMFont::create("0", "allerWhiteScores.fnt");
    plectrumsLabel->setPosition({plectrumsMount->getContentSize().width * 0.5, plectrumsMount->getContentSize().height * 0.6});
    plectrumsMount->addChild(plectrumsLabel);
    
    return true;
}

#pragma mark - callbacks

void GameUI::onPauseBtnPressed(Object *sender) {
    CCLOG("pause");
    
    Director::getInstance()->replaceScene(TransitionFade::create(0.3, TicketScreen::scene()));
}

void GameUI::onBoostBtnPressed(Object *sender) {
    gameLayer->onBoostBtnPressed();
}

#pragma mark - set/get

void GameUI::setBoost(float value) {
    float rotation = -kIndicatorStartAngle + (kIndicatorStartAngle * 2) / kBoostMaxValue * value;
    
    boostArrow->setRotation(rotation);
}

void GameUI::setQuiz(float value) {
    float rotation = -kIndicatorStartAngle + (kIndicatorStartAngle * 2) / kQuizMaxValue * value;
    
    quizArrow->setRotation(rotation);
}

void GameUI::fadeBoostArrowIn() {
    boostArrow->runAction(RotateTo::create(0.3, kIndicatorStartAngle));
}

void GameUI::fadeBoostArrowOut() {
    boostArrow->runAction(RotateTo::create(0.35, -kIndicatorStartAngle));
}

void GameUI::setTime(int seconds) {
    // format time here
    String timeStr = "";

    int min = seconds / 60;
    int sec = seconds % 60;
    
    if(min < 10) {
        timeStr.appendWithFormat("0%i:", min);
    } else {
        timeStr.appendWithFormat("%i:", min);
    }
    
    if(sec < 10) {
        timeStr.appendWithFormat("0%i", sec);
    } else {
        timeStr.appendWithFormat("%i", sec);
    }

    timeLabel->setString(timeStr.getCString());
}

void GameUI::setscore(int score) {
    // format score here
    String scoreStr = "";
    scoreStr.appendWithFormat("%i", score);
    
    scoreLabel->setString(scoreStr.getCString());
}

void GameUI::setPlectrums(int value) {
    String plectrumsStr = "";
    plectrumsStr.appendWithFormat("%i", value);
    
    plectrumsLabel->setString(plectrumsStr.getCString());

    if(value > 999) {
        plectrumsLabel->setScale(0.65);
        plectrumsLabel->setString("999+");
    } else if(value > 99) {
        plectrumsLabel->setScale(0.85);
    } else {
        plectrumsLabel->setScale(1.0);
    }
    
}

#pragma mark - boost btn

void GameUI::setBoostBtnEnabled(bool enabled) {
    if(enabled) {
        if(!boostBtn->isEnabled()) {
            boostBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 1.1),
                                                                       ScaleTo::create(0.4, 1.0),
                                                                       NULL)));
        }
    } else {
        boostBtn->stopAllActions();
        boostBtn->setScale(1);
    }
    
    boostBtn->setEnabled(enabled);
}