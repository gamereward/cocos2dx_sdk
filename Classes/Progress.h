#ifndef __PROGRESS_H__
#define __PROGRESS_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
class Progress : public cocos2d::Node
{
public:
	static void show(Node*parent);
	static void hide();
	virtual bool init();  
	void createProgress();
    // implement the "static create()" method manually
	CREATE_FUNC(Progress);
private:
	cocos2d::Sprite *spinner;
};

#endif // __PROGRESS_H__
