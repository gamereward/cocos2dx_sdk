#include "SimpleAudioEngine.h"
#include "LeaderBoardPanel.h"
USING_NS_CC;

bool LeaderBoardPanel::init(){
	if (!ScrollView::init())
		return false;
	auto touchListener = EventListenerTouchOneByOne::create();
	///
	touchListener->onTouchBegan = CC_CALLBACK_2(LeaderBoardPanel::onTouchStart, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LeaderBoardPanel::onTouchFinished, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}
bool LeaderBoardPanel::onTouchStart(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		LeaderBoardPanel*thisPanel = static_cast<LeaderBoardPanel*>(target);
		thisPanel->isTouch = true;
		return true;
	}
	return false;
}
void LeaderBoardPanel::onTouchFinished(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	LeaderBoardPanel*thisPanel = static_cast<LeaderBoardPanel*>(target);
	if (thisPanel->isTouch)
	{
		if (thisPanel->getScrolledPercentVertical() == 100)
		{
			thisPanel->loadMoreItems();
		}
	}
}
void LeaderBoardPanel::loadMoreItems(){
	if (this->isLoading){
		return;
	}
	this->loadData(this->lastIndex);
}
void LeaderBoardPanel::reloadData(){
	this->lastIndex = 0;
	this->loadData(this->lastIndex);
}
void LeaderBoardPanel::loadData(int start)
{
	if (this->isLoading && start > 0)
	{
		return;
	}
	this->isLoading = true;
	Grd::GrdManager::getInstance()->getLeaderBoard(this->scoretype, start, count, CC_CALLBACK_3(LeaderBoardPanel::onLoadLeaderBoardCallBack, this));
}
void LeaderBoardPanel::onLoadLeaderBoardCallBack(int error, std::string message, std::vector<Grd::GrdLeaderBoard>leaderBoard){
	if (error == 0){
		if (this->lastIndex == 0){
			for (std::vector<Node*>::iterator it = items.begin(); it != items.end(); it++)
			{
				this->removeChild(*it);
			}
			items.clear();
		}
		this->setDirection(ScrollView::Direction::VERTICAL);
		auto thisSize = this->getSize();
		auto containerSize = Size(thisSize.width, 24 * (1+items.size() + leaderBoard.size()));
		this->setInnerContainerSize(containerSize);
		for (std::vector<Grd::GrdLeaderBoard>::iterator it = leaderBoard.begin(); it != leaderBoard.end(); it++){
			auto label = Label::createWithTTF("#" + to_string(it->rank) + " " + it->username + ":" + to_string(it->score), "fonts/Marker Felt.ttf", 24);
			label->setContentSize(Size(thisSize.width, 24));
			label->setPosition(Vec2(thisSize.width/2, thisSize.height - items.size() * 24 - 24));
			this->addChild(label);
			items.push_back(label);
		}
		this->lastIndex = items.size();
	}
	this->isLoading = false;
}