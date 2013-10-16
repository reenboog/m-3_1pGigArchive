//
//  ScoreUI.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 16.10.13.
//
//

#include "ScoreUI.h"
#include "AdScreen.h"
#include "GameScene.h"

ScoreUI::ScoreUI() {
    shadowLayer = nullptr;
    submitBtn = nullptr;
    missOutItem = nullptr;
    topScoreSprite = nullptr;
    scoreLabel = nullptr;
}

ScoreUI::~ScoreUI() {
    
}

bool ScoreUI::init() {
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
    back = Sprite::create("scores/scoreBack.png");
    back->setPosition({visibleSize.width / 2.0, visibleSize.height / 2.0});
    
    this->addChild(back);
    
    // menu
    submitBtn = MenuItemImage::create("scores/scoreSubmitBtn.png", "scores/scoreSubmitBtnOn.png", "scores/scoreSubmitBtnOn.png",
                                      CC_CALLBACK_1(ScoreUI::onSubmitBtnPressed, this));

    missOutItem = MenuItemImage::create("scores/scoreMissOutBtn.png", "scores/scoreMissOutBtnOn.png", "scores/scoreMissOutBtnOn.png",
                                        CC_CALLBACK_1(ScoreUI::onMissOutBtnPressed, this));
    
    Menu *menu = Menu::create(submitBtn, missOutItem, NULL);
    menu->alignItemsHorizontallyWithPadding(55);
    
    menu->setPosition({visibleSize.width / 2.0, 189});
    
    back->addChild(menu);
    
    // score label
    scoreLabel = LabelBMFont::create("289 000", "scoresBigScore.fnt");
    scoreLabel->setPosition({back->getContentSize().width / 2.0, 558});
    
    back->addChild(scoreLabel);
    
    // fake top score label
    topScoreSprite = Sprite::create("scores/scoreTopLabel.png");
    topScoreSprite->setPosition({back->getContentSize().width / 2.0, 427});
    
    back->addChild(topScoreSprite);
    
    this->setPosition(-visibleSize.width, 0);
    
    return true;
}

#pragma mark - animations

void ScoreUI::popUp() {
    this->setPosition({0, 0});
    
    shadowLayer->runAction(FadeTo::create(0.2, 220));
    
    back->setScale(0);
    back->runAction(Sequence::create(DelayTime::create(0.15),
                                     EaseElasticOut::create(ScaleTo::create(0.5, 1.0f)),
                                     NULL));
}

void ScoreUI::popOut() {
    
}

#pragma mark - callbacks

void ScoreUI::onSubmitBtnPressed(cocos2d::Object *sender) {
    
}

void ScoreUI::onMissOutBtnPressed(cocos2d::Object *sender) {
    Director::getInstance()->replaceScene(TransitionFade::create(0.3, AdScreen::scene()));
}

#pragma mark - setters

void ScoreUI::setScore(int score) {
    String *scores = String::createWithFormat("%i", score);
    
    scoreLabel->setString(scores->getCString());
}