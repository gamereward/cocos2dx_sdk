#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
class LeaderBoardPanel : public cocos2d::ui::ScrollView
{
public:
	std::string scoretype;
	virtual bool init();  
	void reloadData();
	void loadMoreItems();
	bool isTouch = false;
    // implement the "static create()" method manually
	CREATE_FUNC(LeaderBoardPanel);
private:
	int lastIndex = 0;
	int count = 10;
	bool isLoading = false;
	std::vector<Node*>items;
	void loadData(int start);
	void onLoadLeaderBoardCallBack(int error, std::string message, std::vector<Grd::GrdLeaderBoard>leaderBoard);
	bool onTouchStart(cocos2d::Touch*touch, cocos2d::Event*e);
	void onTouchFinished(cocos2d::Touch*touch, cocos2d::Event*e);
};

#endif // __LEADERBOARD_H__
