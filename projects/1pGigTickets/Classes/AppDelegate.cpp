
#include "AppDelegate.h"
#include "GameScene.h"
#include "TicketScreen.h"
#include "GameConfig.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
    delete GameConfig::sharedInstance();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    Director * director = Director::getInstance();
    EGLView * eglView = EGLView::getInstance();

    director->setOpenGLView(eglView);
    
    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    Director::getInstance()->setProjection(Director::Projection::_2D);
	
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // I'm really sorry for doing this(
    if(visibleSize.height == 1136) {
        GameConfig::sharedInstance()->iOSFieldDisplacement = Point(10, 33);
        FileUtils::getInstance()->addSearchPath("Res/iphone5");
    } else {
        GameConfig::sharedInstance()->iOSFieldDisplacement = Point(58, 56);
        FileUtils::getInstance()->addSearchPath("Res/iphone");
    }
    
    FileUtils::getInstance()->addSearchPath("Res");
    
    // load game config
    GameConfig::sharedInstance()->load();

    // create a scene. it's an autorelease object
    Scene *scene = TicketScreen::scene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}