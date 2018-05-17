#ifndef __History_H__
#define __History_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
typedef std::function<std::string(const Grd::SessionData& data)> FormatSesssionDataFunc;
class HistoryPanel : public cocos2d::ui::ScrollView
{
public:
	std::vector<std::string> keys;
	std::string sessionStore;
	virtual bool init();  
	void reloadData();
	void loadMoreItems();
	void addHistory(Grd::SessionData);
	bool isTouch = false;
	FormatSesssionDataFunc formatDataFunc;
    // implement the "static create()" method manually
	CREATE_FUNC(HistoryPanel);
private:
	int lastIndex = 0;
	int count = 10;
	bool isLoading = false;
	std::vector<cocos2d::Label*>items;
	void loadData(int start);
	void onLoadUserSessionDataCallBack(int error, std::string message, std::vector<Grd::SessionData>History);
	bool onTouchStart(cocos2d::Touch*touch, cocos2d::Event*e);
	void onTouchFinished(cocos2d::Touch*touch, cocos2d::Event*e);
	std::string formatData(Grd::SessionData&data);
	cocos2d::Label*createItem(std::string st);
};

#endif // __History_H__
