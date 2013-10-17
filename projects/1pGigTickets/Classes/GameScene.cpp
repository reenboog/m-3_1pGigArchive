
#include "GameScene.h"
#include "GemField.h"
#include "Shared.h"
#include "GameConfig.h"
#include "ScoreUI.h"
#include "GameUI.h"
#include "QuizUI.h"

using std::string;

#pragma mark - cocos2d stuff

GameScene::~GameScene() {
}

GameScene::GameScene() {
    field = nullptr;
    back = nullptr;
    ui = nullptr;
    scoreUI = nullptr;
    quizUI = nullptr;

    gameOver = true;

    boostValue = 0;
    quizValue = 0;
    boostAttemptsLeft = kBoostMaxAttempts;
    scoreMultiplier = 1;
    boostTimer = 0;
    
    currentTime = GameConfig::sharedInstance()->gameTimer;
    score = 0;
}

#pragma mark - init

Scene * GameScene::scene() {
    Scene *scene = Scene::create();

    GameScene *layer = GameScene::create();
    scene->addChild(layer);
    
    GameUI *ui = GameUI::create();
    ui->gameLayer = layer;
    
    layer->ui = ui;
    scene->addChild(ui);
    
    ScoreUI *scoreUI = ScoreUI::create();
    scoreUI->gameLayer = layer;
    layer->scoreUI = scoreUI;

    scene->addChild(scoreUI);
    
    QuizUI *quizUI = QuizUI::create();
    quizUI->gameLayer = layer;
    layer->quizUI = quizUI;
    
    scene->addChild(quizUI);
    
    layer->reset();

    return scene;
}

bool GameScene::init() {
    if(!Layer::init()) {
        return false;
    }
    
    // add sprite sheets
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemGuitar.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemKeyboard.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemMark.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemMic.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemPlectrum.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemSax.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/gemNote.plist");

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems/effects.plist");

    Shared::loadAnimation("animations.plist", "guitar");
    Shared::loadAnimation("animations.plist", "keyboard");
    Shared::loadAnimation("animations.plist", "mark");
    Shared::loadAnimation("animations.plist", "mic");
    Shared::loadAnimation("animations.plist", "plectrum");
    Shared::loadAnimation("animations.plist", "sax");
    Shared::loadAnimation("animations.plist", "note");

    Shared::loadAnimation("animations.plist", "gemDeath");
    Shared::loadAnimation("animations.plist", "gemBlast");
    Shared::loadAnimation("animations.plist", "lightning");
    Shared::loadAnimation("animations.plist", "lightningSmall");
    //
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    back = Sprite::create("gameBack.png");
    back->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    this->addChild(back, 0);
    
    field = GemField::create();
    field->addWatcher(this);
    
    this->addChild(field);
    
	float posX = (visibleSize.width - kTileSize * (kFieldWidth)) / 2;
	float posY = (visibleSize.height - kTileSize * (kFieldHeight)) / 2 - GameConfig::sharedInstance()->iOSFieldDisplacement.y;
	
    field->setPosition(posX, posY);
    
    // prepare for touches
    canTouch = false;
    swipeEnded = false;

    firstTouchLocation = Point(-1, -1);

    Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    scheduleUpdate();
    
    return true;
}

void GameScene::reset() {
    boostValue = 0;
    quizValue = 0;
    boostAttemptsLeft = kBoostMaxAttempts;
    
    currentTime = GameConfig::sharedInstance()->gameTimer;
    score = 0;
    scoreMultiplier = 1;
    boostTimer = 0;
    
    gameOver = false;
    
    // reset ui
    // in case we have enough plectrums
    ui->setBoostBtnEnabled(false);
    ui->setscore(0);
    ui->setTime(currentTime);
    ui->setscore(0);
    ui->setPlectrums(GameConfig::sharedInstance()->currentPlectrums);
}

#pragma mark - field watcher delegate

void GameScene::onGemDestroyed(GemColour colour, int x, int y, int score) {
    score *= scoreMultiplier * clampf((quizValue + 1), 0, 6);
    
    this->addScore(score);
    
    Point worldPos = this->convertFieldCoordinatesToWorldLocation({x, y});
    this->popMatchScoresUpAtPoint(score, worldPos.x, worldPos.y);
    
    // apply plectrums if any
    if(colour == GC_Plectrum) {
        GameConfig::sharedInstance()->currentPlectrums++;
        ui->setPlectrums(GameConfig::sharedInstance()->currentPlectrums);
    }
}

void GameScene::onGemsMatched(int length, GemColour colour, int startX, int startY, int endX, int endY, int score) {
    score *= scoreMultiplier * clampf((quizValue + 1), 0, 6);
    
    this->addScore(score);

    float stepX = 1.0f;
    float stepY = -1.0f;
    
    if(startX == endX) {
		stepX = 0.0f;
	} else {
		stepY = 0.0f;
	}
        
    Point worldPos = this->convertFieldCoordinatesToWorldLocation({startX, startY});
    
    worldPos = worldPos + Point(stepX * ((length - 1) * kTileSize) / 2.0f, stepY * ((length - 1) * kTileSize) / 2.0f);
    
    this->popMatchScoresUpAtPoint(score, worldPos.x, worldPos.y);
    
    // apply boost if any
    int scorePerGem = score / length;
    
    float boostPoints = 0;
    
    if(scorePerGem == GameConfig::sharedInstance()->baseIconValue) {
        switch(length) {
            case 3:
                boostPoints = kMatch3BoostPoints;
                break;
            case 4:
                boostPoints = kMatch4BoostPoints;
                break;
            case 5:
            default:
                boostPoints = kMatch5BoostPoints;
                break;
        }
    } else {
        boostPoints = kMatchWithBombsBoostPoints;
    }
    
    if(scoreMultiplier == 1) {
        this->addBoost(boostPoints);
    }

    // apply plectrums if any
    if(colour == GC_Plectrum) {
        GameConfig::sharedInstance()->currentPlectrums += 1;
        ui->setPlectrums(GameConfig::sharedInstance()->currentPlectrums);
    } else if(colour == GC_Question && quizValue < kQuizMaxValue) {
        this->showQuizUI();
    }
}

void GameScene::onGemsToBeShuffled() {
	CCLOG("I'm shuffling");
}

void GameScene::onGemsFinishedMoving() {
	canTouch = true;
}

void GameScene::onMoveMade(bool legal) {
	if(!legal) {
		//
	}
}

void GameScene::onPlectrumComboReady() {
    ui->setPlectrums(GameConfig::sharedInstance()->currentPlectrums + 20);
}

void GameScene::onGemsStartedSwapping() {
	canTouch = false;
}

#pragma mark -

void GameScene::popMatchScoresUpAtPoint(int score, int x, int y) {
    
    String scoreString = "";
    scoreString.appendWithFormat("%i", score);

    LabelBMFont *label = LabelBMFont::create(scoreString.getCString(), "cubanoScore.fnt");
    this->addChild(label, zMatchScore);
    
    label->setPosition({x, y});
    label->setScale(0.6);
    label->setOpacity(0);
    
    label->runAction(Sequence::create(DelayTime::create(0.3),
                                      Spawn::create(EaseBackIn::create(MoveBy::create(0.3, {0, 50})),
                                                    FadeIn::create(0.1),
                                                    ScaleTo::create(0.14, 1.0f), NULL),
                                      DelayTime::create(0.1),
                                      FadeOut::create(0.2),
                                      CallFunc::create([=](){
        label->removeFromParent();
    }),
                                      NULL));
}

#pragma mark - quiz

void GameScene::showQuizUI() {
    quizUI->popUp();
    
    this->onExit();
}

void GameScene::onQuizUIPoppedOut() {
    this->onEnter();
    ui->onEnter();
}

void GameScene::onCorrectQuizAnswer() {
    quizValue += 1;
    
    this->setQuiz(quizValue);
    // add additional bonus score
    this->addScore(100);
}

#pragma mark - ui

void GameScene::onBoostBtnPressed() {
    if(boostAttemptsLeft > 0 && boostValue > kBoostEffectMinimalValueRequired) {
        boostAttemptsLeft--;
        
        scoreMultiplier = 2;
        boostTimer = kBoostingTime;
        
        ui->setBoostBtnEnabled(false);
        ui->fadeBoostArrowIn();
        
        if(boostAttemptsLeft <= 0) {
            boostAttemptsLeft = 0;
        }
    }
}

void GameScene::onExit() {
    Layer::onExit();
    
    if(ui) {
        ui->onExit();
    }
}

void GameScene::onEnter() {
    Layer::onEnter();
    
//    if(ui) {
//        ui->onEnter();
//    }
}

#pragma mark - update logic

void GameScene::update(float dt) {
    if(gameOver) {
        return;
    }
    
    // apply time
    currentTime -= dt;
    
    if(currentTime < 0) {
        currentTime = 0;

        onGameOver();
    }
    
    ui->setTime(currentTime);
    
    if(scoreMultiplier == 1) {
        boostValue -= dt * kBoostFadeOutSpeed;
        this->setBoost(boostValue);
    } else {
        if(boostTimer > 0) {
            boostTimer -= dt;
        } else if(boostTimer <= 0 && boostTimer != -100) {
            boostTimer = -100;
            ui->fadeBoostArrowOut();
            
            this->runAction(Sequence::create(DelayTime::create(0.4),
                                             CallFunc::create([this](){
                                                scoreMultiplier = 1;
                                                boostValue = 0;
                                                boostTimer = 0;
                                             }),
                                             NULL));
        }
    }
}

void GameScene::onGameOver() {
    gameOver = true;
    
    this->onExit();
    
    scoreUI->setScore(score);
    scoreUI->popUp();
}

void GameScene::setBoost(float value) {
    if(boostAttemptsLeft <= 0) {
        return;
    }
    
    boostValue = value;
    
    if(boostValue > kBoostEffectMinimalValueRequired) {
        ui->setBoostBtnEnabled(true);
    } else {
        ui->setBoostBtnEnabled(false);
    }

    if(boostValue <= 0) {
        boostValue = 0;
    } else if(boostValue > kBoostMaxValue) {
        boostValue = kBoostMaxValue;
    }
    
    if(scoreMultiplier == 1) {
        ui->setBoost(boostValue);
    }
}

void GameScene::setQuiz(float value) {
    quizValue = value;
    
    if(quizValue <= 0) {
        quizValue = 0;
    } else if(quizValue > kQuizMaxValue) {
        quizValue = kQuizMaxValue;
    }
    
    ui->setQuiz(quizValue);
}

void GameScene::addBoost(float value) {
    boostValue += value;
    setBoost(boostValue);
}

void GameScene::addQuiz(float value) {
    quizValue += value;
    setQuiz(quizValue);
}

void GameScene::addScore(int score) {
    this->score += score;
    
    ui->setscore(this->score);
}

#pragma mark - touches

bool GameScene::ccTouchBegan(Touch *touch, Event *event) {
    Point location = touch->getLocation();
    location = this->convertToNodeSpace(location);
    
	firstTouchLocation = location;
    
	swipeEnded = false;
	return true;
}

void GameScene::ccTouchMoved(Touch *touch, Event *event) {
	Point touchLocation = convertTouchToNodeSpace(touch);

	if(!swipeEnded && canTouch && firstTouchLocation.getDistance(touchLocation) > (kTileSize / 2)) {
		swipeEnded = true;
        
		float differenceX = touchLocation.x - firstTouchLocation.x;
		float differenceY = touchLocation.y - firstTouchLocation.y;
        
		Direction direction;
		if(fabs(differenceX) >= fabs(differenceY)) {
			if(differenceX >= 0) {
				direction = D_Right;
			} else {
				direction = D_Left;
			}
		} else {
			if(differenceY >= 0) {
				direction = D_Up;
			} else {
				direction = D_Down;
			}
		}
		//field->makeMove(convertToCoordinates(firstTouchLocation), direction);
        field->swipeAction(convertLocationToFieldCoordinates(firstTouchLocation), direction);
	}
}

void GameScene::ccTouchEnded(Touch *touch, Event *event) {
    Point location = touch->getLocation();
    location = this->convertToNodeSpace(location);

	if(!swipeEnded && canTouch) {
		field->clickAction(convertLocationToFieldCoordinates(location));
	}
}

#pragma mark -

Point GameScene::convertLocationToFieldCoordinates(const Point &point) {
	return Point((int) ((point.x - field->getPositionX()) / kTileSize), (int) (kFieldHeight - (point.y - field->getPositionY()) / kTileSize));
}

Point GameScene::convertFieldCoordinatesToWorldLocation(const Point &point) {
    return Point(point.x * kTileSize + field->getPositionX() + kTileSize / 2.0,
                 (kFieldHeight - point.y) * kTileSize + field->getPositionY() - kTileSize / 2.0);
}
