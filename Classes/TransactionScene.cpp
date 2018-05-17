#include "SimpleAudioEngine.h"
#include "TransactionScene.h"
#include "MenuScene.h"

USING_NS_CC;

bool TransactionScene::init(){
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setPosition(Vec2(origin.x+50, origin.y));
	scrollView->setSize(Size(visibleSize.width-100, visibleSize.height - 100));
	this->addChild(scrollView);
	auto lbl = cocos2d::Label::createWithTTF("TRANSACTIONS", "fonts/Marker Felt.ttf", 32);
	lbl->setPosition(visibleSize.width / 2, visibleSize.height-20);
	this->addChild(lbl);
	///
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan= CC_CALLBACK_2(TransactionScene::onTouchStart, this);
	listener->onTouchEnded = CC_CALLBACK_2(TransactionScene::onTouchFinished, this);
	this->scrollView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->reloadData();

	auto btnBack = cocos2d::ui::Button::create("button.png", "button_selected.png");
	if (btnBack != nullptr)
	{
		float scale = 1.2* (visibleSize.height / 10) / btnBack->getContentSize().height;
		btnBack->setScale(scale);
		btnBack->setPosition(Vec2(origin.x + 2*btnBack->getContentSize().width / 3, visibleSize.height - 50));
		btnBack->setTitleFontSize(10);
		btnBack->setTitleText("BACK");
		btnBack->addClickEventListener(CC_CALLBACK_1(TransactionScene::onBackButtonClick, this));
		this->addChild(btnBack, 1);
	}

	return true;
}
bool TransactionScene::onTouchStart(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		TransactionScene*thisPanel = static_cast<TransactionScene*>(target);
		thisPanel->isTouch = true;
		return true;
	}
	return false;
}
void TransactionScene::onTouchFinished(Touch* touch, Event* event){
	auto target = event->getCurrentTarget();
	TransactionScene*thisPanel = static_cast<TransactionScene*>(target);
	if (thisPanel->isTouch)
	{
		if (scrollView->getScrolledPercentVertical() == 100)
		{
			thisPanel->loadMoreItems();
		}
	}
}
void TransactionScene::loadMoreItems(){
	if (this->isLoading){
		return;
	}
	this->loadData(this->lastIndex);
}
void TransactionScene::reloadData(){
	this->lastIndex = 0;
	this->loadData(this->lastIndex);
}
void TransactionScene::loadData(int start)
{
	if (this->isLoading && start > 0)
	{
		return;
	}
	this->isLoading = true;
	Grd::GrdManager::getInstance()->getTransactions(start, count, CC_CALLBACK_3(TransactionScene::onLoadTransactionCallBack, this));
}
void TransactionScene::onLoadTransactionCallBack(int error, std::string message, std::vector<Grd::Transaction>transactions){
	if (error == 0){
		if (this->lastIndex == 0){
			for (std::vector<Label*>::iterator it = items.begin(); it != items.end(); it++)
			{
				this->removeChild(*it);
			}
			items.clear();
		}
		scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
		auto thisSize = scrollView->getSize();
		auto containerSize = Size(thisSize.width, 36 * (1 + items.size() + transactions.size()));
		scrollView->setInnerContainerSize(containerSize);
		for (std::vector<Grd::Transaction>::iterator it = transactions.begin(); it != transactions.end(); it++){
			std::string value = this->formatData(*it);
			createItem(value);
		}
		this->lastIndex = items.size();
	}
	this->isLoading = false;
}
Label*TransactionScene::createItem(std::string value)
{
	auto thisSize = scrollView->getSize();
	auto label = Label::createWithTTF(value, "fonts/Marker Felt.ttf", 16);
	label->setContentSize(Size(thisSize.width, 36));
	label->setPosition(Vec2(thisSize.width / 2, thisSize.height - items.size() * 36 - 36));
	this->addChild(label);
	items.push_back(label);
	return label;
}
std::string TransactionScene::formatData(Grd::Transaction&data){
	char buff[20];
	time_t time = data.transdate;
	strftime(buff, 20, "%m-%d %H:%M:%S", localtime(&time));
	std::string st = buff;
	st = st + "-from:" + data.from + "-to" + data.to + "-amount:" + data.amount.toString();
	st = st + "-status:" + (data.status == Grd::SUCCESS_TRANS ? "success" : (data.status == Grd::PENDING_TRANS ? "pending" : "error"));
	st = st + "-type:" + (data.transtype == Grd::INTERNAL_TRANS ? "internal" : (data.status == Grd::BASE_TRANS ? "base" : "external"));
	return st;
}
void TransactionScene::onBackButtonClick(Ref*sender){
	if (isLoading)return;
	MenuScene*menu = MenuScene::create();
	Director::getInstance()->replaceScene(menu);
}