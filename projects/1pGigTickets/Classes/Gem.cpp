#include"Gem.h"

#include "cocos2d.h"
#include "ccMacros.h"
#include "Constants.h"

USING_NS_CC;

using std::string;

#pragma mark - init

Gem::Gem(GemColour colour, GemType type) {
	// Generate a random gem type if it is not specified
	if(colour == GC_Random) {
		colour =  static_cast<GemColour>((int) ((CCRANDOM_0_1() * kGemTypeAmount) + 1));
	}
	
    this->colour = colour;
	this->type = type;
	this->state = GS_Idle;
}

Gem::~Gem() {
	CCLOG("Gem destructor");
}

void Gem::init(int x, int y, GemColour colour, GemType type) {
	// Get the file name for the gem image
	string fileName = "";
	int zOrder = zGem;
	if(colour == GC_Random) {
		colour =  static_cast<GemColour>((int) ((CCRANDOM_0_1() * kGemTypeAmount) + 1));
	}

    this->colour = colour;
	this->type = type;
	this->state = GS_Idle;

    setGemColour(colour);

	setPosition(convertCoordinatesToPixels(x,y));
	setZOrder(zOrder);

//	switch(type) {
//		case GT_Cross: setOpacity(125);
//				break;
//		case GT_LineHor: setFlipY(true);
//				break;
//		case GT_LineVer: setFlipX(true);
//				break;
//		default:
//				break;
//	}
}

void Gem::reset(int x, int y, GemColour colour, GemType type) {
	// Set parameters
	if(colour == GC_Random) {
		colour =  static_cast<GemColour>((int) ((CCRANDOM_0_1() * kGemTypeAmount) + 1));
	}
	this->colour = colour;
	this->type = type;
	this->state = GS_Idle;
    
    this->stopAllActions();
    
    setGemColour(colour);
    
    setPosition(convertCoordinatesToPixels(x,y));
    
    setFlipX(false);
    setFlipY(false);
    setScale(1.0);
    setRotation(0);
    setOpacity(255);
    
    // Add bonus styling
//    switch(type) {
//        case GT_Cross: setOpacity(125);
//            break;
//        case GT_LineHor: setFlipY(true);
//            break;
//        case GT_LineVer: setFlipX(true);
//            break;
//        default:
//            break;
//    }
}

#pragma mark - bonuses

void Gem::transformIntoBonus(GemType type, float delay, GemState completionState) {
    if(state == GS_Transforming) {
        return;
    }
    
	if(type != GT_Colour) {
		this->type = type;
		this->state = GS_Transforming;
        
		//applyBonusStyling();
        
		Action *enlarge = ScaleTo::create(kTransformationTime / 3.f, 1.5f);
		Action *restyle = nullptr;
        
		switch(type) {
//			case GT_Cross:
//				restyle = FadeTo::create(kTransformationTime / 3.f, 125);
//				break;
            case GT_NoteMaker:
                setGemColour(GC_Note);
                break;
			case GT_LineHor:
				//restyle = Sequence::create(ScaleTo::create(kTransformationTime / 6.f, 1, 0),
                //                           //FlipY::create(true),
                //                           ScaleTo::create(kTransformationTime / 6.f, 1, 1),
                //                           NULL);
				break;
			case GT_LineVer:
				//restyle = Sequence::create(ScaleTo::create(kTransformationTime / 6.f, 0, 1),
                                           //FlipX::create(true),
                //                           ScaleTo::create(kTransformationTime / 6.f, 1, 1),
                //                           NULL);
				break;
			default:
				restyle = DelayTime::create(kTransformationTime / 3.f);
				break;
		}
        
        auto applyAnimation = [&]() {
            string animationName = "";
            
            if(this->type != GT_NoteMaker) {
                switch(colour) {
                    case GC_Guitar: animationName = "guitar"; break;
                    case GC_Keyboard: animationName = "keyboard"; break;
                    case GC_Microphone: animationName = "mic"; break;
                    case GC_Plectrum: animationName = "plectrum"; break;
                    case GC_Question: animationName = "mark"; break;
                    case GC_Saxophone: animationName = "sax"; break;
                        
                    default: CCLOG("default gem color in reset!");
                }
            } else {
                animationName = "note";
            }
            
            Animate *action = Animate::create(AnimationCache::getInstance()->animationByName(animationName.c_str()));
            runAction(RepeatForever::create(action));
        };
        
        Action *endTransformation = nullptr;
        
        switch(completionState) {
            case GS_AboutToExplodeByNote:
                endTransformation = CallFunc::create(CC_CALLBACK_0(Gem::prepareToExplodeByNote, this));
                break;
            case GS_Transformed:
            default:
                endTransformation = CallFunc::create(CC_CALLBACK_0(Gem::onTransformationEnd, this));
                break;
        }
        
		Action *shrink = ScaleTo::create(kTransformationTime / 3.f, 1);
        Action *animation = CallFunc::create(applyAnimation);
		Action *destruction = Sequence::create(DelayTime::create(delay),
                                               //(FiniteTimeAction*) enlarge,
                                               //(FiniteTimeAction*) restyle,
                                               //(FiniteTimeAction*) shrink,
                                               (FiniteTimeAction*) animation,
                                               (FiniteTimeAction*) endTransformation,
                                               NULL);
        runAction(destruction);
	} else {
		this->state = GS_Matched;
	}
}

void Gem::transformIntoPlectrum(float delay) {
    this->state = GS_Transforming;
    this->colour = GC_Plectrum;
    
    setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("plectrum0.png"));
    
    this->runAction(Sequence::create(DelayTime::create(delay),
                                     ScaleTo::create(kTransformationTime / 6.f, 1.1),
                                     ScaleTo::create(kTransformationTime / 5.f, 0.9),
                                     ScaleTo::create(kTransformationTime / 4.f, 1),
                                     CallFunc::create([this]() {
                                        this->onTransformationEnd();
                                     }),
                                     NULL));
}

void Gem::onTransformationEnd() {
	state = GS_Transformed;    
}

void Gem::applyBonusStyling() {
	// Add bonus styling
    switch(type) {
        case GT_Cross: setOpacity(125);
            break;
        case GT_LineHor: setFlipY(true);
            break;
        case GT_LineVer: setFlipX(true);
            break;
        default:
            break;
    }
}

#pragma mark - movement/selection

void Gem::onMovementEnd(Object *sender) {
	state = GS_Moved;
}

void Gem::select() {
	state = GS_Selected;
	//setScale(1.2f);
}

void Gem::deselect() {
	state = GS_Idle;
	setScale(1);
}

#pragma mark - swapping

void Gem::swapTo(int x, int y, bool goBack, GemState completionState) {
	moveTo(x, y, kSwapTime, goBack, 0, 0, completionState, true);
}

void Gem::fallTo(int x, int y, int blocksToWait, int rowsToWait) {
    
	moveTo(x, y, kFallTime, false, blocksToWait, rowsToWait);
}

void Gem::moveTo(int x, int y, float time, bool goBack, int blocksToWait, int rowsToWait, GemState completionState, bool swapping) {
    
    if(state == GS_Moving) {
        return;
    }
    
	Point newLocation = convertCoordinatesToPixels(x, y);
    Point currentPos = getPosition();
    
    Point delta = (newLocation - currentPos).normalize();
    
    float rowDelay = 0;
    
    if(newLocation.y < currentPos.y && !swapping) {
        int gemRow = currentPos.y / kTileSize;
        rowDelay = gemRow * 0.021;
    }
    
    float moveDownDelay = 0;
    
    if(swapping && !goBack) {
        moveDownDelay = 0.1;
    }
    
	Action *move = nullptr;

    Action *moveDown = MoveTo::create(((this->getPosition().getDistance(newLocation) / kTileSize) * time) * 0.7f,
                                      newLocation + Point(delta.x * kTileSize * 0.2f, delta.y * kTileSize * 0.2f));

    Action *moveUp = MoveBy::create(0.15, -Point(delta.x * kTileSize * 0.2, delta.y * kTileSize * 0.2f));

    move = Sequence::create((FiniteTimeAction*) moveDown, DelayTime::create(moveDownDelay), (FiniteTimeAction*) moveUp, NULL);

    Action *wait = DelayTime::create(blocksToWait * kFallTime * kColumnsFallDelay + rowsToWait * kFallTime * kRowsFallDelay + rowDelay);
    
	Action *moveBack = MoveTo::create((this->getPosition().getDistance(newLocation) / kTileSize) * time, this->getPosition());
	Action *movement = nullptr;
    
    Action *endMove = nullptr;
    
    switch(completionState) {
        case GS_AboutToDestroyByNote:
            endMove = CallFunc::create(CC_CALLBACK_0(Gem::prepareToBeDestroyedByNote, this));
            break;
        case GS_AboutToTurnIntoBomb:
            endMove = CallFunc::create(CC_CALLBACK_0(Gem::prepareToTurnIntoBombByNote, this));
            break;
        case GS_AboutToExplodeWithCross:
            endMove = CallFunc::create(CC_CALLBACK_0(Gem::prepareToTurnIntoCrossExplosion, this));
            break;
        case GS_Moved:
        default:
            endMove = CallFuncN::create(CC_CALLBACK_1(Gem::onMovementEnd, this));
    }

	if(goBack) {
		movement = Sequence::create((FiniteTimeAction*) wait, (FiniteTimeAction*) move, (FiniteTimeAction*) moveBack, (FiniteTimeAction*) endMove, NULL);
	} else {
		movement = Sequence::create((FiniteTimeAction*) wait, (FiniteTimeAction*) move, (FiniteTimeAction*) endMove, NULL);
	}
    
	state = GS_Moving;
	runAction(movement);
}

#pragma mark -

void Gem::match(MatchType matchType) {
	state = GS_Matched;
    
    if(type == GT_Explosion && matchType != MT_None) {
        switch(matchType) {
            case MT_Vertical:
                type = GT_LineVer; break;
            case MT_Horizontal:
                type = GT_LineHor; break;
            default: type = GT_Colour;
        }
    }
}

#pragma mark - set/get

GemState Gem::getState() {
	return state;
}

void Gem::setState(GemState state) {
    this->state = state;
}

GemType Gem::getType() {
	return type;
}

void Gem::setType(GemType type) {
    this->type = type;
}

GemColour Gem::getGemColour() {
	return colour;
}

#pragma mark - bonus helpers

void Gem::prepareToBeDestroyedByNote() {
    state = GS_AboutToDestroyByNote;
}

void Gem::prepareToTurnIntoBombByNote() {
    state = GS_AboutToTurnIntoBomb;
}

void Gem::prepareToTurnIntoCrossExplosion() {
    state = GS_AboutToExplodeWithCross;
}

void Gem::prepareToExplodeByNote() {
    state = GS_AboutToExplodeByNote;
}

void Gem::setGemColour(GemColour color) {
    string fileName = "";

    switch(color) {
		case GC_Guitar: fileName = "guitar0.png"; break;
		case GC_Keyboard: fileName = "keyboard0.png"; break;
        case GC_Microphone: fileName = "mic0.png"; break;
		case GC_Plectrum: fileName = "plectrum0.png"; break;
        case GC_Question: fileName = "mark0.png"; break;
		case GC_Saxophone: fileName = "sax0.png"; break;
        case GC_Note: fileName = "note0.png"; break;
            
        default: CCLOG("default gem color in reset!");
	}
    if(getTexture() == nullptr) {
        initWithSpriteFrameName(fileName.c_str());
    } else {
        setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName.c_str()));
    }
    
    if(color != GC_Note) {
        this->colour = color;
    }
}

void Gem::resetState() {
	state = GS_Idle;
}

#pragma mark - destruction

void Gem::onDestructionEnd(Object *sender) {
	state = GS_Destroyed;
}

void Gem::destroy(float delay) {
	if(state != GS_Destroying) {
        Action *wait = DelayTime::create(delay);
		Action *enlarge = ScaleTo::create(kDestructionTime / 3.f, 1.2f);
		Action *shrink = ScaleTo::create(kDestructionTime / 3.f * 2.f, 0);
		Action *endDestruction = CallFuncN::create(CC_CALLBACK_1(Gem::onDestructionEnd, this));
		Action *destruction = Sequence::create((FiniteTimeAction*) wait,
                                               (FiniteTimeAction*) enlarge,
											   (FiniteTimeAction*) shrink,
											   (FiniteTimeAction*) endDestruction, NULL);
        
		state = GS_Destroying;
        
		runAction(destruction);
        
        if(this->getParent()) {
            // apply destruction particle effect
            Sprite *noteFog = Sprite::createWithSpriteFrameName("gemDeath0.png");
            noteFog->setPosition(this->getPosition());
            
            Animate *destructionAnim = Animate::create(AnimationCache::getInstance()->animationByName("gemDeath"));
            
            auto cleanFogUp = CallFunc::create([=](){
                noteFog->removeFromParent();
            });
            
            noteFog->runAction(Sequence::create(destructionAnim, cleanFogUp, NULL));

            // we're definetly sure that the gem has a parent
            Node *parent = this->getParent();
            parent->addChild(noteFog, this->getZOrder());
        }
	}
}

void Gem::remove() {
	state = GS_Destroyed;
}

void Gem::setFreeze(int power) {
	float scale;
	if(power == 0)
		scale = 1.f;
	if(power == 1)
		scale = 0.8f;
	if(power == 2)
		scale = 0.6f;
	if(power == 3)
		scale = 0.4f;
	CCLOG("%f", getScale());
	CCLOG("%i", power);
	state = GS_Immovable;
	runAction(ScaleTo::create(kDestructionTime, scale, scale));
	setZOrder(zTile);
}

#pragma mark - internals

Point Gem::convertCoordinatesToPixels(int x, int y) {
	return Point(kTileSize * (x + 0.5), kTileSize * (kFieldHeight - 1 - y + 0.5));  
}

/*
void Gem::makeBomb(GemType type) {
	this->type = type;
	
	String fileName = "bonus";
	switch (type)
	{
	case GT_Cross: fileName.append("Cross");
		break;
	case GT_LineHor: fileName.append("Horizontal");
		break;
	case GT_LineVer: fileName.append("Vertical");
		break;
	default:
		break;
	}
	fileName.append(".png");
	initWithFile(fileName.getCString());
}
*/