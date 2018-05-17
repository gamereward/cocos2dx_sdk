#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
class MenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
	CREATE_FUNC(MenuScene);
private:
	cocos2d::ui::EditBox* transferToField = nullptr;
	cocos2d::ui::EditBox* transferValue = nullptr;
	cocos2d::ui::EditBox* otpField=nullptr;
	cocos2d::Label * lblMessage = nullptr;
	cocos2d::Label * lblBalance = nullptr;
	cocos2d::ui::CheckBox * otpToogleEnable=nullptr;
	std::vector<cocos2d::Node*> otpInstructions;
	bool isDestroyed = false;
	void createMenu();
	void createEnableOtp();
	void createTransfer();
	// a selector callback
	void onSettingOtpClick(Ref*sender);
	void onTransferClick(Ref*sender);
	void onGame1Click(Ref*sender);
	void onGame2Click(Ref*sender);
	void onBackButtonClick(Ref*sender);
	void onDoTransferClick(Ref*sender);
	void onSaveOtpSettingClick(Ref*sender);
	void onTransactionClick(Ref*sender);
	void onDownloadGoogleAuthenticatorAppClick(Ref*sender);
	void onToggleOtpChanged(Ref* sender, cocos2d::ui::CheckBox::EventType eventType);
	//Network callback
	void onGetQRCodeAddressCallBack(int error, std::string msg, cocos2d::Sprite * sprite);
	void onRequestOTPCallBack(int error, std::string msg, cocos2d::Sprite * sprite);
	void onTransferCallBack(int error, std::string msg);
	void onSaveOtpCallBack(int error, std::string msg);
};

#endif // __HELLOWORLD_SCENE_H__
