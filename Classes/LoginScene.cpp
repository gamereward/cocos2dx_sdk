#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "Progress.h"
USING_NS_CC;

Scene* LoginScene::createScene()
{
	return LoginScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
	//////////////////////////////
	// 1. super init first
	Scene::setColor(cocos2d::Color3B(0, 100, 255));
	if (!Scene::init())
	{
		return false;
	}
	isWating = false;
	const std::string appid = "cc8b8744dbb1353393aac31d371af9a55a67df16";
	const std::string secret = "1679091c5a880faf6fb5e6087eb1b2dc4daa3db355ef2b0e64b472968cb70f0df4be00279ee2e0a53eafdaa94a151e2ccbe3eb2dad4e422a7cba7b261d923784";
	Grd::GrdManager::init(appid, secret, Grd::TEST_NET);
	this->createLogin();
	return true;
}
void LoginScene::createOtp(){
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("TWO FACTOR AUTHENTICATION", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width / 8;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 - 2 * textboxMargin;
	auto y = origin.y + visibleSize.height / 2 + 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);
	label = Label::createWithTTF("OTP Code:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->otpField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->otpField != nullptr)
	{
		this->otpField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->otpField->setFontColor(textColor);
		this->addChild(this->otpField, 1);
	}
	y -= 2 * lineHeight;
	auto btnBack = ui::Button::create("button.png", "button_selected.png");
	if (btnBack != nullptr)
	{
		btnBack->setPosition(Vec2(x + textboxMargin, y));
		float scale = lineHeight / btnBack->getContentSize().height;
		btnBack->setScale(scale);
		btnBack->setTitleText("Back");
		btnBack->setTitleFontSize(16);
		btnBack->addClickEventListener(CC_CALLBACK_1(LoginScene::onBackButtonClick, this));
		this->addChild(btnBack, 1);
	}
	auto btnVerify = ui::Button::create("button.png", "button_selected.png");
	if (btnVerify != nullptr)
	{
		btnVerify->setPosition(Vec2(x + 3 * textboxMargin + textboxMargin / 2, y));
		float scale = lineHeight / btnVerify->getContentSize().height;
		btnVerify->setScale(scale);
		btnVerify->setTitleText("Verify");
		btnVerify->setTitleFontSize(16);
		btnVerify->addClickEventListener(CC_CALLBACK_1(LoginScene::onLoginButtonClick, this));
		this->addChild(btnVerify, 1);
	}

	y -= 1.5*lineHeight;
	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, y));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}
}
void LoginScene::createRegister(){
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("REGISTER USER", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width / 8;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 - 2 * textboxMargin;
	auto y = origin.y + visibleSize.height / 2 + 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);
	label = Label::createWithTTF("Username:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->userNameField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->userNameField != nullptr)
	{
		this->userNameField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->userNameField->setFontColor(textColor);
		this->addChild(this->userNameField, 1);
	}
	y -= lineHeight;
	label = Label::createWithTTF("Password:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->passwordField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->passwordField != nullptr)
	{
		this->passwordField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->passwordField->setFontColor(textColor);
		this->passwordField->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
		this->addChild(this->passwordField, 1);
	}
	y -= lineHeight;
	label = Label::createWithTTF("Email:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->emailField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->emailField != nullptr)
	{
		this->emailField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->emailField->setFontColor(textColor);
		this->addChild(this->emailField, 1);
	}
	y -= 2 * lineHeight;
	auto btnCancel = ui::Button::create("button.png", "button_selected.png", "");
	if (btnCancel != nullptr)
	{
		btnCancel->setPosition(Vec2(x + textboxMargin, y));
		btnCancel->setTitleText("Cancel");
		btnCancel->setTitleFontSize(10);
		float scale = lineHeight / btnCancel->getContentSize().height;
		btnCancel->setScale(scale);
		btnCancel->addClickEventListener(CC_CALLBACK_1(LoginScene::onBackButtonClick, this));
		this->addChild(btnCancel, 1);
	}
	auto btnRegister = ui::Button::create("button.png", "button_selected.png");
	if (btnRegister != nullptr)
	{
		btnRegister->setPosition(Vec2(x + 3 * textboxMargin, y));
		float scale = lineHeight / btnRegister->getContentSize().height;
		btnRegister->setScale(scale);
		btnRegister->setTitleText("Create Account");
		btnRegister->setTitleFontSize(10);
		btnRegister->addClickEventListener(CC_CALLBACK_1(LoginScene::onCreateAccountButtonClick, this));
		this->addChild(btnRegister, 1);
	}

	y -= 1.5*lineHeight;
	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, y));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}
}

void LoginScene::createForgotPassword(){
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("FORGOT PASSWORD", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width / 8;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 - 2 * textboxMargin;
	auto y = origin.y + visibleSize.height / 2 + 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);
	label = Label::createWithTTF("Email:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->emailField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->emailField != nullptr)
	{
		this->emailField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->emailField->setFontColor(textColor);
		this->addChild(this->emailField, 1);
	}
	y -= 2 * lineHeight;
	auto btnCancel = ui::Button::create("button.png", "button_selected.png", "");
	if (btnCancel != nullptr)
	{
		btnCancel->setPosition(Vec2(x + textboxMargin, y));
		btnCancel->setTitleText("Cancel");
		btnCancel->setTitleFontSize(10);
		float scale = lineHeight / btnCancel->getContentSize().height;
		btnCancel->setScale(scale);
		btnCancel->addClickEventListener(CC_CALLBACK_1(LoginScene::onBackButtonClick, this));
		this->addChild(btnCancel, 1);
	}
	auto btnSendEmail = ui::Button::create("button.png", "button_selected.png");
	if (btnSendEmail != nullptr)
	{
		btnSendEmail->setPosition(Vec2(x + 3 * textboxMargin, y));
		float scale = lineHeight / btnSendEmail->getContentSize().height;
		btnSendEmail->setScale(scale);
		btnSendEmail->setTitleText("Send email");
		btnSendEmail->setTitleFontSize(10);
		btnSendEmail->addClickEventListener(CC_CALLBACK_1(LoginScene::onSendEmailResetPasswordClick, this));
		this->addChild(btnSendEmail, 1);
	}

	y -= 1.5*lineHeight;
	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, y));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}
}


void LoginScene::createResetPassword(){
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("RESET PASSWORD", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}

	label = Label::createWithTTF("An email was sent to your email.\r\nPlease check email to get the token code.", "fonts/Marker Felt.ttf", 18);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - 3 * label->getContentSize().height));
		this->addChild(label, 1);
	}

	auto textboxMargin = visibleSize.width / 8;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 - 2 * textboxMargin;
	auto y = origin.y + visibleSize.height / 2 + 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);

	label = Label::createWithTTF("New password:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->passwordField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->passwordField != nullptr)
	{
		this->passwordField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->passwordField->setFontColor(textColor);
		this->addChild(this->passwordField, 1);
	}
	y -= 1.5*lineHeight;
	label = Label::createWithTTF("Token:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}


	this->emailField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->emailField != nullptr)
	{
		this->emailField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->emailField->setFontColor(textColor);
		this->addChild(this->emailField, 1);
	}



	y -= 2 * lineHeight;
	auto btnCancel = ui::Button::create("button.png", "button_selected.png", "");
	if (btnCancel != nullptr)
	{
		btnCancel->setPosition(Vec2(x + textboxMargin, y));
		btnCancel->setTitleText("Cancel");
		btnCancel->setTitleFontSize(10);
		float scale = lineHeight / btnCancel->getContentSize().height;
		btnCancel->setScale(scale);
		btnCancel->addClickEventListener(CC_CALLBACK_1(LoginScene::onBackButtonClick, this));
		this->addChild(btnCancel, 1);
	}
	auto btnResetPassword = ui::Button::create("button.png", "button_selected.png");
	if (btnResetPassword != nullptr)
	{
		btnResetPassword->setPosition(Vec2(x + 3 * textboxMargin, y));
		float scale = lineHeight / btnResetPassword->getContentSize().height;
		btnResetPassword->setScale(scale);
		btnResetPassword->setTitleText("Reset Password");
		btnResetPassword->setTitleFontSize(10);
		btnResetPassword->addClickEventListener(CC_CALLBACK_1(LoginScene::onResetPasswordClick, this));
		this->addChild(btnResetPassword, 1);
	}

	y -= 1.5*lineHeight;
	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, y));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}
}
void LoginScene::createLogin(){
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("LOGIN TO GAME", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width / 6;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 - 2 * textboxMargin;
	auto y = origin.y + visibleSize.height / 2 + 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);
	label = Label::createWithTTF("Username:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->userNameField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, 7 * lineHeight / 8)), "input.png");
	if (this->userNameField != nullptr)
	{
		this->userNameField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->userNameField->setFontColor(textColor);
		this->addChild(this->userNameField, 1);
	}
	y -= lineHeight;
	label = Label::createWithTTF("Password:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->passwordField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(3 * textboxMargin, 7 * lineHeight / 8)), "input.png");
	if (this->passwordField != nullptr)
	{
		this->passwordField->setPosition(Vec2(x + 2 * textboxMargin, y));
		this->passwordField->setFontColor(textColor);
		this->passwordField->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
		this->addChild(this->passwordField, 1);
	}
	y -= 2 * lineHeight;

	auto btnRegister = ui::Button::create("button.png", "button_selected.png");
	if (btnRegister != nullptr)
	{
		btnRegister->setPosition(Vec2(x + textboxMargin + textboxMargin / 3, y));
		float scale = lineHeight / btnRegister->getContentSize().height;
		btnRegister->setScale(scale);
		btnRegister->setTitleText("Register");
		btnRegister->setTitleFontSize(16);
		btnRegister->addClickEventListener(CC_CALLBACK_1(LoginScene::onRegisterButtonClick, this));
		this->addChild(btnRegister, 1);
	}
	auto btnLogin = ui::Button::create("button.png", "button_selected.png");
	if (btnLogin != nullptr)
	{
		btnLogin->setPosition(Vec2(x + 3 * textboxMargin - textboxMargin / 3, y));
		float scale = lineHeight / btnLogin->getContentSize().height;
		btnLogin->setScale(scale);
		btnLogin->setTitleText("Login");
		btnLogin->setTitleFontSize(16);
		btnLogin->addClickEventListener(CC_CALLBACK_1(LoginScene::onLoginButtonClick, this));
		this->addChild(btnLogin, 1);
	}
	y -= 1.5*lineHeight;
	auto btnForgotPassword = ui::Button::create("button.png", "button_selected.png");
	if (btnForgotPassword != nullptr)
	{
		btnForgotPassword->setPosition(Vec2(x + 2 * textboxMargin, y));
		float scale = lineHeight / btnForgotPassword->getContentSize().height;
		btnForgotPassword->setScale(scale);
		btnForgotPassword->setTitleText("Forgot password");
		btnForgotPassword->setTitleFontSize(16);
		btnForgotPassword->addClickEventListener(CC_CALLBACK_1(LoginScene::onForgotPasswordClick, this));
		this->addChild(btnForgotPassword, 1);
	}

	y -= 1.5*lineHeight;
	this->lblMessage = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(visibleSize.width / 2, y));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}
}
/****************** Handler CALLBACK********************/
void LoginScene::onLoginButtonClick(Ref* sender)
{
	if (isWating)
	{
		//Click to 2 times to the login button 
		return;
	}
	isWating = true;
	if (this->userNameField != nullptr){
		this->username = this->userNameField->getText();
		this->password = this->passwordField->getText();
	}
	std::string otp = "";
	if (this->otpField != nullptr)
	{
		otp = this->otpField->getText();
	}
	Progress::show(this);
	Grd::GrdManager::getInstance()->login(username, password, otp, CC_CALLBACK_2(LoginScene::onLoginCallBack, this));
}
void LoginScene::onLoginCallBack(int error, std::string message){
	isWating = false;
	Progress::hide();
	if (error == 0){
		//Success transition to menu
		MenuScene*menu = MenuScene::create();
		Director::getInstance()->replaceScene(menu);
	}
	if (error == 4){
		//Need provide otp
		if (this->otpField == nullptr){
			this->createOtp();
			this->userNameField = nullptr;
		}
		else{
			this->lblMessage->setString(message);
		}
	}
	else{
		this->lblMessage->setString(message);
	}
}
void LoginScene::onRegisterButtonClick(Ref* sender)
{
	this->createRegister();
}
void LoginScene::onBackButtonClick(Ref* sender)
{
	this->createLogin();
	this->otpField = nullptr;
	this->emailField = nullptr;
}
void LoginScene::onCreateAccountButtonClick(Ref* sender)
{
	if (this->userNameField != nullptr){
		this->username = this->userNameField->getText();
		this->password = this->passwordField->getText();
	}
	std::string email = "";
	if (this->emailField != nullptr)
	{
		email = this->emailField->getText();
	}
	std::string userData = "";
	Grd::GrdManager::getInstance()->registerAccount(username, password, email, userData, CC_CALLBACK_2(LoginScene::onCreateAccountCallBack, this));
}

void LoginScene::onCreateAccountCallBack(int error, std::string message){
	if (error == 0){
		this->lblMessage->setTextColor(Color4B(0, 0, 255, 255));
		this->lblMessage->setString("CREATE ACCOUNT SUCCESSFULLY!");
	}
	else{
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->lblMessage->setString(message);
	}
}
void LoginScene::onForgotPasswordClick(cocos2d::Ref*sender){
	this->createForgotPassword();
}

void LoginScene::onSendEmailResetPasswordClick(cocos2d::Ref*sender){
	std::string email = emailField->getText();
	if (email.length() == 0){
		this->lblMessage->setString("The email can not be empty!");
		return;
	}
	Grd::GrdManager::getInstance()->requestResetPassword(email, CC_CALLBACK_2(LoginScene::onSendEmailResetPasswordCallBack, this));
}
void LoginScene::onSendEmailResetPasswordCallBack(int error, std::string message){
	if (error == 0){
		this->createResetPassword();
	}
	else{
		this->lblMessage->setString(message);
	}
}
void LoginScene::onResetPasswordClick(cocos2d::Ref*sender){
	std::string token = emailField->getText();
	std::string password = passwordField->getText();
	if (token.length() == 0){
		this->lblMessage->setString("The token can not be empty!");
		return;
	}
	if (password.length() == 0){
		this->lblMessage->setString("The new password can not be empty!");
		return;
	}
	Grd::GrdManager::getInstance()->resetPassword(token, password, CC_CALLBACK_2(LoginScene::onResetPasswordCallBack, this));
}
void LoginScene::onResetPasswordCallBack(int error, std::string message){
	if (error != 0){
		this->lblMessage->setString(message);
	}
	else{
		this->lblMessage->setString("YOUR PASSWORD HAD BEEN RESET!");
	}
}
void LoginScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
