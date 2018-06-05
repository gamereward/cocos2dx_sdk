#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GrdManager.h"
class LoginScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
	CREATE_FUNC(LoginScene);
private:
	cocos2d::ui::EditBox* userNameField = nullptr;
	cocos2d::ui::EditBox* passwordField = nullptr;
	cocos2d::ui::EditBox* emailField = nullptr;
	cocos2d::ui::EditBox* otpField=nullptr;
	cocos2d::Label * lblMessage;
	std::string username;
	std::string password;
	std::vector<Node*>otpPanel;
	bool isWating = false;
	// a selector callback
	void createLogin();
	void createRegister();
	void createOtp();
	void createForgotPassword();
	void createResetPassword();
	//Button callback
	void onLoginButtonClick(cocos2d::Ref*sender);
	void onRegisterButtonClick(cocos2d::Ref*sender);
	void onBackButtonClick(cocos2d::Ref*sender);
	void onCreateAccountButtonClick(cocos2d::Ref*sender);
	void onForgotPasswordClick(cocos2d::Ref*sender);
	void onSendEmailResetPasswordClick(cocos2d::Ref*sender);
	void onResetPasswordClick(cocos2d::Ref*sender);

	//GRD Callback
	void onLoginCallBack(int error, std::string message);
	void onCreateAccountCallBack(int error, std::string message);
	void onSendEmailResetPasswordCallBack(int error, std::string message);
	void onResetPasswordCallBack(int error, std::string message);



	void menuCloseCallback(cocos2d::Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
