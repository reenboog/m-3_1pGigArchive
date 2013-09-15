
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "FieldWatcherDelegate.h"

USING_NS_CC;

class GemField;

class GameScene : public cocos2d::Layer, public FieldWatcherDelegate {
private:
    GemField *field;
    Sprite *back;
    
    bool swipeEnded;
	bool canTouch;
    
    Point firstTouchLocation;
public:
    // cocos2d stuff
    virtual bool init();
    static cocos2d::Scene * scene();
    CREATE_FUNC(GameScene);
    
    // touches
    // Working with touches
	virtual void ccTouchEnded(Touch *touch, Event *event);
	virtual bool ccTouchBegan(Touch *touch, Event *event);
	virtual void ccTouchMoved(Touch *touch, Event *event);
    
    // touch extras
    Point convertToCoordinates(Point point);
    Point convertTouchToCoordinates(Touch *touch);

    // field watch delegate methods
    virtual void onGemDestroyed(GemColour colour);
	virtual void onMatch(int length, GemColour colour);
	virtual void onFieldShuffleStarted();
	virtual void onMovementStarted();
	virtual void onMovementEnded();
	virtual void onMove(bool legal);
    
    // update logic
    void update(float dt);
};

#endif
