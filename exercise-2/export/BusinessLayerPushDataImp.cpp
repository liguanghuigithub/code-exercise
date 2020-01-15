#include "BusinessLayerPushDataImp.h"
#include "BasicBusinessComponent.h"
#include "BusinessLayerPushModel.h"
#include "servant/Application.h"
#include "util.h"

using namespace std;

IMCommonPlatform::UserOnlineStatusPrx user_onlineStatus_prx;
IMCommonPlatform::DbProxyPrx db_proxy_prx;
IMCommonPlatform::RedisProxyPrx redis_proxy_prx;
IMCommonPlatform::ServiceDispatcherPrx service_dispatcher_prx;

//////////////////////////////////////////////////////
void BusinessLayerPushDataImp::initialize()
{
    //initialize servant here:
    //...
    Application::getCommunicator()->stringToProxy("IMCommonPlatform.UserOnlineStatusServer.UserOnlineStatusObj" , user_onlineStatus_prx);
    if(!user_onlineStatus_prx)
    {
        TLOGERROR("init connection with IMCommonPlatform.UserOnlineStatusServer.UserOnlineStatusObj failed, exit...." << endl);
         exit(-1);
    }
    TLOGINFO("init connection with " << user_onlineStatus_prx->tars_name() << " success"<< endl);

    Application::getCommunicator()->stringToProxy("IMCommonPlatform.DbProxyServer.DbProxyObj" , db_proxy_prx);
    if(!db_proxy_prx)
    {
        TLOGERROR("init connection with IMCommonPlatform.DbProxyServer.DbProxyObj failed, exit...." << endl);
         exit(-1);
    }

    Application::getCommunicator()->stringToProxy("IMCommonPlatform.RedisProxyServer.RedisProxyObj" , redis_proxy_prx);
    if(!redis_proxy_prx)
    {
        TLOGERROR("init connection with IMCommonPlatform.RedisProxyServer.RedisProxyObj failed, exit...." << endl);
         exit(-1);
    }
    TLOGINFO("init connection with " << redis_proxy_prx->tars_name() << " success"<< endl);

    Application::getCommunicator()->stringToProxy("IMCommonPlatform.ServiceDispatchServer.ServiceDispatcherObj" , service_dispatcher_prx);
    if(!service_dispatcher_prx)
    {
        TLOGERROR("init connection with IMCommonPlatform.ServiceDispatchServer.ServiceDispatcherObj failed, exit...." << endl);
        exit(-1);
    }
    TLOGINFO("init connection with " << service_dispatcher_prx->tars_name() << " success"<< endl);

    TLOGINFO("BusinessLayerPushDataServer.BusinessLayerPushDataObj init success" << endl);
}

//////////////////////////////////////////////////////
void BusinessLayerPushDataImp::destroy()
{
    //destroy servant here:
    //...
}

tars::Int32 BusinessLayerPushDataImp::PushData(const IMCommonPlatform::BusinessLayerPushDataReq & req,IMCommonPlatform::BusinessLayerPushDataRes &res,tars::TarsCurrentPtr current)
{
    CUseTime use_tm("PushData");
    DEBUG_TARS_MSG(req);
    
    vector<long> toIdList = req.toIdList();
    vector<PushUserInfo> excludeUserInfoList = req.excludeUserInfoList();
    bool pushNotification = req.pushNotification();
    bool bAsyncInvoke = req.bAsyncInvoke();
    string msgClassName = req.msgClassName();
    string msg = req.msg();
    long fromSessionId = req.fromSessionId();
    SessionType sessionType = req.sessionType();

    std::sort(toIdList.begin(), toIdList.end());
    toIdList.erase(std::unique(toIdList.begin(), toIdList.end()), toIdList.end());

    if(msgClassName.empty() || msg.empty() || (fromSessionId != 0 && !isValidEnum<SessionType>(sessionType)) || toIdList.empty())
    {
        TLOGWARN("PushData failed, illegal parameters");
        res.set_resultCode(SvcReqRC_FAILED);
        res.set_reason("illegal parameters");
        return 0;
    }

    QueryAccessServerInfoReq query_access_server_info_req; 
    QueryAccessServerInfoRes query_access_server_info_res;
    query_access_server_info_req.mutable_queryUserIdList() = toIdList;
    if(0 != user_onlineStatus_prx->QueryAccessServerInfo(query_access_server_info_req, query_access_server_info_res) || SvcReqRC_SUCCESS != query_access_server_info_res.resultCode())
    {
        TLOGWARN("PushData failed, QueryAccessServerInfo failed");
        res.set_resultCode(SvcReqRC_FAILED);
        res.set_reason("PushData failed, QueryAccessServerInfo failed");
        return 0;
    }

    map<string, vector<PushUserInfo> > receiver_map = query_access_server_info_res.mUserAccessServerInfoMap();
    vector<long> appOnlineUserList;
    // 去除excludeUserInfoList， 获取APP在线用户
    for(auto it = receiver_map.begin(); it != receiver_map.end();)
    {
        for(auto it2 = it->second.begin(); it2 != it->second.end();)
        {
            if(CLIENT_TYPE_ANDROID == it2->clientType() || CLIENT_TYPE_IOS == it2->clientType())
            {
                appOnlineUserList.push_back(it2->imid());
            }

            if(find(excludeUserInfoList.begin(), excludeUserInfoList.end(), *it2) != excludeUserInfoList.end())
            {
                it2 = it->second.erase(it2);
            }
            else
            {
                ++it2;
            }
        }
        if(it->second.empty())
        {
            it = receiver_map.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if(receiver_map.empty())
    {
        TLOGINFO("no online user, msgClassName:" << msgClassName << endl);
    }
    else
    {
        TLOGINFO("receiver_map.size(): " << receiver_map.size() << endl);
        for(const auto& info: receiver_map)
        {
            TLOGINFO("access_info:" << info.first << endl);
            for(auto & user: info.second)
            {
                TLOGINFO("----user: " << user.toJson() << endl);
            }
        }
        if(false == CBusinessLayerPushModel::getInstance()->PushMsg(receiver_map, msgClassName, msg, bAsyncInvoke, service_dispatcher_prx))
        {
            TLOGWARN("push msg to online user failed" << endl);
            res.set_resultCode(SvcReqRC_FAILED);
            res.set_reason("push msg to online users error");
        }
    }

    if(pushNotification)
    {
        do
        {
            // 计算离线用户
            vector<long> appOfflineUserList;
            std::sort(appOnlineUserList.begin(), appOnlineUserList.end());
            set_difference(toIdList.begin(), toIdList.end(), appOnlineUserList.begin(), appOnlineUserList.end(), back_inserter(appOfflineUserList));

            TLOGINFO("toIdList:" << TC_Common::tostr(toIdList) << endl);
            TLOGINFO("appOnlineUserList:" << TC_Common::tostr(appOnlineUserList) << endl);
            TLOGINFO("appOfflineUserList:" << TC_Common::tostr(appOfflineUserList) << endl);

            if(appOfflineUserList.empty())
            {
                TLOGINFO("appOfflineUserList is empty, no need to push notification" << endl);
                break;
            }
            // get dbname
            string dbname;
            bool bGetDbname = false;
            for(const auto& imid : appOfflineUserList)
            {
                if(imid > 0)
                {
                    if(false == (bGetDbname = CBasicBusinessComponent::getInstance()->GetDbnameByImid(imid, dbname, db_proxy_prx)))
                    {   
                        TLOGWARN("GetDbnameByImid failed. imid:" << imid << endl);
                    }
                    break;
                }
            }
            if(false == bGetDbname)
            {
                break;
            }
            
            // 移除会话免打扰的用户
            if(false == CBusinessLayerPushModel::getInstance()->RemoveSessionNoDisturbUsers(appOfflineUserList, fromSessionId, sessionType, dbname, db_proxy_prx, redis_proxy_prx))
            {
                TLOGWARN("RemoveSessionNoDisturbUsers failed, fromSessionId:" << fromSessionId << ", sessionType:" << sessionType << ", msgClassName:" << msgClassName << endl);
            }
            if(appOfflineUserList.empty())
            {
                TLOGINFO("after RemoveSessionNoDisturbUsers: appOfflineUserList is empty, no need to push notification" << endl);
                break;
            }
            // 查Token
            QueryUserDeviceTokenReq queryUserDeviceTokenReq;
            QueryUserDeviceTokenRes queryUserDeviceTokenRes;
            queryUserDeviceTokenReq.mutable_queryIdList() = appOfflineUserList;
            if(0 != user_onlineStatus_prx->QueryUserDeviceToken(queryUserDeviceTokenReq, queryUserDeviceTokenRes) || SvcReqRC_SUCCESS != queryUserDeviceTokenRes.resultCode())
            {
                TLOGWARN("QueryUserDeviceToken failed" << endl);
                break;
            }
            auto mPlatformTokenInfoMap = queryUserDeviceTokenRes.mPlatformTokenInfoMap();
            if(mPlatformTokenInfoMap.empty())
            {
                TLOGINFO("QueryUserDeviceToken return mPlatformTokenInfoMap is empty" << endl);
                break;
            }
            // 去除excludeUserInfoList, 得到PushUserInfo列表, 等待检查全局免打扰
            vector<PushUserInfo> checkGlobalNoDisturbPushUserInfoList;
            for(auto it = mPlatformTokenInfoMap.begin(); it != mPlatformTokenInfoMap.end();)
            {
                for(auto it2 = it->second.begin(); it2 != it->second.end();)
                {
                    PushUserInfo pushUserInfo;
                    pushUserInfo.set_imid(it2->imid());
                    pushUserInfo.set_clientType(it2->clientType());
                    if(find(excludeUserInfoList.begin(), excludeUserInfoList.end(), pushUserInfo) != excludeUserInfoList.end())
                    {
                        it2 = it->second.erase(it2);
                    }
                    else
                    {
                        checkGlobalNoDisturbPushUserInfoList.push_back(pushUserInfo);
                        ++it2;
                    }
                }
                if(it->second.empty())
                {
                    it = mPlatformTokenInfoMap.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if(checkGlobalNoDisturbPushUserInfoList.empty())
            {
                TLOGINFO("after remove excludeUserInfoList, checkGlobalNoDisturbPushUserInfoList is empty" << endl);
                break;
            }

            // 去查这些imid对应的clientType的全局免打扰状态
            if(false == CBusinessLayerPushModel::getInstance()->RemoveGlobalNoDisturbPushUserInfo(checkGlobalNoDisturbPushUserInfoList, dbname, redis_proxy_prx))
            {
                TLOGWARN("checkGlobalNoDisturbPushUserInfoList failed, fromSessionId:" << fromSessionId << ", sessionType:" << sessionType << ", msgClassName:" << msgClassName << endl);
            }
            // 剩下的是没有设置免打扰的,再找他们登录的接入层，进行推送
            TLOGDEBUG("-------------print need push notification users-------------" << endl);
            for(auto it = mPlatformTokenInfoMap.begin(); it != mPlatformTokenInfoMap.end();)
            {
                for(auto it2 = it->second.begin(); it2 != it->second.end();)
                {
                    PushUserInfo pushUserInfo;
                    pushUserInfo.set_imid(it2->imid());
                    pushUserInfo.set_clientType(it2->clientType());
                    if(find(checkGlobalNoDisturbPushUserInfoList.begin(), checkGlobalNoDisturbPushUserInfoList.end(), pushUserInfo) == checkGlobalNoDisturbPushUserInfoList.end())
                    {
                        it2 = it->second.erase(it2);
                    }
                    else
                    {
                        TLOGDEBUG("push notification----, imid:" << pushUserInfo.imid() << ", clientType:" << pushUserInfo.clientType() << endl);
                        ++it2;
                    }
                }
                if(it->second.empty())
                {
                    it = mPlatformTokenInfoMap.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            // 剩下的mPlatformTokenInfoMap 是真正需要推送的了
            /*
            if(false == CBusinessLayerPushModel::getInstance()->PushNotification(mPlatformTokenInfoMap, msgClassName, msg, true, service_dispatcher_prx))
            {
                TLOGWARN("push msg to online user failed, msgClassName:" << msgClassName << endl);
            }
            */
        }while(0);
    }

    if(false == res.has_resultCode())
    {   
        res.set_resultCode(SvcReqRC_SUCCESS);
        res.set_reason("PushMsg OK");
    }
    return 0;
}
