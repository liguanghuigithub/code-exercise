#ifndef __PUSHTOUSER_H__
#define __PUSHTOUSER_H__

#include "util/tc_singleton.h"
#include "ServiceDispatcher.h"
#include "DbProxy.h"
#include "RedisProxy.h"

using namespace tars;
using namespace IMCommonPlatform;

class CBusinessLayerPushModel : public TC_Singleton<CBusinessLayerPushModel>
{
public:
    bool PushMsg(const map<string, vector<IMCommonPlatform::PushUserInfo> >& mUserAccessServerInfoMap, const string& msgClassName, const string& msg, bool asyncInvoke, const IMCommonPlatform::ServiceDispatcherPrx& service_dispatcher_prx);
    bool PushNotification(const map<IMCommonPlatform::PushPlatform, vector<IMCommonPlatform::DeviceTokenInfo> >& mPlatformTokenInfoMap, const string& msgClassName, const string& msg, bool asyncInvoke, const IMCommonPlatform::ServiceDispatcherPrx& service_dispatcher_prx);

    bool RemoveSessionNoDisturbUsers(vector<long>& userList,const long fromSessionId, const SessionType sessionType, const string dbname, const DbProxyPrx& db_proxy_prx, const RedisProxyPrx& redis_proxy_prx);
    bool RemoveGlobalNoDisturbPushUserInfo(vector<PushUserInfo>& checkGlobalNoDisturbPushUserInfoList, const string& dbname, const RedisProxyPrx& redis_proxy_prx);
};

#endif
