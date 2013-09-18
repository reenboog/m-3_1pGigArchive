
#include "GameScene.h"
#include "GemField.h"

#pragma mark - cocos2d stuff

Scene * GameScene::scene() {
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
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
    
    //
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    back = Sprite::create("gameBack.png");

    back->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    this->addChild(back, 0);
    
    field = new GemField();
    field->addWatcher(this);
    
    this->addChild(field);
	float posX = (visibleSize.width - kTileSize * (kFieldWidth)) / 2;
	float posY = (visibleSize.height - kTileSize * (kFieldHeight)) / 2 - 20;
	field->setPosition(posX, posY);
    
    // prepare for touches
    canTouch = false;
    swipeEnded = false;
    firstTouchLocation = Point(-1, -1);
    Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    return true;
}

#pragma mark - field watcher delegate

void GameScene::onGemDestroyed(GemColour colour) {
	// get is destroyed
}

void GameScene::onGemsMatched(int length, GemColour colour) {
	// on match
}

void GameScene::onGemsToBeShuffled() {
	CCLOG("I'm shuffling");
}

void GameScene::onMovementEnded() {
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

void GameScene::update(float dt) {
    
}

#pragma mark - touches

bool GameScene::ccTouchBegan(Touch *touch, Event *event) {
	firstTouchLocation = convertTouchToNodeSpace(touch);
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
        field->swipeAction(convertToCoordinates(firstTouchLocation), direction);
	}
}

void GameScene::ccTouchEnded(Touch *touch, Event *event) {
	if(!swipeEnded && canTouch) {
		field->clickAction(convertTouchToCoordinates(touch));
	}
}

Point GameScene::convertToCoordinates(Point point) {
	return Point((int) ((point.x - field->getPositionX()) / kTileSize), (int) (kFieldHeight - (point.y - field->getPositionY()) / kTileSize));
}

Point GameScene::convertTouchToCoordinates(Touch *touch) {
	return convertToCoordinates(convertTouchToNodeSpace(touch));
}

