#ifndef __GAME_REWARD__
#define __GAME_REWARD__
#include <vector>
#include <map>
#include <string>
#include "network/HttpClient.h"
#include "spine/Json.h"
#include "BigDecimal.h"
#include "2d/CCSprite.h" 
#include "platform/CCImage.h"
namespace Grd{
	typedef class GrdManager;
	typedef class GrdLeaderBoard;
	typedef class SessionData;
	typedef class Transaction;
	typedef std::function<void(int, std::string)> GrdStringFunc;
	typedef std::function<void(int, std::string,int)> GrdIntFunc;
	typedef std::function<void(int, std::string, cocos2d::Sprite*)> GrdSpriteFunc;
	typedef std::function<void(int, std::string, Json*)> GrdJsonFunc;
	typedef std::function<void(int, std::string, std::vector<GrdLeaderBoard>)> GrdLeaderBoardFunc;
	typedef std::function<void(int, std::string, std::vector<SessionData>)> GrdSessionDataFunc;
	typedef std::function<void(int, std::string, std::vector<Transaction>)> GrdTransactionFunc;
	typedef void (GrdManager::*PTR)(int, Json*, int);
	enum GrdNet
	{
		MAIN_NET,TEST_NET
	};
	enum TransactionStatus
	{
		PENDING_TRANS = 0,
		SUCCESS_TRANS = 1,
		ERROR_TRANS = 2
	};
	enum TransactionType
	{
		BASE_TRANS = 1,
		INTERNAL_TRANS = 2,
		EXTERNAL_TRANS = 3
	};
	inline static long stol(std::string value)
	{
		BigDecimal d = value;
		return d.toLongLong();
	}
	inline static long stod(std::string value)
	{
		BigDecimal d = value;
		return d.toDouble();
	}
	inline static int stoi(std::string value){
		BigDecimal d = value;
		return d.toInt();
	}
	class Transaction{
	public:
		Transaction(){

		}
		Transaction(Json* json){
			if (!json)return;
			Json*field = json->child;
			while (field)
			{
				if (strcmp(field->name, "tx") == 0){
					tx = field->valueString;
				}
				else if (strcmp(field->name, "from") == 0){
						from = field->valueString;
				}
				else if (strcmp(field->name, "to") == 0){
					to = field->valueString;
				}
				else if (strcmp(field->name, "amount") == 0){
					if (field->valueString){
						amount = field->valueString;
					}
					else if (field->valueFloat){
						amount = field->valueFloat;
					}
				}
				else if (strcmp(field->name, "status") == 0){
					status = static_cast< TransactionStatus>(field->valueInt);
				}
				else if (strcmp(field->name, "transtype") == 0){
					transtype = static_cast< TransactionType>(field->valueInt);
				}
				else if (strcmp(field->name, "transdate") == 0){
					if (field->valueInt){
						transdate = field->valueInt;
					}
					else if (field->valueString){
						transdate = Grd::stol( field->valueString);
					}
				}
				field = field->next;
			}
		}
		std::string tx;
		std::string from;
		std::string to;
		BigDecimal amount;
		TransactionStatus status;
		TransactionType transtype;
		long transdate;
	};
	class SessionData{
	public:
		SessionData(){

		}
		SessionData(Json* json){
			if (!json)return;
			Json*field = json->child;
			while (field)
			{
				if (strcmp(field->name , "sessionid")==0){
					sessionid = field->valueInt;
				}
				else if (strcmp(field->name , "sessionstart")==0){
					if (field->valueString){
						sessionstart = Grd::stol(field->valueString);
					}
					else{
						sessionstart = field->valueInt;
					}
				}
				else if (strcmp(field->name , "values")==0){
					Json*child = field->child;
					while (child)
					{
						values.insert(std::pair<std::string, std::string>(child->name, child->valueString));
						child = child->next;
					}
				}
				field = field->next;
			}
		};
		long sessionid;
		long sessionstart;
		std::map<std::string, std::string>values;
	};
	class GrdLeaderBoard
	{
	public:
		GrdLeaderBoard(Json*json){
			if (!json)return;
			Json*field = json->child;
			while(field)
			{
				if (strcmp(field->name, "username") == 0){
					username = field->valueString;
				}
				else if (strcmp(field->name, "score") == 0){
					if (field->valueString){
						score = Grd::stod(field->valueString);
					}
					else if (field->valueFloat){
						score = field->valueFloat;
					}
				}
				else if (strcmp(field->name , "rank")==0){
					rank = field->valueInt;
				}
				field = field->next;
			}
		}
		~GrdLeaderBoard(){

		}
		std::string username;
		double score;
		int rank;
	private:
		
	};
	class GrdUser
	{
	public:
		std::string username;
		std::string email;
		std::string address;
		BigDecimal balance;
		bool otp;
	private:

	};
	
	class GrdManager
	{
	public:
		~GrdManager();	
		static GrdManager* getInstance();
		static void init(std::string appId, std::string apiSecret,GrdNet net);
		GrdUser* getUser();
		bool isLogged();		
		void callServerScript(const std::string &scriptname, const std::string &funcname, std::vector<std::string>params, const GrdJsonFunc& callback);
		void logout(const GrdStringFunc& callback);
		void login(const std::string &username, const std::string &password, std::string &otp, const GrdStringFunc& callback);
		void registerAccount(const std::string &username, const std::string &password, std::string &email, std::string &userdata, const GrdStringFunc& callback);
		void requestResetPassword(const std::string &username, const GrdStringFunc& callback);
		void requestEnableOtp(const GrdSpriteFunc& callback);
		void enableOtp(std::string otp, bool enabled, const GrdStringFunc& callback);
		void updateBalance(const GrdStringFunc& callback);
		void transfer(std::string toAddress, BigDecimal value, std::string otp, const GrdStringFunc& callback);
		void getQRCodeAddress(const GrdSpriteFunc& callback);
		void getLeaderBoard(std::string scoretype, int start, int count, const GrdLeaderBoardFunc&callback);
		void getUserSessionData(std::string store, std::string key, int start, int count, const GrdSessionDataFunc&callback);
		void getUserSessionData(std::string store, std::vector<std::string> keys, int start, int count, const GrdSessionDataFunc&callback);
		void getTransactions(int start, int count,const GrdTransactionFunc&callback);
		void getTransactionCount(const GrdIntFunc&callback);
	private:
		GrdManager(std::string apiId, std::string secret, GrdNet net);
		const std::string LOGIN_ACTION = "login";
		const std::string REGISTER_ACTION = "createaccount";
		const std::string LOGOUT_ACTION = "logout";
		const std::string REQUEST_RESETPASSWORD_ACTION = "requestresetpassword";
		const std::string REQUEST_ENABLE_OTP_ACTION = "requestotp";
		const std::string ENABLE_OTP_ACTION = "enableotp";
		const std::string CALL_SERVERSCRIPT_ACTION = "callserverscript";
		const std::string TRANSFER_ACTION = "transfer";
		const std::string GET_QRCODE_ACTION = "qrcode";
		const std::string GET_LEADERBOARD_ACTION = "getleaderboard";
		const std::string GET_SESSIONDATA_ACTION = "getusersessiondata";
		const std::string GET_TRANSACTION_ACTION = "transactions";
		const std::string COUNT_TRANSACTION_ACTION = "counttransactions";
		const std::string UPDATEBALANCE_ACTION = "accountbalance";
		const std::string MAIN_NET_URL = "https://gamereward.io/appapi/";
		const std::string TEST_NET_URL = "https://test.gamereward.io/appapi/";
		std::string apiId = "";
		std::string apiSecret = "";
		std::string apiUrl = "";
		int call_id = 0;
		GrdUser * user=nullptr;
		std::string userToken;
		std::map<int, GrdStringFunc>mapStringFunc;
		std::map<int, GrdJsonFunc>mapJsonFunc;
		std::map<int, GrdSpriteFunc>mapSpriteFunc;
		std::map<int, GrdLeaderBoardFunc>mapLeaderBoardFunc;
		std::map<int, GrdSessionDataFunc>mapSessionDataFunc;
		std::map<int, GrdTransactionFunc>mapTransactionFunc;
		std::map<int, GrdIntFunc>mapIntFunc;
		std::map<std::string, PTR>networkCallBacks;
		std::string getRequestKey();
		GrdStringFunc popStringFunc(int index);
		GrdSpriteFunc popSpriteFunc(int index);
		GrdJsonFunc popJsonFunc(int index);
		GrdLeaderBoardFunc popLeaderBoardFunc(int index);
		GrdSessionDataFunc popSessionDataFunc(int index);
		GrdTransactionFunc popTransactionFunc(int index);
		GrdIntFunc popIntFunc(int index);
		void putStringFunc(const GrdStringFunc &func);
		void putSpriteFunc(const GrdSpriteFunc&func);
		void putJsonFunc(const GrdJsonFunc&func);
		void putLeaderBoardFunc(const GrdLeaderBoardFunc&func);
		void putSessionDataFunc(const GrdSessionDataFunc&func);
		void putTransactionFunc(const GrdTransactionFunc&func);
		void putIntFunc(const GrdIntFunc&func);
		void registerCallBack(const std::string&action, PTR callback);
		void requestHttp(const std::string &action, std::map<std::string, std::string>*params, bool isGet);
		void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
		void onLoginCallBack(int error, Json* data, int callbackId);
		void onLogoutCallBack(int error, Json* data, int callbackId);
		void onRegisterCallBack(int error, Json* data, int callbackId);
		void onRequestResetPasswordCallBack(int error, Json* data, int callbackId);
		void onCallServerScriptCallBack(int error, Json* data, int callbackId);
		void onRequestEnableOtpCallBack(int error, Json* data, int callbackId);
		void onEnableOtpCallBack(int error, Json* data, int callbackId);
		void onTransferCallBack(int error, Json* data, int callbackId);
		void onQRCodeCallBack(int error, Json* data, int callbackId);
		void onGetLoaderBoardCallBack(int error, Json* data, int callbackId);
		void onGetSessionDataCallBack(int error, Json* data, int callbackId);
		void onGetTransactionCallBack(int error, Json* data, int callbackId);
		void onGetTransactionCountCallBack(int error, Json* data, int callbackId);
		void onUpdateBalanceCallBack(int error, Json* data, int callbackId);
	};


}
#endif