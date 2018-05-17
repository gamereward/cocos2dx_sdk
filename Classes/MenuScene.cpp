#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "Game1.h"
#include "Game2.h"
#include "TransactionScene.h"
USING_NS_CC;

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	Scene::setColor(cocos2d::Color3B(0, 100, 255));
	if (!Scene::init())
	{
		return false;
	}
	isDestroyed = false;
	this->createMenu();
	return true;
}
static std::string getOtpTextStatus(bool  otp){
	if (otp){
		return "Enabled";
	}
	return "Disabled";
}
void MenuScene::createMenu(){
	Grd::GrdUser* user = Grd::GrdManager::getInstance()->getUser();
	Grd::GrdManager::getInstance()->getQRCodeAddress(CC_CALLBACK_3(MenuScene::onGetQRCodeAddressCallBack, this));
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("GAME MENU", "fonts/Marker Felt.ttf", 32);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width / 8;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 -  textboxMargin;
	auto y = origin.y + visibleSize.height -1.5*lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);
	/*USER ACCOUNT*/
	label = Label::createWithTTF("USER ACCOUNT INFORMATION:", "fonts/Marker Felt.ttf", 28);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x + textboxMargin, y));
		this->addChild(label, 1);
	}
	y -= lineHeight;
	label = Label::createWithTTF("Wallet Address:", "fonts/Marker Felt.ttf", 24);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x + textboxMargin, y));
		this->addChild(label, 1);
	}
	label = Label::createWithTTF("Balance:", "fonts/Marker Felt.ttf", 24);
	if (label != nullptr)
	{
		label->setPosition(Vec2(visibleSize.width - textboxMargin, y));
		this->addChild(label, 1);
	}
	y -= 30;
	label = Label::createWithTTF(user->address, "fonts/Marker Felt.ttf", 24);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x + textboxMargin, y));
		this->addChild(label, 1);
	}
	lblBalance = Label::createWithTTF(user->balance.toString(), "fonts/Marker Felt.ttf", 24);
	if (lblBalance != nullptr)
	{
		lblBalance->setPosition(Vec2(visibleSize.width - textboxMargin, y));
		this->addChild(lblBalance, 1);
	}
	y -= 30;
	label = Label::createWithTTF("OTP SECURITY:" + getOtpTextStatus(user->otp), "fonts / Marker Felt.ttf", 24);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x + textboxMargin, y));
		this->addChild(label, 1);
	}
	/*ACTIONS*/
	y -= lineHeight;
	float scale;
	auto btnTransactions = cocos2d::ui::Button::create("button.png", "button_selected.png");
	if (btnTransactions != nullptr)
	{
		btnTransactions->setPosition(Vec2(x, y));
		btnTransactions->setTitleFontSize(10);
		btnTransactions->setTitleText("TRANSACTIONS");
		scale = 1.2*lineHeight / btnTransactions->getContentSize().height;
		btnTransactions->setScale(scale);
		btnTransactions->addClickEventListener(CC_CALLBACK_1(MenuScene::onTransactionClick, this));
		this->addChild(btnTransactions, 1);
	}
	y -= 1.5* lineHeight;
	auto btnChangeOTP = cocos2d::ui::Button::create("button.png", "button_selected.png");
	if (btnChangeOTP != nullptr)
	{
		btnChangeOTP->setPosition(Vec2(x, y));
		btnChangeOTP->setTitleFontSize(10);
		btnChangeOTP->setTitleText("OTP SETTINGS");
		scale = 1.2*lineHeight / btnChangeOTP->getContentSize().height;
		btnChangeOTP->setScale(scale);
		btnChangeOTP->addClickEventListener(CC_CALLBACK_1(MenuScene::onSettingOtpClick, this));
		this->addChild(btnChangeOTP, 1);
	}
	auto btnTransfer = cocos2d::ui::Button::create("button.png", "button_selected.png");
	if (btnTransfer != nullptr)
	{
		btnTransfer->setPosition(Vec2(x + 3 * textboxMargin, y));
		btnTransfer->setTitleFontSize(10);
		btnTransfer->setTitleText("TRANSFER MONEY");
		btnTransfer->setScale(scale);
		btnTransfer->addClickEventListener(CC_CALLBACK_1(MenuScene::onTransferClick, this));
		this->addChild(btnTransfer, 1);
	}
	y -= 1.5* lineHeight;
	auto btnGame1 = cocos2d::ui::Button::create("button.png", "button_selected.png");
	if (btnGame1 != nullptr)
	{
		btnGame1->setPosition(Vec2(x, y));
		btnGame1->setTitleFontSize(10);
		btnGame1->setTitleText("GAME 1");
		btnGame1->setScale(scale);
		btnGame1->addClickEventListener(CC_CALLBACK_1(MenuScene::onGame1Click, this));
		this->addChild(btnGame1, 1);
	}
	auto btnGame2 = cocos2d::ui::Button::create("button.png", "button_selected.png");
	if (btnGame2 != nullptr)
	{
		btnGame2->setPosition(Vec2(x + 3 * textboxMargin, y));
		btnGame2->setScale(scale);
		btnGame2->setTitleFontSize(10);
		btnGame2->setTitleText("GAME 2");
		btnGame2->setContentSize(cocos2d::Size(1.5f*textboxMargin, 2 * lineHeight / 3));
		btnGame2->addClickEventListener(CC_CALLBACK_1(MenuScene::onGame2Click, this));
		this->addChild(btnGame2, 1);
	}
}
void MenuScene::createTransfer(){
	Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("TRANSFER MONEY", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width / 8;
	auto lineHeight = visibleSize.height / 10;
	auto x = origin.x + visibleSize.width / 2 - 3 * textboxMargin;
	auto y = origin.y + visibleSize.height / 2 + 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);

	label = Label::createWithTTF("ACCOUNT BALANCE:", "fonts/Marker Felt.ttf", 24);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->lblBalance = Label::createWithTTF(user->balance.toString(), "fonts/Marker Felt.ttf", 24);
	if (this->lblBalance != nullptr)
	{
		this->lblBalance->setPosition(Vec2(x + 3 * textboxMargin, y));
		this->addChild(this->lblBalance, 1);
	}
	y -= lineHeight;

	label = Label::createWithTTF("Transfer to:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->transferToField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(5 * textboxMargin, lineHeight - 10)), "input.png");
	if (this->transferToField != nullptr)
	{
		this->transferToField->setPosition(Vec2(x + 3 * textboxMargin, y));
		this->transferToField->setFontColor(textColor);
		this->addChild(this->transferToField, 1);
	}
	y -= lineHeight;
	label = Label::createWithTTF("Amount:", "fonts/Marker Felt.ttf", 16);
	if (label != nullptr)
	{
		label->setPosition(Vec2(x, y));
		this->addChild(label, 1);
	}
	this->transferValue = ui::EditBox::create(cocos2d::Size(cocos2d::Size(textboxMargin, lineHeight - 10)), "input.png");
	if (this->transferValue != nullptr)
	{
		this->transferValue->setPosition(Vec2(x + textboxMargin, y));
		this->transferValue->setFontColor(textColor);
		this->addChild(this->transferValue, 1);
	}
	if (user->otp){

		label = Label::createWithTTF("OTP:", "fonts/Marker Felt.ttf", 16);
		if (label != nullptr)
		{
			label->setPosition(Vec2(x + 2 * textboxMargin, y));
			this->addChild(label, 1);
		}
		this->otpField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(textboxMargin, lineHeight - 10)), "input.png");
		if (this->otpField != nullptr)
		{
			this->otpField->setPosition(Vec2(x + 3 * textboxMargin, y));
			this->otpField->setFontColor(textColor);
			this->addChild(this->otpField, 1);
		}
	}

	y -= 2 * lineHeight;
	auto btnCancel = ui::Button::create("button.png", "button_selected.png");
	if (btnCancel != nullptr)
	{
		btnCancel->setPosition(Vec2(x + 2*textboxMargin, y));
		float scale = lineHeight / btnCancel->getContentSize().height;
		btnCancel->setScale(scale);
		btnCancel->setTitleText("Back");
		btnCancel->setTitleFontSize(18);
		btnCancel->addClickEventListener(CC_CALLBACK_1(MenuScene::onBackButtonClick, this));
		this->addChild(btnCancel, 1);
	}
	auto btnAccept = ui::Button::create("button.png", "button_selected.png");
	if (btnAccept != nullptr)
	{
		btnAccept->setPosition(Vec2(x + 3 * textboxMargin + textboxMargin , y));
		float scale = lineHeight / btnAccept->getContentSize().height;
		btnAccept->setScale(scale);
		btnAccept->setTitleText("Send");
		btnAccept->setTitleFontSize(18);
		btnAccept->addClickEventListener(CC_CALLBACK_1(MenuScene::onDoTransferClick, this));
		this->addChild(btnAccept, 1);
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
void MenuScene::createEnableOtp(){
	Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
	this->removeAllChildren();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("OTP SECURITY SETTINGS", "fonts/Marker Felt.ttf", 30);
	if (label != nullptr)
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));
		this->addChild(label, 1);
	}
	auto textboxMargin = visibleSize.width/8;
	auto lineHeight = visibleSize.height/10;
	auto x = origin.x + visibleSize.width / 2 - 3 * textboxMargin;
	auto y = origin.y + visibleSize.height - 2 * lineHeight;
	auto textColor = cocos2d::Color3B(0, 0, 0);

	label = Label::createWithTTF("Otp security:", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(x + textboxMargin, y));
	this->addChild(label, 1);

	this->otpToogleEnable = ui::CheckBox::create("checkbox.png", "checkbox_selected.png");
	this->otpToogleEnable->setPosition(Vec2(x + 4 * textboxMargin, y));
	this->otpToogleEnable->setSelected(user->otp);
	this->otpToogleEnable->addEventListener(CC_CALLBACK_2(MenuScene::onToggleOtpChanged, this));
	this->addChild(this->otpToogleEnable, 1);

	y -= lineHeight;
	label = Label::createWithTTF("OTP Code:", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(x + textboxMargin, y));
	this->addChild(label, 1);

	this->otpField = ui::EditBox::create(cocos2d::Size(cocos2d::Size(2*textboxMargin, lineHeight - 10)), "input.png");
	this->otpField->setPosition(Vec2(x +4 * textboxMargin, y));
	this->otpField->setFontColor(textColor);
	this->addChild(this->otpField, 1);

	y -= lineHeight;
	this->lblMessage = cocos2d::Label::create("", "fonts/Marker Felt.ttf", 16);
	if (this->lblMessage != nullptr)
	{
		this->lblMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, y));
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->addChild(this->lblMessage, 1);
	}
	y -= 5 * lineHeight;
	auto btnBack = ui::Button::create("button.png", "button_selected.png");
	if (btnBack != nullptr)
	{
		float scale = lineHeight / btnBack->getContentSize().height;
		btnBack->setScale(scale);
		btnBack->setPosition(Vec2(x + 2 * textboxMargin, y+lineHeight/2+10));
		btnBack->setTitleText("Back");
		btnBack->setTitleFontSize(18);
		btnBack->addClickEventListener(CC_CALLBACK_1(MenuScene::onBackButtonClick, this));
		this->addChild(btnBack, 1);
	}
	auto btnSave = ui::Button::create("button.png", "button_selected.png");
	if (btnSave != nullptr)
	{
		float scale = lineHeight / btnSave->getContentSize().height;
		btnSave->setScale(scale);
		btnSave->setPosition(Vec2(x + 4 * textboxMargin , y + lineHeight / 2 + 10));
		btnSave->setTitleText("Save");
		btnSave->setTitleFontSize(18);
		btnSave->addClickEventListener(CC_CALLBACK_1(MenuScene::onSaveOtpSettingClick, this));
		this->addChild(btnSave, 1);
	}
}
void MenuScene::onGetQRCodeAddressCallBack(int error, std::string msg, cocos2d::Sprite * sprite){
	if (error == 0){
		if (this == nullptr)return;
		if (this->isDestroyed)return;
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto textboxMargin = visibleSize.width/8;
		auto lineHeight = visibleSize.width/10;
		auto x = origin.x + visibleSize.width / 2 - 3*textboxMargin;
		auto y = origin.y + visibleSize.height - 2 * lineHeight;
		sprite->setPosition(x , y);
		sprite->setContentSize(cocos2d::Size(lineHeight * 2, lineHeight * 2));
		this->addChild(sprite, 1);
	}
}
void MenuScene::onTransferCallBack(int error, std::string msg)
{
	if (error == 0){
		this->lblBalance->setString(Grd::GrdManager::getInstance()->getUser()->balance.toString());
		this->lblMessage->setTextColor(Color4B(0, 0, 255, 255));
		this->lblMessage->setString("Transfer successfully!");
	}
	else
	{
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->lblMessage->setString("Error:" + to_string(error) + ",message:" + msg);
	}
}

void MenuScene::onTransactionClick(Ref*sender){

	isDestroyed = true;
	TransactionScene*transcene = TransactionScene::create();
	Director::getInstance()->replaceScene(transcene);
}
void MenuScene::onSaveOtpSettingClick(Ref*sender){
	Grd::GrdManager::getInstance()->enableOtp(this->otpField->getText(), this->otpToogleEnable->getSelectedState(), CC_CALLBACK_2(MenuScene::onSaveOtpCallBack, this));
}
void MenuScene::onTransferClick(Ref* sender){
	this->createTransfer();
}
void MenuScene::onDoTransferClick(Ref* sender){
	std::string toAddress = this->transferToField->getText();
	std::string value = this->transferValue->getText();
	BigDecimal dvalue = 0;
	if (value != ""){
		dvalue = value;
	}
	std::string otp = "";
	if (this->otpField != nullptr){
		otp = this->otpField->getText();
	}
	Grd::GrdManager::getInstance()->transfer(toAddress, dvalue, otp, CC_CALLBACK_2(MenuScene::onTransferCallBack, this));
}
void MenuScene::onSettingOtpClick(Ref* sender){
	this->createEnableOtp();
}
void MenuScene::onBackButtonClick(Ref*sender){
	this->createMenu();
	this->otpField = nullptr;
	this->transferToField = nullptr;
	this->transferValue = nullptr;
}
void MenuScene::onToggleOtpChanged(Ref* sender, ui::CheckBox::EventType eventType){
	Grd::GrdUser*user = Grd::GrdManager::getInstance()->getUser();
	if (!user->otp){
		if (this->otpToogleEnable->getSelectedState()){
			Grd::GrdManager::getInstance()->requestEnableOtp(CC_CALLBACK_3(MenuScene::onRequestOTPCallBack, this));
		}
		else{
			for (std::vector<Node*>::iterator it = this->otpInstructions.begin(); it != this->otpInstructions.end(); it++){
				this->removeChild(*it);
			}
			otpInstructions.clear();
		}
	}
	else{
		if (!this->otpToogleEnable->getSelectedState()){
			for (std::vector<Node*>::iterator it = this->otpInstructions.begin(); it != this->otpInstructions.end(); it++){
				this->removeChild(*it);
			}
			otpInstructions.clear();
		}
	}
}
void MenuScene::onRequestOTPCallBack(int error, std::string text, Sprite* qrcode){
	if (error == 0){
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		float y = this->otpField->getPosition().y;
		float textboxMargin = visibleSize.width / 6;
		float lineHeight = visibleSize.height / 10;
		y -= 4*lineHeight;

		auto lbl = Label::createWithTTF("DOWNLOAD APP", "fonts/Marker Felt.ttf", 24);
		lbl->setPosition(textboxMargin, y);
		this->addChild(lbl, 1);
		this->otpInstructions.push_back(lbl);

		auto btnDownloadGooglePlay = ui::Button::create("chplay.png");
		btnDownloadGooglePlay->setContentSize(Size(140, 50));
		btnDownloadGooglePlay->setTag(1);
		btnDownloadGooglePlay->addClickEventListener(CC_CALLBACK_1(MenuScene::onDownloadGoogleAuthenticatorAppClick, this));
		btnDownloadGooglePlay->setPosition(Vec2(textboxMargin, y + 36));
		this->addChild(btnDownloadGooglePlay, 1);
		this->otpInstructions.push_back(btnDownloadGooglePlay);

		auto btnDownloadAppstore = ui::Button::create("appstore.png");
		btnDownloadAppstore->setContentSize(Size(140, 50));
		btnDownloadAppstore->setTag(2);
		btnDownloadAppstore->setPosition(Vec2(textboxMargin, y + 80));
		btnDownloadAppstore->addClickEventListener(CC_CALLBACK_1(MenuScene::onDownloadGoogleAuthenticatorAppClick, this));
		this->addChild(btnDownloadAppstore, 1);
		this->otpInstructions.push_back(btnDownloadAppstore);

		auto arrow = Sprite::create("arrow.png");
		arrow->setPosition(Vec2(textboxMargin + 130, y + 20));
		this->addChild(arrow, 1);
		this->otpInstructions.push_back(arrow);

		lbl = Label::createWithTTF("SCAN QRCODE", "fonts/Marker Felt.ttf", 24);
		lbl->setPosition(textboxMargin + 250, y);
		this->addChild(lbl, 1);
		this->otpInstructions.push_back(lbl);

		if (qrcode != nullptr){
			qrcode->setContentSize(Size(150, 150));
			qrcode->setPosition(textboxMargin + 250, y + 100);
			this->addChild(qrcode, 1);
			this->otpInstructions.push_back(qrcode);
		}


		lbl = Label::createWithTTF("OR COPY SECRET", "fonts/Marker Felt.ttf", 24);
		lbl->setPosition(textboxMargin + 470, y);
		this->addChild(lbl, 1);
		this->otpInstructions.push_back(lbl);

		lbl = Label::createWithTTF("", "fonts/Marker Felt.ttf", 14);
		if (lbl != nullptr){
			lbl->setDimensions(200, 0);
			lbl->setPosition(textboxMargin + 500, y + 100);
			lbl->setString(text);
			this->addChild(lbl, 1);
			this->otpInstructions.push_back(lbl);
		}
	}
}
void MenuScene::onSaveOtpCallBack(int error, std::string message){
	if (error == 0){
		std::string st = (Grd::GrdManager::getInstance()->getUser()->otp ? "enabled" : "disabled");
		this->lblMessage->setTextColor(Color4B(0, 0, 255, 255));
		this->lblMessage->setString("You have " + st + " OTP successfully!");
	}
	else{
		this->lblMessage->setTextColor(Color4B(255, 0, 0, 255));
		this->lblMessage->setString("Error" + to_string(error) + ",message:" + message);
	}
}


void MenuScene::onDownloadGoogleAuthenticatorAppClick(Ref*sender){
	auto button = dynamic_cast<ui::Button*>(sender);
	std::string stUrl = "";
	if (button->getTag() == 1){
		stUrl = "https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2&hl=en";
	}
	else{
		stUrl = "https://itunes.apple.com/vn/app/google-authenticator/id388497605?mt=8";
	}
	Application::getInstance()->openURL(stUrl);
}
void MenuScene::onGame1Click(Ref*sender){
	isDestroyed = true;
	Game1*game = Game1::create();
	Director::getInstance()->replaceScene(game);
}

void MenuScene::onGame2Click(Ref*sender){
	isDestroyed = true;
	Game2*game = Game2::create();
	Director::getInstance()->replaceScene(game);
}