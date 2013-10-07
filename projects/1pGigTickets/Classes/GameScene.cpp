
#include "GameScene.h"
#include "GemField.h"
#include "Shared.h"
#include "GameConfig.h"

using std::string;

#pragma mark - cocos2d stuff

GameScene::~GameScene() {
}

GameScene::GameScene() {
    field = nullptr;
}

#pragma mark - init

Scene * GameScene::scene() {
    Scene *scene = Scene::create();

    GameScene *layer = GameScene::create();
    scene->addChild(layer);

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
    
    // I'm really sorry for doing this(
    if(visibleSize.height == 1136) {
        GameConfig::sharedInstance()->iOSFieldDisplacement = Point(0, 31);
    } else {
        GameConfig::sharedInstance()->iOSFieldDisplacement = Point(14, 31);
    }

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
    
    return true;
}

#pragma mark - field watcher delegate

void GameScene::onGemDestroyed(GemColour colour, int x, int y, int score) {
	// get is destroyed
    
    Point worldPos = this->convertFieldCoordinatesToWorldLocation({x, y});
    this->popMatchScoresUpAtPoint(score, worldPos.x, worldPos.y);
}

void GameScene::onGemsMatched(int length, GemColour colour, int startX, int startY, int endX, int endY, int score) {
	// on match
    
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

void GameScene::onGemsStartedSwapping() {
	canTouch = false;
}

#pragma mark - update logic

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

void GameScene::update(float dt) {
    
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
