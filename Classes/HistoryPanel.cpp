#include "SimpleAudioEngine.h"
#include "HistoryPanel.h"
USING_NS_CC;

bool HistoryPanel::init(){
	if (!ScrollView::init())
		return false;
	auto touchListener = EventListenerTouchOneByOne::create();
	///
	touchListener->onTouchBegan = CC_CALLBACK_2(HistoryPanel::onTouchStart, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HistoryPanel::onTouchFinished, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}
bool HistoryPanel::onTouchStart(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		HistoryPanel*thisPanel = static_cast<HistoryPanel*>(target);
		thisPanel->isTouch = true;
		return true;
	}
	return false;
}
void HistoryPanel::onTouchFinished(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	HistoryPanel*thisPanel = static_cast<HistoryPanel*>(target);
	if (thisPanel->isTouch)
	{
		if (thisPanel->getScrolledPercentVertical() == 100)
		{
			thisPanel->loadMoreItems();
		}
	}
}
void HistoryPanel::loadMoreItems(){
	if (this->isLoading){
		return;
	}
	this->loadData(this->lastIndex);
}
void HistoryPanel::reloadData(){
	this->lastIndex = 0;
	this->loadData(this->lastIndex);
}
void HistoryPanel::loadData(int start)
{
	if (this->isLoading && start > 0)
	{
		return;
	}
	this->isLoading = true;
	Grd::GrdManager::getInstance()->getUserSessionData(this->sessionStore,this->keys, start, count, CC_CALLBACK_3(HistoryPanel::onLoadUserSessionDataCallBack, this));
}
void HistoryPanel::onLoadUserSessionDataCallBack(int error, std::string message, std::vector<Grd::SessionData>data){
	if (error == 0){
		if (this->lastIndex == 0){
			for (std::vector<Label*>::iterator it = items.begin(); it != items.end(); it++)
			{
				this->removeChild(*it);
			}
			items.clear();
		}
		this->setDirection(ScrollView::Direction::VERTICAL);
		auto thisSize = this->getSize();
		auto containerSize = Size(thisSize.width, 36 * (1 + items.size() + data.size()));
		this->setInnerContainerSize(containerSize);
		for (std::vector<Grd::SessionData>::iterator it = data.begin(); it != data.end(); it++){
			std::string value = this->formatData(*it);
			createItem(value);
		}
		this->lastIndex = items.size();
	}
	this->isLoading = false;
}
Label*HistoryPanel::createItem(std::string value)
{
	auto thisSize = this->getSize();
	auto label = Label::createWithTTF(value, "fonts/Marker Felt.ttf", 16);
	label->setContentSize(Size(thisSize.width, 36));
	label->setPosition(Vec2(thisSize.width / 2, thisSize.height - items.size() * 36 - 36));
	this->addChild(label);
	items.push_back(label);
	return label;
}
void HistoryPanel::addHistory(Grd::SessionData item){
	std::string st = formatData(item);
	createItem("");
	for (int i = items.size() - 1; i > 1; i--){
		items[i]->setString(items[i - 1]->getString());
	}
	items[0]->setString(st);
}
std::string HistoryPanel::formatData(Grd::SessionData&data){
	if (this->formatDataFunc){
		return this->formatDataFunc(data);
	}
	char buff[20];
	time_t time = data.sessionstart;
	strftime(buff, 20, "%m-%d %H:%M:%S", localtime(&time));
	std::string st = buff;
	st = st + "-";
	for (std::map<std::string, std::string>::iterator it = data.values.begin(); it != data.values.end(); it++){
		st += it->first + "=" + it->second;
	}
	return st;
}