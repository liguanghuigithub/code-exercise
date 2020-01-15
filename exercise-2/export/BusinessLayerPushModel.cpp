#include "BusinessLayerPushModel.h"
#include "util/tc_common.h"
#include "ADLogger.h"

bool CBusinessLayerPushModel::PushMsg(const map<string, vector<IMCommonPlatform::PushUserInfo> >& mUserAccessServerInfoMap, const string& msgClassName, const string& msg, bool asyncInvoke, const IMCommonPlatform::ServiceDispatcherPrx& service_dispatcher_prx)
{
    if(mUserAccessServerInfoMap.empty())
    {
        TLOGINFO("push object is null" << endl);
        return true;
    }
    if(msgClassName.empty() || msg.empty())
    {
        TLOGINFO("push msg is null or class name is null");
        return false;
    }

    PushToSvcDispReq pushReq;
    pushReq.mutable_mUserAccessServerInfoMap() = mUserAccessServerInfoMap;
    pushReq.set_msgClassName(msgClassName);
    pushReq.set_msg(msg);

    if(!service_dispatcher_prx)
    {
        TLOGWARN("SvcDispatcherPushReq failed, Msg Type:" << msgClassName);
        return false;
    }

    if(asyncInvoke)
    {
        service_dispatcher_prx->async_SvcDispatcherPushReq(NULL, pushReq);
        return true;
    }
    else
    {
        return (0 == service_dispatcher_prx->SvcDispatcherPushReq(pushReq));
    }
}

bool CBusinessLayerPushModel::PushNotification(const map<IMCommonPlatform::PushPlatform, vector<IMCommonPlatform::DeviceTokenInfo> >& mPlatformTokenInfoMap, const string& msgClassName, const string& msg, bool asyncInvoke, const IMCommonPlatform::ServiceDispatcherPrx& service_dispatcher_prx)
{
    if(mPlatformTokenInfoMap.empty())
    {
        TLOGINFO("push notification object is null" << endl);
        return true;
    }
    TLOGDEBUG("-----------PushNotificationINFO----------" << endl);
    for(auto it = mPlatformTokenInfoMap.begin(); it != mPlatformTokenInfoMap.end(); ++it)
    {
        TLOGDEBUG("PushPlatform:" << it->first << "-------" << endl);
        for(size_t i = 0; i != it->second.size(); ++i)
        {
            auto info = it->second[i];
            TLOGDEBUG(info.imid() << "." << int(info.clientType()) << (i == it->second.size() -1 ? "\n" : ", "));
        }
    }
    if(msgClassName.empty() || msg.empty())
    {
        TLOGINFO("push msg is null or class name is null");
        return false;
    }

    PushNotificationReq pushNotificationReq;
    pushNotificationReq.mutable_mPlatformTokenInfoMap() = mPlatformTokenInfoMap;
    pushNotificationReq.set_msgClassName(msgClassName);
    pushNotificationReq.set_msg(msg);

    if(!service_dispatcher_prx)
    {
        TLOGWARN("SDNotificationPushReq failed, Msg Type:" << msgClassName);
        return false;
    }

    if(asyncInvoke)
    {
        service_dispatcher_prx->async_SDNotificationPushReq(NULL, pushNotificationReq);
        return true;
    }
    else
    {
        return (0 == service_dispatcher_prx->SDNotificationPushReq(pushNotificationReq));
    }
}

bool CBusinessLayerPushModel::RemoveSessionNoDisturbUsers(vector<long>& userList,const long fromSessionId, const SessionType sessionType, const string dbname, const DbProxyPrx& db_proxy_prx, const RedisProxyPrx& redis_proxy_prx)
{
    return true;
}

bool CBusinessLayerPushModel::RemoveGlobalNoDisturbPushUserInfo(vector<PushUserInfo>& checkGlobalNoDisturbPushUserInfoList, const string& dbname, const RedisProxyPrx& redis_proxy_prx)
{
    return true;
}
