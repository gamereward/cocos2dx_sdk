#include "GrdManager.h"
#include "md5.h"
#include "base\base64.h"
#include <chrono>

namespace Grd{
	static inline cocos2d::Sprite* createSpriteWithBase64(const std::string& data)
	{
		if (data.length() == 0)
			return nullptr;
		int len = 0;
		unsigned char *buffer;

		len = cocos2d::base64Decode((unsigned char*)data.c_str(), (unsigned int)data.length(), &buffer);

		cocos2d::Image *img = new cocos2d::Image();
		bool ok = img->initWithImageData(buffer, len);

		if (!ok) {
			return nullptr;
		}
		cocos2d::Texture2D* texture = new cocos2d::Texture2D();
		texture->initWithImage(img);
		cocos2d::Sprite* sprite = new cocos2d::Sprite();
		if (sprite && sprite->initWithTexture(texture)){
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	static GrdManager*instance = nullptr;
	void GrdManager::init(std::string apiId, std::string secret,GrdNet net){
		if (instance != nullptr){
			CC_SAFE_DELETE(instance);
		}
		instance = new GrdManager(apiId, secret, net);

	}
	Grd::GrdManager*Grd::GrdManager::getInstance(){
		return instance;
	}
	Grd::GrdManager::GrdManager(std::string appId, std::string secret, GrdNet net)
	{
		this->apiId = appId;
		this->apiSecret = secret;
		this->apiUrl = (net == MAIN_NET ? MAIN_NET_URL : TEST_NET_URL);
		this->registerCallBack(LOGIN_ACTION, &Grd::GrdManager::onLoginCallBack);
		this->registerCallBack(REGISTER_ACTION, &Grd::GrdManager::onRegisterCallBack);
		this->registerCallBack(LOGOUT_ACTION, &Grd::GrdManager::onLogoutCallBack);
		this->registerCallBack(REQUEST_RESETPASSWORD_ACTION, &Grd::GrdManager::onRequestResetPasswordCallBack);
		this->registerCallBack(RESETPASSWORD_ACTION, &Grd::GrdManager::onResetPasswordCallBack);
		this->registerCallBack(CALL_SERVERSCRIPT_ACTION, &Grd::GrdManager::onCallServerScriptCallBack);
		this->registerCallBack(REQUEST_ENABLE_OTP_ACTION, &Grd::GrdManager::onRequestEnableOtpCallBack);
		this->registerCallBack(ENABLE_OTP_ACTION, &Grd::GrdManager::onEnableOtpCallBack);
		this->registerCallBack(TRANSFER_ACTION, &Grd::GrdManager::onTransferCallBack);
		this->registerCallBack(GET_QRCODE_ACTION, &Grd::GrdManager::onQRCodeCallBack);
		this->registerCallBack(GET_LEADERBOARD_ACTION, &Grd::GrdManager::onGetLoaderBoardCallBack);
		this->registerCallBack(GET_SESSIONDATA_ACTION, &Grd::GrdManager::onGetSessionDataCallBack);
		this->registerCallBack(GET_TRANSACTION_ACTION, &Grd::GrdManager::onGetTransactionCallBack);
		this->registerCallBack(COUNT_TRANSACTION_ACTION, &Grd::GrdManager::onGetTransactionCountCallBack);
		this->registerCallBack(UPDATEBALANCE_ACTION, &Grd::GrdManager::onUpdateBalanceCallBack);
	}

	Grd::GrdManager::~GrdManager()
	{
		CC_SAFE_DELETE(this->user);
	}
	bool Grd::GrdManager::isLogged(){
		return this->user != nullptr;
	}
	Grd::GrdUser * Grd::GrdManager::getUser(){
		return this->user;
	}
	//private funcions
	void Grd::GrdManager::registerCallBack(const std::string&action, PTR callback)
	{
		networkCallBacks.insert(std::pair<std::string, Grd::PTR>(action, callback));
	}
	Grd::GrdJsonFunc Grd::GrdManager::popJsonFunc(int index){
		std::map<int, GrdJsonFunc>::iterator it = mapJsonFunc.find(index);
		if (it != mapJsonFunc.end()){
			Grd::GrdJsonFunc func = it->second;
			mapJsonFunc.erase(it);
			return func;
		}
		return nullptr;
	}
	Grd::GrdSpriteFunc  Grd::GrdManager::popSpriteFunc(int index){
		std::map<int, GrdSpriteFunc>::iterator it = mapSpriteFunc.find(index);
		if (it != mapSpriteFunc.end()){
			GrdSpriteFunc func = it->second;
			mapSpriteFunc.erase(it);
			return func;
		}
		return nullptr;
	}
	Grd::GrdStringFunc Grd::GrdManager::popStringFunc(int index){
		std::map<int, GrdStringFunc>::iterator it = mapStringFunc.find(index);
		if (it != mapStringFunc.end()){
			GrdStringFunc func = it->second;
			mapStringFunc.erase(it);
			return func;
		}
		return nullptr;
	}
	Grd::GrdLeaderBoardFunc Grd::GrdManager::popLeaderBoardFunc(int index){
		std::map<int, GrdLeaderBoardFunc>::iterator it = mapLeaderBoardFunc.find(index);
		if (it != mapLeaderBoardFunc.end()){
			GrdLeaderBoardFunc func = it->second;
			mapLeaderBoardFunc.erase(it);
			return func;
		}
		return nullptr;
	}

	Grd::GrdSessionDataFunc Grd::GrdManager::popSessionDataFunc(int index){
		std::map<int, GrdSessionDataFunc>::iterator it = mapSessionDataFunc.find(index);
		if (it != mapSessionDataFunc.end()){
			GrdSessionDataFunc func = it->second;
			mapSessionDataFunc.erase(it);
			return func;
		}
		return nullptr;
	}
	Grd::GrdTransactionFunc Grd::GrdManager::popTransactionFunc(int index){
		std::map<int, GrdTransactionFunc>::iterator it = mapTransactionFunc.find(index);
		if (it != mapTransactionFunc.end()){
			GrdTransactionFunc func = it->second;
			mapTransactionFunc.erase(it);
			return func;
		}
		return nullptr;
	}
	Grd::GrdIntFunc Grd::GrdManager::popIntFunc(int index){
		std::map<int, GrdIntFunc>::iterator it = mapIntFunc.find(index);
		if (it != mapIntFunc.end()){
			GrdIntFunc func = it->second;
			mapIntFunc.erase(it);
			return func;
		}
		return nullptr;
	}
	void Grd::GrdManager::putStringFunc(const GrdStringFunc& func){
		this->call_id++;
		mapStringFunc.insert(std::pair<int, GrdStringFunc>(this->call_id, func));
	}
	void Grd::GrdManager::putJsonFunc(const GrdJsonFunc& func){
		this->call_id++;
		mapJsonFunc.insert(std::pair<int, GrdJsonFunc>(this->call_id, func));
	}
	void Grd::GrdManager::putSpriteFunc(const GrdSpriteFunc& func){
		this->call_id++;
		mapSpriteFunc.insert(std::pair<int, GrdSpriteFunc>(this->call_id, func));
	}
	void Grd::GrdManager::putLeaderBoardFunc(const GrdLeaderBoardFunc& func){
		this->call_id++;
		mapLeaderBoardFunc.insert(std::pair<int, GrdLeaderBoardFunc>(this->call_id, func));
	}
	void Grd::GrdManager::putSessionDataFunc(const GrdSessionDataFunc& func){
		this->call_id++;
		mapSessionDataFunc.insert(std::pair<int, GrdSessionDataFunc>(this->call_id, func));
	}
	void Grd::GrdManager::putTransactionFunc(const GrdTransactionFunc& func){
		this->call_id++;
		mapTransactionFunc.insert(std::pair<int, GrdTransactionFunc>(this->call_id, func));
	}
	void Grd::GrdManager::putIntFunc(const GrdIntFunc& func){
		this->call_id++;
		mapIntFunc.insert(std::pair<int, GrdIntFunc>(this->call_id, func));
	}
	std::string Grd::GrdManager::getRequestKey()
	{
		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		long sec = ms.count()/1000;
		int t = (int)sec;
		t = t / 15;
		int len = apiSecret.length() / 20;
		int k = t % 20;
		std::string str = apiSecret.substr(k * len, len);
		str = md5(str+to_string(t));
		return str;
	}
	void Grd::GrdManager::requestHttp(const std::string &action, std::map<std::string, std::string>*params, bool isGet)
	{
		cocos2d::network::HttpRequest * request = new cocos2d::network::HttpRequest();
		std::string data = "api_id=" + apiId + "&api_key=" + this->getRequestKey();
		if (userToken.length() > 0){
			data += "&token=" + userToken;
		}
		if (params != NULL)
		{
			std::map<std::string, std::string>::iterator it;
			for (it = params->begin(); it != params->end(); it++)
			{
				data = data + "&" + it->first + "=" + it->second;
			}
		}
		if (isGet)
		{
			request->setUrl(apiUrl + action + "/" + data);
			request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
		}
		else
		{
			request->setUrl(apiUrl + action);

			request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
			request->setRequestData(data.c_str(), data.length());
		}
		request->setResponseCallback(CC_CALLBACK_2(GrdManager::onHttpRequestCompleted, this));
		std::string tag = action + "," + to_string(this->call_id);
		request->setTag(tag);
		cocos2d::network::HttpClient::getInstance()->send(request);
	}
	void Grd::GrdManager::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){

		std::string pResponse;
		if (200 == response->getResponseCode()){
			std::vector<char>*buffer = response->getResponseData();
			pResponse = std::string(buffer->begin(), buffer->end());
			if (pResponse.length() == 0){
				pResponse = "{\"error\":500,\"message\":\"" + pResponse + "\"}";
			}
		}
		else{
			pResponse = response->getErrorBuffer();
			pResponse = "{\"error\":500,\"message\":\"" + pResponse + "\"}";
		}
		int pos = pResponse.find("{");
		if (pos> 0){
			//remove unicode first characters
			pResponse = pResponse.substr(pos);
		}
		cocos2d::network::HttpRequest * request = response->getHttpRequest();
		std::string tag = request->getTag();
		request->release();
		int index = tag.find(',');
		if (index > 0){
			Json * json = Json_create(pResponse.c_str());
			int error = Json_getInt(json, "error", 100);
			std::string action = tag.substr(0, index).c_str();
			std::string sindex = tag.substr(index + 1);
			index = Grd::stoi(sindex);
			PTR networkFunc = this->networkCallBacks.find(action)->second;
			(this->*networkFunc)(error, json, index);
			Json_dispose(json);

		}

	}
	//public action
	void Grd::GrdManager::callServerScript(const std::string &scriptname, const std::string &funcname, std::vector<std::string>params, const GrdJsonFunc& callback){
		std::map<std::string, std::string>data;
		std::string vars = "[";
		std::vector<std::string>::iterator it;
		for (it = params.begin(); it != params.end(); it++){
			vars += "" + *it + ",";
		}
		if (vars.length() > 0){
			vars = vars.substr(0, vars.length() - 1);
		}
		vars += "]";
		data.insert(std::pair<std::string, std::string>("script", scriptname));
		data.insert(std::pair<std::string, std::string>("fn", funcname));
		data.insert(std::pair<std::string, std::string>("vars", vars));
		this->putJsonFunc(callback);
		this->requestHttp(CALL_SERVERSCRIPT_ACTION, &data, false);
	}

	void Grd::GrdManager::updateBalance(const GrdStringFunc& callback){
		std::map<std::string, std::string>data;
		this->putStringFunc(callback);
		this->requestHttp(UPDATEBALANCE_ACTION, &data, false);
	}
	void Grd::GrdManager::login(const std::string &username, const std::string &password, std::string &otp, const GrdStringFunc& callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("username", username));
		data.insert(std::pair<std::string, std::string>("password", md5(password)));
		data.insert(std::pair<std::string, std::string>("otp", otp));
		this->putStringFunc(callback);
		this->requestHttp(LOGIN_ACTION, &data, false);
	}


	void Grd::GrdManager::registerAccount(const std::string &username, const std::string &password, std::string &email, std::string &userdata, const GrdStringFunc& callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("username", username));
		data.insert(std::pair<std::string, std::string>("password", md5(password)));
		data.insert(std::pair<std::string, std::string>("email", email));
		data.insert(std::pair<std::string, std::string>("userdata", userdata));
		this->putStringFunc(callback);
		this->requestHttp(REGISTER_ACTION, &data, false);
	}

	void Grd::GrdManager::logout(const GrdStringFunc& callback){
		if (this->user != nullptr){
			CC_SAFE_DELETE(this->user);
		}
		this->user = nullptr;
		this->userToken = "";
		std::map<std::string, std::string>data;
		this->putStringFunc(callback);
		this->requestHttp(LOGOUT_ACTION, &data, false);
	}

	void Grd::GrdManager::requestResetPassword(const std::string&email, const GrdStringFunc& callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("email", email));
		this->putStringFunc(callback);
		this->requestHttp(REQUEST_RESETPASSWORD_ACTION, &data, false);
	}

	void Grd::GrdManager::resetPassword(const std::string&token, const std::string&newpassword, const GrdStringFunc& callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("token", token));
		data.insert(std::pair<std::string, std::string>("password", md5(newpassword)));
		this->putStringFunc(callback);
		this->requestHttp(RESETPASSWORD_ACTION, &data, false);
	}
	void Grd::GrdManager::requestEnableOtp(const GrdSpriteFunc& callback){
		std::map<std::string, std::string>data;
		this->putSpriteFunc(callback);
		this->requestHttp(REQUEST_ENABLE_OTP_ACTION, &data, false);
	}
	void Grd::GrdManager::getQRCodeAddress(const GrdSpriteFunc& callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("text", "gamereward:" + this->getUser()->address));
		data.insert(std::pair<std::string, std::string>("type", "1"));
		this->putSpriteFunc(callback);
		this->requestHttp(GET_QRCODE_ACTION, &data, false);
	}
	void Grd::GrdManager::enableOtp(std::string otp, bool enabled, const GrdStringFunc& callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("otp", otp));
		data.insert(std::pair<std::string, std::string>("otpoptions", enabled ? "1" : "0"));
		this->putStringFunc(callback);
		this->requestHttp(ENABLE_OTP_ACTION, &data, false);
	}
	void Grd::GrdManager::transfer(std::string toAddress, BigDecimal value, std::string otp, const GrdStringFunc& callback)
	{
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("to", toAddress));
		data.insert(std::pair<std::string, std::string>("otp", otp));
		data.insert(std::pair<std::string, std::string>("value", value.toString()));
		this->putStringFunc(callback);
		this->requestHttp(TRANSFER_ACTION, &data, false);
	}
	void Grd::GrdManager::getLeaderBoard(std::string scoretype, int start, int count, const GrdLeaderBoardFunc&callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("scoretype", scoretype));
		data.insert(std::pair<std::string, std::string>("start", to_string(start)));
		data.insert(std::pair<std::string, std::string>("count", to_string(count)));
		this->putLeaderBoardFunc(callback);
		this->requestHttp(GET_LEADERBOARD_ACTION, &data, false);
	}
	void Grd::GrdManager::getUserSessionData(std::string store, std::string key, int start, int count, const GrdSessionDataFunc&callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("store", store));
		data.insert(std::pair<std::string, std::string>("keys", key));
		data.insert(std::pair<std::string, std::string>("start", to_string(start)));
		data.insert(std::pair<std::string, std::string>("count", to_string(count)));
		this->putSessionDataFunc(callback);
		this->requestHttp(GET_SESSIONDATA_ACTION, &data, false);
	}
	void Grd::GrdManager::getUserSessionData(std::string store, std::vector<std::string> keys, int start, int count, const GrdSessionDataFunc&callback){
		std::map<std::string, std::string>data;
		std::string st = "";
		for (std::vector<std::string>::iterator key = keys.begin(); key != keys.end(); key++)
		{
			st += "," + *key;
		}
		if (keys.size() > 0){
			st = st.substr(1);
		}
		data.insert(std::pair<std::string, std::string>("store", store));
		data.insert(std::pair<std::string, std::string>("keys", st));
		data.insert(std::pair<std::string, std::string>("start", to_string(start)));
		data.insert(std::pair<std::string, std::string>("count", to_string(count)));
		this->putSessionDataFunc(callback);
		this->requestHttp(GET_SESSIONDATA_ACTION, &data, false);
	}
	void Grd::GrdManager::getTransactionCount(const GrdIntFunc&callback){
		std::map<std::string, std::string>data;		
		this->putIntFunc(callback);
		this->requestHttp(COUNT_TRANSACTION_ACTION, &data, false);
	}
	void Grd::GrdManager::getTransactions(int start, int count, const GrdTransactionFunc&callback){
		std::map<std::string, std::string>data;
		data.insert(std::pair<std::string, std::string>("start", to_string(start)));
		data.insert(std::pair<std::string, std::string>("count", to_string(count)));
		this->putTransactionFunc(callback);
		this->requestHttp(GET_TRANSACTION_ACTION, &data, false);
	}
	//Handler action callbacks
	void Grd::GrdManager::onLoginCallBack(int error, Json* data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message;
		if (error == 0){
			userToken = Json_getString(data, "token", "");
			if (this->user != nullptr){
				CC_SAFE_DELETE(this->user);
			}
			this->user = new Grd::GrdUser();
			this->user->email = Json_getString(data, "email", "");
			this->user->username = Json_getString(data, "username", "");
			this->user->balance = Json_getString(data, "balance", "0");
			this->user->address = Json_getString(data, "address", "0");
			this->user->otp = Json_getInt(data, "otpoptions", 0) != 0;
			message = Json_getString(data, "message", "");
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onRegisterCallBack(int error, Json* data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message = Json_getString(data, "message", "");
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onRequestResetPasswordCallBack(int error, Json* data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message = Json_getString(data, "message", "");
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onResetPasswordCallBack(int error, Json* data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message = Json_getString(data, "message", "");
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onCallServerScriptCallBack(int error, Json* data, int callbackid){
		GrdJsonFunc callback = this->popJsonFunc(callbackid);
		std::string message = "";
		Json*result = nullptr;
		if (error == 0){
			result = Json_getItem(data, "result");
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message, result);
		}
	}
	void Grd::GrdManager::onRequestEnableOtpCallBack(int error, Json* data, int callbackid){
		GrdSpriteFunc callback = this->popSpriteFunc(callbackid);
		cocos2d::Sprite* result = nullptr;
		std::string message = "";
		if (error == 0){
			std::string qrcode = Json_getString(data, "qrcode", "");
			message = Json_getString(data, "secret", "");
			const std::string prev = "data:image/image/png;base64,";
			if (qrcode.length() > prev.length())
			{
				qrcode = qrcode.substr(prev.length());
			}
			result = createSpriteWithBase64(qrcode);
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message, result);
		}
	}
	void Grd::GrdManager::onLogoutCallBack(int error, Json* data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message = Json_getString(data, "message", "");
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onEnableOtpCallBack(int error, Json* data, int callbackid){
		std::string message = Json_getString(data, "message", "");
		GrdStringFunc callback = this->popStringFunc(callbackid);
		if (!error){
			this->user->otp = Json_getInt(data, "otpoptions", 0) != 0;
		}
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onTransferCallBack(int error, Json* data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message = Json_getString(data, "message", "");
		if (!error){
			this->user->balance = Json_getString(data, "balance", "0");
		}
		if (callback != nullptr){
			callback(error, message);
		}
	}
	void Grd::GrdManager::onQRCodeCallBack(int error, Json* data, int callbackid){
		GrdSpriteFunc callback = this->popSpriteFunc(callbackid);
		std::string message = "";
		cocos2d::Sprite* sprite;
		if (error == 0){
			std::string qrcode = Json_getString(data, "qrcode", "");
			const std::string prev = "data:image/image/png;base64,";
			if (qrcode.length() > prev.length())
			{
				qrcode = qrcode.substr(prev.length());
			}
			sprite = createSpriteWithBase64(qrcode);
		}
		if (callback != nullptr){
			callback(error, message, sprite);
		}
	}
	void Grd::GrdManager::onGetLoaderBoardCallBack(int error, Json*data, int callbackid){
		GrdLeaderBoardFunc callback = this->popLeaderBoardFunc(callbackid);
		std::vector<GrdLeaderBoard>leaderBoard;
		int count = 0;
		std::string message = "";
		if (error == 0){
			Json*item = Json_getItem(data, "leaderboard");
			if (item){
				item = item->child;
				while (item){
					GrdLeaderBoard litem = item;
					leaderBoard.push_back(litem);
					item = item->next;
				}
			}
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message, leaderBoard);
		}
	}
	void Grd::GrdManager::onGetSessionDataCallBack(int error, Json*data, int callbackid){
		GrdSessionDataFunc callback = this->popSessionDataFunc(callbackid);
		std::vector<SessionData>sessions;
		int count = 0;
		std::string message = "";
		if (error == 0){
			Json*item = Json_getItem(data, "data");
			if (item){
				item = item->child;
				while (item){
					SessionData sitem = item;
					sessions.push_back(sitem);
					item = item->next;
				}
			}
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message, sessions);
		}
	}

	void Grd::GrdManager::onGetTransactionCountCallBack(int error, Json*data, int callbackid){
		GrdIntFunc callback = this->popIntFunc(callbackid);
		int count = 0;
		std::string message = "";
		if (error == 0){
			Json*item = Json_getItem(data, "count");
			if (item){
				count = item->valueInt;
			}
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message, count);
		}
	}

	void Grd::GrdManager::onGetTransactionCallBack(int error, Json*data, int callbackid){
		GrdTransactionFunc callback = this->popTransactionFunc(callbackid);
		int count = 0;
		std::string message = "";
		std::vector<Transaction>trans;
		if (error == 0){
			Json*item = Json_getItem(data, "transactions");
			if (item){
				item = item->child;
				while (item){
					Transaction sitem = item;
					trans.push_back(sitem);
					item = item->next;
				}
			}
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message, trans);
		}
	}
	void Grd::GrdManager::onUpdateBalanceCallBack(int error, Json*data, int callbackid){
		GrdStringFunc callback = this->popStringFunc(callbackid);
		std::string message = "";
		if (error == 0){
			Json*item = Json_getItem(data, "balance");
			if (item){
				if (item->valueString){
					user->balance = item->valueString;
				}
				else if(item->valueFloat){
					user->balance = item->valueFloat;
				}
			}
		}
		else{
			message = Json_getString(data, "message", "");
		}
		if (callback != nullptr){
			callback(error, message);
		}
	}
}