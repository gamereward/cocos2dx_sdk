#include "SimpleAudioEngine.h"
#include "Progress.h"
USING_NS_CC;
static Progress * currentProgress = nullptr;
void Progress::show(Node*parent){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (currentProgress == nullptr){
		currentProgress = Progress::create();
		currentProgress->setContentSize(Size(visibleSize.width, visibleSize.height));
		//currentProgress->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		parent->addChild(currentProgress, 2);
		currentProgress->createProgress();
	}
	else{
		//currentProgress->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		parent->addChild(currentProgress, 2);
	}
}

void Progress::hide(){
	if (currentProgress != nullptr)
	{
		currentProgress->retain();
		currentProgress->removeFromParent();
	}
}
bool Progress::init(){
	if (currentProgress != nullptr){
		return false;
	}
	
	return true;
}
void Progress::createProgress(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bground = Sprite::create("background.png");
	bground->setContentSize(Size(visibleSize.width, visibleSize.height));
	bground->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(bground,1);
	spinner = Sprite::create();
	spinner->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	// use an array of AnimationFrames instead of SpriteFrames in order to be able to set userInfo
	Vector<AnimationFrame*> animFrames;

	//these are normal frames
	for (int i = 0; i <= 3; i++){
		for (int j = 0; j < 2; j++){
			std::string s = "spinner" + to_string(i) + "-" + to_string(j) + ".png";
			animFrames.pushBack(AnimationFrame::create(SpriteFrame::create(s, Rect(0, 0, 100, 100)), 1, ValueMap()));
		}
	}

	auto animation = Animation::create(animFrames, 100);

	this->addChild(spinner, 2);
	spinner->runAction(Animate::create(animation));
}