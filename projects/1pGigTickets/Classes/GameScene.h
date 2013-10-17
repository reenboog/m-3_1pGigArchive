
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "FieldWatcherDelegate.h"

USING_NS_CC;

class GemField;
class GameUI;
class ScoreUI;
class QuizUI;

class GameScene: public cocos2d::Layer, public FieldWatcherDelegate {
public:
    ~GameScene();
    GameScene();
    // cocos2d stuff
    virtual bool init();
    static cocos2d::Scene * scene();
    CREATE_FUNC(GameScene);

    // touches
	virtual void ccTouchEnded(Touch *touch, Event *event);
	virtual bool ccTouchBegan(Touch *touch, Event *event);
	virtual void ccTouchMoved(Touch *touch, Event *event);
    
    // touch extras
    Point convertLocationToFieldCoordinates(const Point &point);
    Point convertFieldCoordinatesToWorldLocation(const Point &point);
    
    // some gui
    
    void onBoostBtnPressed();
    
    void popMatchScoresUpAtPoint(int score, int x, int y);

    // field watcher delegate methods
    virtual void onGemDestroyed(GemColour colour, int x, int y, int score);
	virtual void onGemsMatched(int length, GemColour colour, int startX, int startY, int endX, int endY, int score);
	virtual void onGemsToBeShuffled();
	virtual void onGemsStartedSwapping();
	virtual void onGemsFinishedMoving();
	virtual void onMoveMade(bool legal);
    
    virtual void onPlectrumComboReady();
    
    // quiz
    void showQuizUI();
    void onQuizUIPoppedOut();
    
    void onCorrectQuizAnswer();
    
    // update logic
    void update(float dt);
    void onGameOver();
    
    // ui freeze
    void onExit();
    void onEnter();
    
    // boost/quiz
    void setBoost(float value);
    void setQuiz(float value);
    
    void addBoost(float value);
    void addQuiz(float value);
    
    void addScore(int score);
    
    // reset
    void reset();
private:
    GemField *field;
    GameUI *ui;
    ScoreUI *scoreUI;
    QuizUI *quizUI;

    Sprite *back;
    
    bool swipeEnded;
	bool canTouch;
    
    bool gameOver;
    
    Point firstTouchLocation;
    
    float currentTime;
    int score;
    int scoreMultiplier;
    
    float boostTimer;
    
    // boost/quiz value
    float boostValue;
    float quizValue;
    int boostAttemptsLeft;
};

#endif
