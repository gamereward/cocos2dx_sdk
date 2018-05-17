#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "Game1.h"
#include "Progress.h"
#include "MenuScene.h"
USING_NS_CC;

Scene* Game1::createScene()
{
	return Game1::create();
}

// on "init" you need to initialize your instance
bool Game1::init()
{
	//////////////////////////////
	// 1. super init first
	Scene::setColor(cocos2d::Color3B(0, 0, 0));
	if (!Scene::init())
	{
		return false;
	}
	Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto rightBarSize = Size(visibleSize.width/4, (visibleSize.height - 160) / 2);

	int marginLeft = (visibleSize.width - rightBarSize.width) /4;
	int cellMargin = 5;
	auto lineHeight = (visibleSize.width / 10);
	cellwidth = (visibleSize.width - 2 * marginLeft - rightBarSize.width - 3 * cellMargin) / 3;
	Game1*thisGame = this;
	marginLeft = marginLeft + 2*cellwidth / 3;
	int marginTop = 1.5*lineHeight;
	for (int i = 0; i < 9; i++)
	{
		auto btn = Sprite::create("square_normal.png");
		btn->setContentSize(Size(cellwidth, cellwidth));
		btn->setPosition(Vec2(marginLeft + (i % 3)*(cellwidth + cellMargin),visibleSize.height- marginTop - (i / 3)*(cellwidth + cellMargin)));

		auto touchListener = EventListenerTouchOneByOne::create();
		///
		touchListener->onTouchBegan = [=](Touch* touch, Event* event){
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = target->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(locationInNode))
			{
				thisGame->onButtonClick(btn);
				return true;
			}
			return false;
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btn);

		btn->setTag(i + 1);
		this->addChild(btn);
		auto lbl = Label::createWithTTF(to_string(i + 1), "fonts/Marker Felt.ttf", 32);
		lbl->setPosition(cellwidth / 2, cellwidth / 2);
		lbl->setTextColor(Color4B(0, 0, 255, 255));
		btn->addChild(lbl);
		buttons.insert(std::pair<int, Sprite*>(i, btn));
	}
	auto label = cocos2d::Label::createWithTTF("GAME RANDOM 1-9", "fonts/Marker Felt.ttf", 32);
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 30);
	this->addChild(label);

	label = cocos2d::Label::createWithTTF("ACCOUNT BALANCE", "fonts/Marker Felt.ttf", 28);
	label->setPosition(origin.x + visibleSize.width - 130, origin.y + visibleSize.height - 50);
	this->addChild(label);
	balanceLabel = cocos2d::Label::createWithTTF(user->balance.toString(), "fonts/Marker Felt.ttf", 28);
	balanceLabel->setPosition(origin.x + visibleSize.width - 130, origin.y + visibleSize.height - 90);
	this->addChild(balanceLabel);

	label = cocos2d::Label::createWithTTF("BET", "fonts/Marker Felt.ttf", 28);
	label->setPosition(origin.x + 90, origin.y + visibleSize.height - 50);
	this->addChild(label);
	betInput = cocos2d::ui::EditBox::create(Size(120, 60), "input.png");
	betInput->setText("1");
	betInput->setFontColor(Color3B(0, 0, 0));
	betInput->setPosition(Vec2(origin.x + 90, origin.y + visibleSize.height - 90));
	betInput->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	this->addChild(betInput);

	label = cocos2d::Label::createWithTTF("LEADER BOARD", "fonts/Marker Felt.ttf", 28);
	label->setPosition(origin.x + visibleSize.width - 130, origin.y + 40 + 2 * rightBarSize.height);
	this->addChild(label);
	this->leaderBoard = LeaderBoardPanel::create();
	this->leaderBoard->setBackGroundColor(Color3B(0, 100, 150));
	this->addChild(this->leaderBoard);
	this->leaderBoard->setSize(rightBarSize);
	this->leaderBoard->setPosition(Vec2(origin.x + visibleSize.width - rightBarSize.width - 10, origin.y + 32 + rightBarSize.height));
	this->leaderBoard->scoretype = "random9_score";
	this->leaderBoard->reloadData();

	label = cocos2d::Label::createWithTTF("HISTORY", "fonts/Marker Felt.ttf", 28);
	label->setPosition(origin.x + visibleSize.width - 130, origin.y + 26 + rightBarSize.height);
	this->addChild(label);
	this->history = HistoryPanel::create();
	this->history->setBackGroundColor(Color3B(0, 100, 150));
	this->addChild(this->history);
	this->history->setSize(rightBarSize);
	this->history->setPosition(Vec2(origin.x + visibleSize.width - rightBarSize.width - 10, origin.y + 10));
	this->history->sessionStore = "GAME-9";
	this->history->keys.push_back("rand");
	this->history->formatDataFunc = [=](const Grd::SessionData&data){
		char buff[20];
		time_t time = data.sessionstart;
		strftime(buff, 20, "%m-%d %H:%M:%S", localtime(&time));
		std::string st = buff;
		std::string values = data.values.find("rand")->second;
		int i = values.find(",");
		std::string randNumber = "";
		if (i > 0){
			randNumber = values.substr(0, i);
			values = values.substr(i + 1);
		}
		i = values.find(",");
		std::string yourNumber = "";
		if (i > 0){
			yourNumber = values.substr(0, i);
			values = values.substr(i + 1);
		}
		st = st + "-SELECT:" + yourNumber + "-RESULT:" + randNumber + "\r\n";
		BigDecimal d = values;
		if (d > 0){
			st += "WIN:" + d.toString();
		}
		else if (d < 0){
			st += "LOSE:" + d.toString();
		}
		else{
			st += "DRAW";
		}
		return st;
	};
	this->history->reloadData();

	auto btnCancel = ui::Button::create("button.png", "button_selected.png");
	if (btnCancel != nullptr)
	{
		btnCancel->setPosition(Vec2(origin.x + marginLeft/3, origin.y + lineHeight/2+10));
		float scale =0.5*lineHeight / btnCancel->getContentSize().height;
		btnCancel->setScale(scale);
		btnCancel->setTitleText("Back");
		btnCancel->setTitleFontSize(16);
		btnCancel->addClickEventListener(CC_CALLBACK_1(Game1::onBackButtonClick, this));
		this->addChild(btnCancel, 1);
	}


	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y+ visibleSize.height-lineHeight));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}

	return true;
}
void Game1::onBackButtonClick(Ref*sender){
	MenuScene*menu = MenuScene::create();
	Director::getInstance()->replaceScene(menu);
}
void Game1::onButtonClick(Sprite*sender){
	if (isWaiting)return;
	isWaiting = true;
	Sprite*btn = sender;
	for (std::map<int, Sprite*>::iterator it = buttons.begin(); it != buttons.end(); it++){

		it->second->setTexture("square_normal.png");
		it->second->setContentSize(Size(cellwidth, cellwidth));
	}

	std::vector<std::string>params;
	btn->setTexture("square_selected.png");
	btn->setContentSize(Size(cellwidth, cellwidth));
	params.push_back(to_string(btn->getTag()));//Number
	params.push_back(betInput->getText());//Bet
	Grd::GrdManager::getInstance()->callServerScript("testscript", "random9", params, CC_CALLBACK_3(Game1::onGameBetFinished, this));
}
void Game1::onGameBetFinished(int error, std::string msg, Json *result){
	if (error == 0){
		Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
		result = result->child;
		int successed = result->valueInt;
		result = result->next;
		if (successed == 0){
			int randNumber = result->valueInt;//Position 1
			result = result->next;
			int yournumber = result->valueInt; //Position 2
			result = result->next;
			float money = result->valueFloat;//Position 3
			user->balance += money;
			std::map<int, Sprite*>::iterator it1 = buttons.find(yournumber - 1);
			std::map<int, Sprite*>::iterator it2 = buttons.find(randNumber - 1);
			this->balanceLabel->setString(user->balance.toString());
			Grd::SessionData session;
			session.sessionstart = time(nullptr);
			session.values.insert(std::pair<std::string, std::string>("rand", to_string(randNumber) + "," + to_string(yournumber) + "," +to_string( money)));
			if (randNumber == yournumber){
				if (it1 != buttons.end()){
					it1->second->setTexture("square_win.png");
					it1->second->setContentSize(Size(cellwidth, cellwidth));
				}
				this->lblMessage->setTextColor(Color4B(0, 255, 0, 255));
				this->lblMessage->setString("CONGRATULATIONS! YOU WIN:" + to_string(money) + " GRD");
			}
			else{
				if (it2 != buttons.end()){
					it2->second->setTexture("square_lose.png");
					it2->second->setContentSize(Size(cellwidth, cellwidth));
				}
				this->lblMessage->setTextColor(Color4B(255, 255, 0, 255));
				this->lblMessage->setString("NOT LUCKY YET:" + to_string(money) + " GRD");
			}
			this->history->addHistory(session);
			this->leaderBoard->reloadData();
		}
		else{
			this->lblMessage->setString(msg);
			this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		}
	}
	else{
		this->lblMessage->setString(result->valueString);//Message from script
		this->lblMessage->setTextColor(Color4B(255, 255, 0, 255));
	}
	isWaiting = false;
}