#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "Game2.h"
#include "Progress.h"
#include "MenuScene.h"
USING_NS_CC;
const int CARD_WIDTH = 150;
const int CARD_HEIGHT = 210;
Scene* Game2::createScene()
{
	return Game2::create();
}

// on "init" you need to initialize your instance
bool Game2::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto rightBarSize = Size(visibleSize.width / 4, (visibleSize.height - 160) / 2);
	auto lineHeight = visibleSize.height / 10;
	int margin = visibleSize.width/8;
	auto label = cocos2d::Label::createWithTTF("HIGH-LOW GAME", "fonts/Marker Felt.ttf", 32);
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


	this->btBetHigh = ui::Button::create("button.png", "button_selected.png");
	this->btBetHigh->setPosition(Vec2(origin.x + (visibleSize.width - rightBarSize.width) / 2, origin.y + visibleSize.height - margin));
	this->btBetHigh->addClickEventListener(CC_CALLBACK_1(Game2::onBetButtonClick, this));
	float scale = lineHeight / this->btBetHigh->getContentSize().height;
	this->btBetHigh->setScale(scale);
	this->btBetHigh->setTitleFontSize(16);
	this->btBetHigh->setTag(0);// Not islow
	this->addChild(btBetHigh, 1);

	this->btBetLow = ui::Button::create("button.png", "button_selected.png");
	this->btBetLow->setPosition(Vec2(origin.x + (visibleSize.width - rightBarSize.width) / 2 , origin.y + margin));
	scale = lineHeight / this->btBetLow->getContentSize().height;
	this->btBetLow->setScale(scale);
	this->btBetLow->setTitleFontSize(16);
	this->btBetLow->addClickEventListener(CC_CALLBACK_1(Game2::onBetButtonClick, this));
	this->btBetLow->setTag(1);//Islow
	this->addChild(btBetLow, 1);

	this->currentCard = Sprite::create("cards/cardback.png");
	this->currentCard->setPosition(Vec2(origin.x + margin, origin.y + visibleSize.height / 2));
	this->currentCard->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
	this->addChild(currentCard, 1);

	this->resultCard = Sprite::create("cards/cardback.png");
	this->resultCard->setPosition(Vec2(origin.x - margin + visibleSize.width - rightBarSize.width, origin.y + visibleSize.height / 2));
	this->resultCard->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
	this->addChild(resultCard, 1);

	auto btnRandom = ui::Button::create("button.png", "button_selected.png");
	btnRandom->setTitleText("Next");
	btnRandom->setScale(scale);
	btnRandom->setPosition(Vec2(origin.x + margin, origin.y + visibleSize.height / 2 - CARD_HEIGHT / 2 - 50));
	btnRandom->addClickEventListener(CC_CALLBACK_1(Game2::onRandomButtonClick, this));
	this->addChild(btnRandom);

	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + (visibleSize.width - rightBarSize.width) / 2, origin.y + visibleSize.height / 2));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}

	label = cocos2d::Label::createWithTTF("LEADER BOARD", "fonts/Marker Felt.ttf", 28);
	label->setPosition(origin.x + visibleSize.width - rightBarSize.width / 2, origin.y + 40 + 2 * rightBarSize.height);
	this->addChild(label);
	this->leaderBoard = LeaderBoardPanel::create();
	this->leaderBoard->setBackGroundColor(Color3B(0, 100, 150));
	this->leaderBoard->setSize(rightBarSize);
	this->leaderBoard->setPosition(Vec2(origin.x + visibleSize.width - rightBarSize.width - 5, origin.y + 32 + rightBarSize.height));
	this->addChild(this->leaderBoard);
	this->leaderBoard->scoretype = "lowhighgame_score";
	this->leaderBoard->reloadData();

	label = cocos2d::Label::createWithTTF("HISTORY", "fonts/Marker Felt.ttf", 28);
	label->setPosition(origin.x + visibleSize.width - rightBarSize.width / 2, origin.y + 26 + rightBarSize.height);
	this->addChild(label);
	this->history = HistoryPanel::create();
	this->history->setBackGroundColor(Color3B(0, 100, 150));
	this->history->setSize(rightBarSize);
	this->history->setPosition(Vec2(origin.x + visibleSize.width - rightBarSize.width - 5, origin.y + 10));
	this->history->formatDataFunc = [=](const Grd::SessionData&data){
		char buff[20];
		time_t time = data.sessionstart;
		strftime(buff, 20, "%m-%d %H:%M:%S", localtime(&time));
		std::string st = buff;
		std::map<std::string, std::string>mapValues = data.values;
		std::map<std::string, std::string>::iterator it = mapValues.find("result");
		if (it != mapValues.end()){
			std::string values = it->second;
			Json*json = Json_create(values.c_str());//the data server save as array [islow,your_card_symbol,random_symbol,money]
			std::string cards[] = { "J", "Q", "K", "A" };
			if (json){
				json = json->child;
				int islow = json->valueInt;
				json = json->next;
				int yourcard = json->valueInt;
				json = json->next;
				int randomcard = json->valueInt;
				json = json->next;
				BigDecimal d = 0;
				if (json){
					if (json->valueString){
						d = json->valueString;
					}
					else{
						d = json->valueFloat;
					}
				}
				st += "CARD:" + (yourcard <= 10 ? to_string(yourcard) : cards[yourcard - 11]);
				st += "-SELECT:" + (std::string)(islow > 0 ? "LOW" : "HIGH");
				st += "\r\nREUSLT:" + (randomcard <= 10 ? to_string(randomcard) : cards[randomcard - 11]) + "-";
				if (d > 0){
					st += "WIN:" + d.toString();
				}
				else if (d < 0){
					st += "LOSE:" + d.toString();
				}
				else{
					st += "DRAW";
				}
			}
		}
		return st;
	};
	this->history->sessionStore = "LOWHIGHGAME";
	this->history->keys.push_back("result");
	this->addChild(this->history);
	this->history->reloadData();


	auto btnCancel = ui::Button::create("button.png", "button_selected.png");
	if (btnCancel != nullptr)
	{
		btnCancel->setPosition(Vec2(origin.x + margin, origin.y + 50));
		btnRandom->setScale(scale);
		btnCancel->setTitleText("Back");
		btnCancel->setTitleFontSize(18);
		btnCancel->addClickEventListener(CC_CALLBACK_1(Game2::onBackButtonClick, this));
		this->addChild(btnCancel, 1);
	}

	this->onRandomButtonClick(btnRandom);

	return true;
}
void Game2::onBackButtonClick(Ref*sender){
	MenuScene*menu = MenuScene::create();
	Director::getInstance()->replaceScene(menu);
}
void Game2::onBetButtonClick(Ref*sender){
	if (isWaiting)return;
	isWaiting = true;
	this->hideCard(this->resultCard);
	ui::Button*btn = dynamic_cast<ui::Button*>(sender);
	islow = btn->getTag();
	std::vector<std::string>params;
	params.push_back(to_string(islow));//Is Low
	params.push_back(to_string(this->randomCard.symbol));//Symbol
	params.push_back(betInput->getText());//Bet
	Grd::GrdManager::getInstance()->callServerScript("testscript", "lowhighgame", params, CC_CALLBACK_3(Game2::onGameBetFinished, this));
}
void Game2::onGameBetFinished(int error, std::string msg, Json *result){
	if (error == 0){
		Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
		result = result->child; //Position 0
		int successed = result->valueInt;
		if (successed == 0){
			result = result->next;
			Card card = result;//Position 1
			result = result->next;
			BigDecimal money = "0";
			if (result->valueString){
				money = result->valueString;
			}
			else if (result->valueFloat){
				money=result->valueFloat;//Position 2 money
			}
			this->balanceLabel->setString(user->balance.toString());
			this->showCard(this->resultCard, card);
			if (money > 0){
				this->lblMessage->setTextColor(Color4B(0, 255, 0, 255));
				this->lblMessage->setString("CONGRATULATIONS!\r\nYOU WIN:" + money.toString() + " GRD");
			}
			else if (money < 0){
				this->lblMessage->setTextColor(Color4B(255, 255, 0, 255));
				this->lblMessage->setString("NOT LUCKY!\r\nLOSE:" + money.toString() + " GRD");
			}
			else {
				this->lblMessage->setTextColor(Color4B(0, 255, 0, 255));
				this->lblMessage->setString("DRAW");
			}
			Grd::SessionData session;
			session.sessionstart = time(nullptr);
			session.values.insert(std::pair<std::string, std::string>("result","["+to_string(islow)+","+to_string(randomCard.symbol)+","+to_string(card.symbol)+","+money.toString()+"]"));
			this->history->addHistory(session);
		}
		else{
			this->lblMessage->setString(result->valueString);//Message from script
			this->lblMessage->setTextColor(Color4B(255, 255, 0, 255));
		}
	}
	else{
		this->lblMessage->setString(msg);
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
	}
	isWaiting = false;
}

void Game2::onRandomButtonClick(Ref*sender){
	this->randomCard.symbol = random<int>(3, 13);
	this->randomCard.suit = random<int>(0, 3);
	this->showCard(this->currentCard, this->randomCard);
	BigDecimal bet = this->betInput->getText();
	BigDecimal rate = bet*(14 - this->randomCard.symbol) / (this->randomCard.symbol - 2);
	rate.round(2);
	this->btBetLow->setTitleText("LOW RATE :\n" + rate.toString() + " / " + bet.toString());
	this->btBetLow->setTitleFontSize(16);
	rate = bet* (this->randomCard.symbol - 2) / (14 - this->randomCard.symbol);
	rate.round(2);
	this->btBetHigh->setTitleText("HIGH RATE:\n" + rate.toString() + "/" + bet.toString());
	this->btBetHigh->setTitleFontSize(16);
}
void Game2::showCard(Sprite*sprite, Card& card){
	int symbol = card.symbol;
	int suit = card.suit;
	std::string suites[] = { "clubs", "spades", "diamonds", "hearts" };
	std::string cardImage = "cards/" + to_string(symbol) + "_of_" + suites[suit] + (symbol > 10 && symbol != 14 ? "2" : "") + ".png";
	sprite->setTexture(cardImage);
	sprite->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
}
void Game2::hideCard(Sprite*sprite){
	sprite->setTexture("cards/cardback.png");
	sprite->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
}