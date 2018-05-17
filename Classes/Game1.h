#ifndef __GAME1_SCENE_H__
#define __GAME1_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
#include "LeaderBoardPanel.h"
#include "HistoryPanel.h"
class Game1 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
	CREATE_FUNC(Game1);
	void onButtonClick(cocos2d::Sprite*sender);
private:
	bool isWaiting = false;
	int cellwidth;
	cocos2d::ui::EditBox* betInput;
	cocos2d::Label* balanceLabel;
	cocos2d::Label* lblMessage;
	LeaderBoardPanel*leaderBoard;
	HistoryPanel*history;
	std::map<int, cocos2d::Sprite*>buttons;
	void onGameBetFinished(int error, std::string msg, Json *result);
	void onBackButtonClick(Ref*sender);
};

#endif // __GAME1_SCENE_H__
