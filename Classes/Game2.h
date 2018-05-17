#ifndef __Game2_SCENE_H__
#define __Game2_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
#include "LeaderBoardPanel.h"
#include "HistoryPanel.h"
class Card
{
public:
	Card(){

	}
	Card(int symbol, int suit){
		this->symbol = symbol;
		this->suit = suit;
	};
	Card(Json*result){
		symbol = Json_getInt(result, "symbol", 0);
		suit = Json_getInt(result, "suit", 0);
	};
	~Card(){

	};
	int symbol;
	int suit;

private:

};
class Game2 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
	CREATE_FUNC(Game2);
private:
	bool isWaiting = false;
	cocos2d::ui::EditBox* betInput;
	cocos2d::Label* balanceLabel;
	cocos2d::Label* lblMessage;
	cocos2d::ui::Button* btBetHigh;
	cocos2d::ui::Button* btBetLow;
	cocos2d::Sprite*currentCard;
	cocos2d::Sprite*resultCard;
	LeaderBoardPanel*leaderBoard;
	HistoryPanel*history;
	Card randomCard;
	int islow;
	void showCard(cocos2d::Sprite*sprite, Card&card);
	void hideCard(cocos2d::Sprite*sprite);
	void onGameBetFinished(int error, std::string msg, Json *result);
	void onBackButtonClick(Ref*sender);
	void onBetButtonClick(Ref*sender);
	void onRandomButtonClick(Ref*sender);
};

#endif // __Game2_SCENE_H__
