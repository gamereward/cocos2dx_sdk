#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
class TransactionScene : public cocos2d::Scene
{
public:
	std::vector<std::string> keys;
	std::string sessionStore;
	virtual bool init();  
	void reloadData();
	void loadMoreItems();
	bool isTouch = false;
    // implement the "static create()" method manually
	CREATE_FUNC(TransactionScene);
private:
	int lastIndex = 0;
	int count = 10;
	bool isLoading = false;
	std::vector<cocos2d::Label*>items;
	cocos2d::ui::ScrollView* scrollView;
	void loadData(int start);
	void onLoadTransactionCallBack(int error, std::string message, std::vector<Grd::Transaction>transactions);
	bool onTouchStart(cocos2d::Touch*touch, cocos2d::Event*e);
	void onTouchFinished(cocos2d::Touch*touch, cocos2d::Event*e);
	std::string formatData(Grd::Transaction&data);
	cocos2d::Label*createItem(std::string st);
	void onBackButtonClick(Ref*sender);
};

#endif // __History_H__
