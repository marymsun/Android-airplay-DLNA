/*
 * =====================================================================================
 *
 *       Filename:  com_xindawn_jni_PlatinumJniProxy.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年02月12日 16时31分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xindawn
 *   Organization:  
 *
 * =====================================================================================
 */
#include <jni.h>
#include <stdlib.h>
#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include "StdString.h"
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>

#define closesocket close
#define ioctlsocket ioctl

#define INVALID_SOCKET  -1


#ifdef __cplusplus
extern "C"{
#endif
#include "addon.h"

#ifdef __cplusplus
}
#endif










#define  LOG_TAG    "XinDawn"
#define  LOGI(...)  //__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  //__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


static JavaVM*   	g_vm = NULL;
static jclass 		g_inflectClass = NULL;
static jmethodID 	g_methodID = NULL;
static jmethodID 	g_methodID1 = NULL;
static jmethodID 	g_methodID2 = NULL;
static jmethodID 	g_methodID3 = NULL;
static jmethodID 	g_methodID4 = NULL;
//static jobject     g_activityObj = NULL;


JavaVM *get_jvm()
{

	return g_vm;
}


static const char* const kClassPathName = "com/xindawn/jni/PlatinumJniProxy";

static jint com_xindawn_jni_PlatinumJniProxy_startMediaRender(JNIEnv* env,jobject thiz, jstring friendname,jstring libpath,jstring activecode,int width,int height,int airtunes_port,int airplay_port,int rcvsize, jobject context);
static jint com_xindawn_jni_PlatinumJniProxy_stopMediaRender(JNIEnv *env, jobject thiz);


#define MEDIA_RENDER_CTL_MSG_BASE                       		 (0x100)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_DURATION            (MEDIA_RENDER_CTL_MSG_BASE+0)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_POSITION            (MEDIA_RENDER_CTL_MSG_BASE+1)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_PLAYINGSTATE        (MEDIA_RENDER_CTL_MSG_BASE+2)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_MUTE                (MEDIA_RENDER_CTL_MSG_BASE+3)
#define MEDIA_RENDER_TOCONTRPOINT_SET_CACHEPOSITION             (MEDIA_RENDER_CTL_MSG_BASE+4)
#define MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_CACHE               (MEDIA_RENDER_CTL_MSG_BASE+5)

static long long	totalDuration;
static long		currentPosition;
static int			isPlaying;
static int			isPause;


void UpdateState(int cmd,char *value,char *data)
{
			char hours[3];
			char mins[3];
			char secs[3];

	        switch(cmd)
			{

					case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_DURATION:


						hours[0] = value[0];
						hours[1] = value[1];
						hours[2] = 0;

						mins[0] = value[3];
						mins[1] = value[4];
						mins[2] = 0;

						secs[0] = value[6];
						secs[1] = value[7];
						secs[2] = 0;

						if(totalDuration == 0)
						{
							totalDuration = atoi(hours) * 3600 +
															atoi(mins) * 60 +
															atoi(secs)  ;
							totalDuration *= 1000;
						}


						break;
					case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_POSITION:

						hours[0] = value[0];
						hours[1] = value[1];
						hours[2] = 0;

						mins[0] = value[3];
						mins[1] = value[4];
						mins[2] = 0;

						secs[0] = value[6];
						secs[1] = value[7];
						secs[2] = 0;

						if(isPlaying)
						{
							currentPosition = atoi(hours) * 3600 +
															atoi(mins) * 60 +
															atoi(secs)  ;

							currentPosition *= 1000;
						}

						break;
					case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_PLAYINGSTATE:
						if(!strcmp(value,"TRANSITIONING"))
						{

						}
						else if(!strcmp(value,"PLAYING"))
						{
							isPlaying = 1;
							isPause = 0;


						}
						else if(!strcmp(value,"STOPPED"))
						{
							 if(isPlaying)
							 {
								 isPlaying = 0;
							 }

						}
						else if(!strcmp(value,"PAUSED_PLAYBACK"))
						{
							if(isPlaying)
							{
								isPause = 1;

							}
						}
						break;
					case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_MUTE:
						break;
		             case MEDIA_RENDER_TOCONTRPOINT_SET_CACHEPOSITION:
		            	 break;
		             case MEDIA_RENDER_TOCONTRPOINT_SET_MEDIA_CACHE:
		                 break;

			}

}


static jint com_xindawn_jni_PlatinumJniProxy_responseGenaEvent(JNIEnv *env, jobject thiz,jint cmd,jbyteArray value,jbyteArray data)
{

	 jbyte       *pValue = NULL;
	 jbyte		 *pData  = NULL;
	 char 		 *cValue = NULL;
	 char		 *cData  = NULL;
	 int		 numVlaue  = 0;
	 int		 numData   = 0;



	 pValue   = value ? env->GetByteArrayElements(value, NULL):NULL;
	 numVlaue = value ? env->GetArrayLength(value): 0;
	 pData    = data?env->GetByteArrayElements(data, NULL):NULL;
	 numData =  data ? env->GetArrayLength(data): 0;

	 if(numVlaue > 0)
	 {
		 cValue = (char *) malloc(numVlaue+1);
		 memcpy(cValue,pValue,numVlaue);
		 cValue[numVlaue] = 0;
	 }

	 if(numData > 0)
	 {
		 cData = (char *)malloc(numData+1);
		 memcpy(cData,pData,numData);
		 cData[numData] = 0;
	 }


	 UpdateState(cmd,cValue,cData);
     LOGI("native  responseGenaEvent is [cmd=%d][value=%s][data=%s]",cmd,cValue,cData);


    if(cValue)
    	free(cValue);

    if(cData)
    	free(cData);

	 if ( pValue != NULL)
	     env->ReleaseByteArrayElements(value, pValue,NULL);

	 if ( pData != NULL)
	     env->ReleaseByteArrayElements(data, pData, NULL);

	 return 0;
}



int jniRegisterNativeMethods(JNIEnv* env,
							const char* className,
							const JNINativeMethod* gMethods,
							int numMethods){
	jclass clazz;



	jclass localClass = env->FindClass("com/xindawn/jni/PlatinumReflection");
	if(localClass == NULL)
	{

		LOGE("get localClass error!!");
		return -1;
	}

	g_inflectClass = (jclass)env->NewGlobalRef(localClass);
	if(g_inflectClass == NULL)
	{

		LOGE("get g_inflectClass error!!");
		return -1;
	}

	 env->DeleteLocalRef(localClass);


	g_methodID = env->GetStaticMethodID(g_inflectClass, "onActionReflection","(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if(g_methodID == NULL)
	{

		LOGE("get g_methodID error!!");
		return -1;
	}

	g_methodID1 = env->GetStaticMethodID(g_inflectClass, "onActionReflection","(ILjava/lang/String;[BLjava/lang/String;)V");
	if(g_methodID1 == NULL)
	{

		LOGE("get g_methodID1 error!!");
		return -1;
	}


	g_methodID2 = env->GetStaticMethodID(g_inflectClass, "audio_init","(IIII)V");
	if(g_methodID2 == NULL)
	{

		LOGE("get g_methodID2 error!!");
		return -1;
	}

	g_methodID3 = env->GetStaticMethodID(g_inflectClass, "audio_process","([BDI)V");
	if(g_methodID3 == NULL)
	{

		LOGE("get g_methodID3 error!!");
		return -1;
	}

	g_methodID4 = env->GetStaticMethodID(g_inflectClass, "audio_destroy","()V");
	if(g_methodID4 == NULL)
	{

		LOGE("get g_methodID4 error!!");
		return -1;
	}

	clazz = env->FindClass(className);
	if(clazz == NULL){

		LOGE("findclass error!!");
		return -1;
	}



	LOGI("className = %s,numMethods =%d",className,numMethods);

	if(env->RegisterNatives(clazz, gMethods, numMethods) < 0){
		return -1;
	}

	return 0;
}

static JNINativeMethod gMethods[] = {
	{"startMediaRender", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIIIILandroid/content/Context;)I", (void *)com_xindawn_jni_PlatinumJniProxy_startMediaRender },
	{"stopMediaRender", "()I", (void *)com_xindawn_jni_PlatinumJniProxy_stopMediaRender },
	{"responseGenaEvent", "(I[B[B)Z", (void *)com_xindawn_jni_PlatinumJniProxy_responseGenaEvent },
};

int register_com_xindawn_jni_PlatinumJniProxy(JNIEnv* env){
	return jniRegisterNativeMethods(env, kClassPathName, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
}

jint JNI_OnLoad(JavaVM* vm, void* reserved){
	JNIEnv* env = NULL;
	jint result = JNI_ERR;

	g_vm = vm;

	if(vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK){		
		LOGE("jni init fialed!!");
		return result;
	}

	if(register_com_xindawn_jni_PlatinumJniProxy(env) != JNI_OK){
		LOGE("register PlatinumJniProxy failed");
		goto end;
	}

	result = JNI_VERSION_1_4;

end:
	return result;
}


extern "C" void ActionInflect(int cmd, const char* value, const char* data,const char *title)
{
	jclass inflectClass = NULL;
	jmethodID inflectMethod = NULL;
	//LOGI("ActionInflect 1");

	if (g_vm == NULL)
	{
		LOGE("g_vm = NULL!!!");
		return ;
	}
	//LOGI("ActionInflect 2");

	int status;
	JNIEnv *env = NULL;
	bool isAttach = false;
	status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status != JNI_OK)
	{
		status = g_vm->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("callback_handler: failed to attach , current thread, status = %d", status);
			return;
		}
		isAttach = true;
	}
	//LOGI("ActionInflect 3");

	jstring valueString		= NULL;
	jstring dataString 		= NULL;
	jstring titleString 	= NULL;

	inflectClass 	= g_inflectClass;//env->FindClass("com/xindawn/jni/PlatinumReflection");
	//LOGI("ActionInflect 4");
	if (inflectClass == NULL )
	{
		LOGE("inflectClass == NULL!!!");
		goto end;
	}

	//jmethodID inflectMethod = env->GetStaticMethodID(inflectClass, "onActionReflection","()I;Ljava/lang/String;Ljava/lang/String;");
	inflectMethod = g_methodID;//env->GetStaticMethodID(inflectClass, "onActionReflection","(I;Ljava/lang/String;Ljava/lang/String;)V");

	//LOGI("ActionInflect 5");
	//jclass inflectClass 	= g_inflectClass;
	//jmethodID inflectMethod = g_methodID;



	if (inflectMethod == NULL)
	{
		LOGE("inflectMethod == NULL!!!");
		goto end;
	}

	//LOGI("ActionInflect 6");


	valueString = env->NewStringUTF(value);
	dataString  = env->NewStringUTF(data);
	titleString = env->NewStringUTF(title);

	env->CallStaticVoidMethod(inflectClass, inflectMethod, cmd, valueString, dataString,titleString);

	env->DeleteLocalRef(valueString);
	env->DeleteLocalRef(dataString);
	env->DeleteLocalRef(titleString);

end:
	if (env->ExceptionOccurred())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
	if (isAttach)
	{
		g_vm->DetachCurrentThread();
	}
}



extern "C" void ActionInflectByte(int cmd, const char* value, const char* data,unsigned int datasize,const char *title)
{
	jclass inflectClass = NULL;
	jmethodID inflectMethod = NULL;
	//LOGI("ActionInflect 1");

	if (g_vm == NULL)
	{
		LOGE("g_vm = NULL!!!");
		return ;
	}
	//LOGI("ActionInflect 2");

	int status;
	JNIEnv *env = NULL;
	bool isAttach = false;
	status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status != JNI_OK)
	{
		status = g_vm->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("callback_handler: failed to attach , current thread, status = %d", status);
			return;
		}
		isAttach = true;
	}
	//LOGI("ActionInflect 3");

	jstring valueString		= NULL;
	jbyteArray databyte 	= NULL;
	jstring titleString 	= NULL;

	inflectClass 	= g_inflectClass;//env->FindClass("com/xindawn/jni/PlatinumReflection");
	//LOGI("ActionInflect 4");
	if (inflectClass == NULL )
	{
		LOGE("inflectClass == NULL!!!");
		goto end;
	}

	//jmethodID inflectMethod = env->GetStaticMethodID(inflectClass, "onActionReflection","()I;Ljava/lang/String;Ljava/lang/String;");
	inflectMethod = g_methodID1;//env->GetStaticMethodID(inflectClass, "onActionReflection","(I;Ljava/lang/String;Ljava/lang/String;)V");

	//LOGI("ActionInflect 5");
	//jclass inflectClass 	= g_inflectClass;
	//jmethodID inflectMethod = g_methodID;



	if (inflectMethod == NULL)
	{
		LOGE("inflectMethod == NULL!!!");
		goto end;
	}


	valueString = env->NewStringUTF(value);
	databyte = env->NewByteArray(datasize);
	env->SetByteArrayRegion(databyte, 0, datasize, (jbyte*)data);
	titleString = env->NewStringUTF(title);

	env->CallStaticVoidMethod(inflectClass, inflectMethod, cmd, valueString, databyte,titleString);

	env->DeleteLocalRef(valueString);
	env->DeleteLocalRef(databyte);
	env->DeleteLocalRef(titleString);
end:
	if (env->ExceptionOccurred())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
	if (isAttach)
	{
		g_vm->DetachCurrentThread();
	}
}






extern "C" void ActionInflect_AudioInit(int bits, int channels, int samplerate,int isaudio)
{
	jclass inflectClass = NULL;
	jmethodID inflectMethod = NULL;

	if (g_vm == NULL)
	{
		LOGE("g_vm = NULL!!!");
		return ;
	}

	int status;
	JNIEnv *env = NULL;
	bool isAttach = false;
	status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status != JNI_OK)
	{
		status = g_vm->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("callback_handler: failed to attach , current thread, status = %d", status);
			return;
		}
		isAttach = true;
	}


	inflectClass 	= g_inflectClass;
	if (inflectClass == NULL )
	{
		LOGE("inflectClass == NULL!!!");
		goto end;
	}

	inflectMethod = g_methodID2;

	if (inflectMethod == NULL)
	{
		LOGE("inflectMethod == NULL!!!");
		goto end;
	}

	env->CallStaticVoidMethod(inflectClass, inflectMethod, bits, channels, samplerate,isaudio);


end:
	if (env->ExceptionOccurred())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
	if (isAttach)
	{
		g_vm->DetachCurrentThread();
	}
}





extern "C" void ActionInflect_AudioProcess(const char* data,unsigned int datasize,double timestamp,int seqnum)
{
	jclass inflectClass = NULL;
	jmethodID inflectMethod = NULL;

	if (g_vm == NULL)
	{
		LOGE("g_vm = NULL!!!");
		return ;
	}

	int status;
	JNIEnv *env = NULL;
	bool isAttach = false;
	status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status != JNI_OK)
	{
		status = g_vm->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("callback_handler: failed to attach , current thread, status = %d", status);
			return;
		}
		isAttach = true;
	}

	jbyteArray databyte 	= NULL;

	inflectClass 	= g_inflectClass;
	if (inflectClass == NULL )
	{
		LOGE("inflectClass == NULL!!!");
		goto end;
	}

	inflectMethod = g_methodID3;
	if (inflectMethod == NULL)
	{
		LOGE("inflectMethod == NULL!!!");
		goto end;
	}



	databyte = env->NewByteArray(datasize);
	env->SetByteArrayRegion(databyte, 0, datasize, (jbyte*)data);
	env->CallStaticVoidMethod(inflectClass, inflectMethod, databyte,timestamp,seqnum);
	env->DeleteLocalRef(databyte);

end:
	if (env->ExceptionOccurred())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
	if (isAttach)
	{
		g_vm->DetachCurrentThread();
	}
}





extern "C" void ActionInflect_AudioDestroy()
{
	jclass inflectClass = NULL;
	jmethodID inflectMethod = NULL;

	if (g_vm == NULL)
	{
		LOGE("g_vm = NULL!!!");
		return ;
	}

	int status;
	JNIEnv *env = NULL;
	bool isAttach = false;
	status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status != JNI_OK)
	{
		status = g_vm->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("callback_handler: failed to attach , current thread, status = %d", status);
			return;
		}
		isAttach = true;
	}


	inflectClass 	= g_inflectClass;
	if (inflectClass == NULL )
	{
		LOGE("inflectClass == NULL!!!");
		goto end;
	}

	inflectMethod = g_methodID4;

	if (inflectMethod == NULL)
	{
		LOGE("inflectMethod == NULL!!!");
		goto end;
	}

	env->CallStaticVoidMethod(inflectClass, inflectMethod);


end:
	if (env->ExceptionOccurred())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
	if (isAttach)
	{
		g_vm->DetachCurrentThread();
	}
}



extern "C" void ActionInflect_destroy(void)
{


	LOGI("ActionInflect_destroy");

	JNIEnv *env = NULL;
	int status;
	bool isAttach = false;
	status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status != JNI_OK)
	{
		status = g_vm->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("callback_handler: failed to attach , current thread, status = %d", status);
			return;
		}
		isAttach = true;
	}


	 if(g_inflectClass)
	 {
		 env->DeleteGlobalRef(g_inflectClass);
		 g_inflectClass = NULL;
	 }

 	if (isAttach)
 	{
 		g_vm->DetachCurrentThread();
 	}
}







static void airplay_open(void *cls,char *url, float fPosition)
{
	CStdString m_didl_metadata;
	char data[10];


	isPlaying  		= 0;
	isPause    		= 0;
	totalDuration 	= 0;
	currentPosition = 0;


	m_didl_metadata.Format("%s","<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\" xmlns:sec=\"http://www.sec.co.kr/dlna\">\r\n");
	m_didl_metadata += "<item id=\"7\" parentID=\"2\" restricted=\"1\">\r\n";

	m_didl_metadata += "<upnp:class>object.item.videoItem.movie</upnp:class>\r\n";
	m_didl_metadata += "</item>\r\n";
	m_didl_metadata += "</DIDL-Lite>";

	ActionInflect(0x100,(const char *)url,m_didl_metadata.c_str(),NULL);
	usleep(5000);

	memset(data,0,sizeof(data));
	sprintf(data,"%8f",fPosition);

	ActionInflect(0x102,(const char *)url,data,NULL);
	m_didl_metadata.Empty();

	LOGE("==============airplay_open===============================[%f]",fPosition);
}

static void airplay_play(void *cls)
{
    ActionInflect(0x102,NULL,NULL,NULL);

	LOGE("===airplay_play===\n");

}

static void airplay_stop(void *cls)
{
	char data[10];
    memset(data,0,sizeof(data));
    sprintf(data,"%d",0);
    ActionInflect(0x101,NULL,data,NULL);

	LOGE("===airplay_stop===\n");
}

static void airplay_pause(void *cls)
{

	ActionInflect(0x103,NULL,NULL,NULL);
	LOGE("===airplay_pause===\n");


}


static void airplay_seek(void *cls,long fPosition)
{
	char seektimeformat[128];
	memset(seektimeformat,0,sizeof(seektimeformat));
	sprintf(seektimeformat,"%s%02d:%02d:%02d","REL_TIME=",fPosition/3600,(fPosition - (fPosition/3600)*3600)/60,fPosition - (fPosition/3600)*3600 - ((fPosition - (fPosition/3600)*3600)/60)*60);
	LOGE("==============seektimeformat===============================[%d][%s]",fPosition,seektimeformat);
	ActionInflect(0x104,seektimeformat,NULL,NULL);
	LOGE("===airplay_seek===\n");
}

static void airplay_setvolume(void *cls,int volume)
{
	LOGE("===airplay_setvolume===\n");
}

static void airplay_showphoto(void *cls, unsigned char *data, long long size)
{
	CStdString m_didl_metadata;


    FILE *tmpFile;
    CStdString tmpFileName = "/mnt/sdcard/Android/data/airplay.jpg";
    tmpFile = fopen(tmpFileName.c_str(), "w+");
    fwrite(data, size, 1, tmpFile);
    fclose(tmpFile);


    m_didl_metadata.Format("%s","<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\" xmlns:sec=\"http://www.sec.co.kr/dlna\">\r\n");
	m_didl_metadata += "<item id=\"7\" parentID=\"2\" restricted=\"1\">\r\n";

	m_didl_metadata += "<upnp:class>object.item.imageItem.photo</upnp:class>\r\n";
	m_didl_metadata += "</item>\r\n";
	m_didl_metadata += "</DIDL-Lite>";

	ActionInflect(0x100,(const char *)tmpFileName.c_str(),m_didl_metadata.c_str(),NULL);
	usleep(5000);
	ActionInflect(0x102,(const char *)tmpFileName.c_str(),NULL,NULL);

	m_didl_metadata.Empty();

	LOGE("===airplay_showphoto===\n");
}

static long airplay_get_duration(void *cls)
{
	LOGE("===airplay_get_duration===\n");
	return totalDuration;
}

static long airplay_get_position(void *cls)
{
	LOGE("===airplay_get_position===\n");
	return  currentPosition;
}
static int airplay_isplaying(void *cls)
{
	LOGE("===airplay_isplaying===\n");
	return isPlaying;
}

static int airplay_ispaused(void *cls)
{
	LOGE("===airplay_ispaused===\n");
	return isPause;
}




static int proxy_fd = -1;

#define SAVE_264_ENABLE 0

#if SAVE_264_ENABLE
FILE  *m_handle = NULL;
#endif


static void video_mirroring_open(void *cls,int width,int height,const void *buffer, int buflen, int payloadtype, double timestamp)
{
	int wait = 70;
	int	 res;

	CStdString m_didl_metadata;
	char url[256];
	memset(url,0,256);


	sprintf(url,"%s","http://127.0.0.1:55556/airmirror-760ba1810b647cb0.flv");


	 m_didl_metadata.Format("%s","<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\" xmlns:sec=\"http://www.sec.co.kr/dlna\">\r\n");
	 m_didl_metadata += "<item id=\"7\" parentID=\"2\" restricted=\"1\">\r\n";
	 m_didl_metadata += "<upnp:class>object.item.screenItem.movie</upnp:class>\r\n";
	 m_didl_metadata += "</item>\r\n";
	 m_didl_metadata += "</DIDL-Lite>";

    ActionInflect(0x100,(const char *)url,m_didl_metadata.c_str(),NULL);

    usleep(5000);

    ActionInflect(0x102,(const char *)url,NULL,NULL);

    m_didl_metadata.Empty();


	while(1)
	{
		if(proxy_fd == -1)
		{

			usleep(100000);
			wait--;
			if(wait)
				continue;

		}
		break;
	}




	if(payloadtype == 1)
	{
		char tempbuf[256];
		memset(tempbuf,0,sizeof(tempbuf));
		sprintf(tempbuf,"--myboundary\r\nContent-Type: video/h264hdr/w=%d/h=%d\r\nContent-Length: %d\r\nTimestamp: %d\r\n\r\n",width,height,buflen,0);

		if(proxy_fd != -1)
		{
			int datalen;
			int written;
			datalen = strlen(tempbuf);
			written = 0;
			while (written < datalen)
			{
				res = send(proxy_fd,tempbuf+written,datalen-written,0);
				if (res == -1)
				{
					return;
				}
				written += res;
			}

		}
		if(proxy_fd != -1)
		{

			int datalen;
			int written;
			datalen = buflen;
			written = 0;
			while (written < datalen)
			{
				res = send(proxy_fd,(char *)buffer+written,datalen-written,0);
				if (res == -1)
				{
					return;
				}
				written += res;
			}
		}
	}
	else if(payloadtype == 0)
	{
		char tempbuf[256];
		memset(tempbuf,0,sizeof(tempbuf));
		sprintf(tempbuf, "--myboundary\r\nContent-Type: video/h264frm/w=%d/h=%d/tid=1/r=%d\r\nContent-Length: %d\r\nTimestamp: %d\r\n\r\n",0,0,0,buflen,0/*(tv.tv_usec - timebase.tv_usec) / 1000 + 1000 * (tv.tv_sec - timebase.tv_sec )*/);
		if(proxy_fd != -1)
		{
			int datalen;
			int written;
			datalen = strlen(tempbuf);
			written = 0;
			while (written < datalen)
			{
				res = send(proxy_fd,tempbuf+written,datalen-written,0);
				if (res == -1)
				{
					LOGE("send frm header(close)");
					return;
				}
				written += res;
			}
		}
		if(proxy_fd != -1)
		{

			int datalen;
			int written;
			datalen = buflen;
			written = 0;
			while (written < datalen)
			{
				res = send(proxy_fd,(char *)buffer+written,datalen-written,0);
				if (res == -1)
				{
					LOGE("send frm data(close)");
					return;
				}
				written += res;
			}
		}

	}



	LOGE("=====video_mirroring_open========\n");



#if SAVE_264_ENABLE
    {
        m_handle = fopen("test.264", "wb");



        int spscnt;
        int spsnalsize;
        int ppscnt;
        int ppsnalsize;

        unsigned    char *head = (unsigned  char *)buffer;




        spscnt = head[5] & 0x1f;
        spsnalsize = ((uint32_t)head[6] << 8) | ((uint32_t)head[7]);
        ppscnt = head[8 + spsnalsize];
        ppsnalsize = ((uint32_t)head[9 + spsnalsize] << 8) | ((uint32_t)head[10 + spsnalsize]);


        unsigned char *data = (unsigned char *)malloc(4 + spsnalsize + 4 + ppsnalsize);


        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 1;

        memcpy(data + 4, head + 8, spsnalsize);

        data[4 + spsnalsize] = 0;
        data[5 + spsnalsize] = 0;
        data[6 + spsnalsize] = 0;
        data[7 + spsnalsize] = 1;

        memcpy(data + 8 + spsnalsize, head + 11 + spsnalsize, ppsnalsize);


        fwrite(data,1,4 + spsnalsize + 4 + ppsnalsize,m_handle);


        free(data);


    }

#endif





}


static void video_mirroring_process(void *cls,const void *buffer, int buflen, int payloadtype, double timestamp)
{
	int	 res;


	if(payloadtype == 1)
	{
		char tempbuf[256];
		memset(tempbuf,0,sizeof(tempbuf));

		sprintf(tempbuf,"--myboundary\r\nContent-Type: video/h264hdr/w=%d/h=%d\r\nContent-Length: %d\r\nTimestamp: %d\r\n\r\n",0,0,buflen,0);

		if(proxy_fd != -1)
		{
			int datalen;
			int written;
			datalen = strlen(tempbuf);
			written = 0;
			while (written < datalen)
			{
				res = send(proxy_fd,tempbuf+written,datalen-written,0);
				if (res == -1)
				{
					return;
				}
				written += res;
			}

		}
		if(proxy_fd != -1)
		{

			int datalen;
			int written;
			datalen = buflen;
			written = 0;
			while (written < datalen)
			{
				res = send(proxy_fd,(char *)buffer+written,datalen-written,0);
				if (res == -1)
				{
					return;
				}
			written += res;
			}
		}
	}
	else if(payloadtype == 0)
	{
		char tempbuf[256];
		memset(tempbuf,0,sizeof(tempbuf));
		sprintf(tempbuf, "--myboundary\r\nContent-Type: video/h264frm/w=%d/h=%d/tid=1/r=%d\r\nContent-Length: %d\r\nTimestamp: %d\r\n\r\n",0,0,0,buflen,0/*(tv.tv_usec - timebase.tv_usec) / 1000 + 1000 * (tv.tv_sec - timebase.tv_sec )*/);
		if(proxy_fd != -1)
		{
				int datalen;
				int written;
				datalen = strlen(tempbuf);
				written = 0;
				while (written < datalen)
				{
					res = send(proxy_fd,tempbuf+written,datalen-written,0);
					if (res == -1)
					{
						LOGE("send frm header(close)");
						return;
					}
					written += res;
				}
		}
		if(proxy_fd != -1)
		{

			  int datalen;
			  int written;
			  datalen = buflen;
			  written = 0;
			  while (written < datalen)
			  {
					res = send(proxy_fd,(char *)buffer+written,datalen-written,0);
					if (res == -1)
					{
						LOGE("send frm data(close)");
						return;
					}
					written += res;
			  }
		}

	}



    if (payloadtype == 0)
    {

#if SAVE_264_ENABLE
        {


            int		    rLen;
            unsigned    char *head;



            unsigned char *data = (unsigned char *)malloc(buflen);
            memcpy(data, buffer, buflen);



            rLen = 0;
            head = (unsigned char *)data + rLen;
            while (rLen < buflen)
            {
                rLen += 4;
                rLen += (((uint32_t)head[0] << 24) | ((uint32_t)head[1] << 16) | ((uint32_t)head[2] << 8) | (uint32_t)head[3]);

                head[0] = 0;
                head[1] = 0;
                head[2] = 0;
                head[3] = 1;

                head = (unsigned char *)data + rLen;
            }



            fwrite(data,1,buflen,m_handle);

            free(data);


        }
#endif

    }
    else if (payloadtype == 1)
    {



#if SAVE_264_ENABLE
        {
            int spscnt;
            int spsnalsize;
            int ppscnt;
            int ppsnalsize;

            unsigned    char *head = (unsigned  char *)buffer;




            spscnt = head[5] & 0x1f;
            spsnalsize = ((uint32_t)head[6] << 8) | ((uint32_t)head[7]);
            ppscnt = head[8 + spsnalsize];
            ppsnalsize = ((uint32_t)head[9 + spsnalsize] << 8) | ((uint32_t)head[10 + spsnalsize]);


            unsigned char *data = (unsigned char *)malloc(4 + spsnalsize + 4 + ppsnalsize);


            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 1;

            memcpy(data + 4, head + 8, spsnalsize);

            data[4 + spsnalsize] = 0;
            data[5 + spsnalsize] = 0;
            data[6 + spsnalsize] = 0;
            data[7 + spsnalsize] = 1;

            memcpy(data + 8 + spsnalsize, head + 11 + spsnalsize, ppsnalsize);


            fwrite(data,1,4 + spsnalsize + 4 + ppsnalsize,m_handle);


            free(data);


        }

#endif

    }


    LOGE("=====video_mirroring_process====%f====\n",timestamp);

}

static void video_mirroring_stop(void *cls)
{

	proxy_fd = -1;

	char data[10];
	memset(data,0,sizeof(data));
	sprintf(data,"%d",1);
	ActionInflect(0x101,NULL,data,NULL);

#if SAVE_264_ENABLE
    fclose(m_handle);
#endif

    LOGE("=====video_mirroring_stop========\n");

}

static void audio_open(void *cls, int bits, int channels, int samplerate, int isaudio)
{
	ActionInflect_AudioInit(bits,channels,samplerate,isaudio);
	LOGE("=====audio_open====\n");

}


static void audio_setvolume(void *cls,int volume)
{
	char data[10];
	memset(data,0,sizeof(data));
	sprintf(data,"%d",volume);
    ActionInflect(0x105,data,NULL,NULL);
	LOGE("=====audio_setvolume====%d====\n",volume);
}


static void audio_process(void *cls,const void *buffer, int buflen, double timestamp, uint32_t seqnum)
{

	ActionInflect_AudioProcess((const char *)buffer,buflen,timestamp,seqnum);
	LOGE("=====audio_process====%f====\n",timestamp);
}

static void audio_flush(void *cls)
{


	LOGE("=====audio_flush====\n");
}


static inline uint32_t Endian_SwapBE32(uint32_t x)
{
        return((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
}


std::string m_metadata[3];
std::map<std::string, std::string> decodeDMAP(const char *buffer, unsigned int size)
{
  std::map<std::string, std::string> result;
  unsigned int offset = 8;
  while (offset < size)
  {
    std::string tag;
    tag.append(buffer + offset, 4);
    offset += 4;
    uint32_t length = Endian_SwapBE32(*(uint32_t *)(buffer + offset));
    offset += sizeof(uint32_t);
    std::string content;
    content.append(buffer + offset, length);//possible fixme - utf8?
    offset += length;
    result[tag] = content;
  }
  return result;
}




static void audio_metadata(void *cls,const void *buffer, int buflen)
{
	//TODO
	  CStdString m_didl_metadata;
	  CStdString m_didl_metadata_body;
	  std::map<std::string, std::string> metadata = decodeDMAP(buffer, buflen);

	  m_didl_metadata.Format("%s","<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\" xmlns:dlna=\"urn:schemas-dlna-org:metadata-1-0/\" xmlns:sec=\"http://www.sec.co.kr/dlna\">\r\n");
	  m_didl_metadata += "<item id=\"7\" parentID=\"2\" restricted=\"1\">\r\n";

	  if(metadata["asal"].length())
	  {
	    m_metadata[0] = metadata["asal"];//album
	    //ALOGD( "AirPlayServer~~~~~~~:audio_set_metadata[%s]\n",m_metadata[0].c_str());
	    m_didl_metadata += "<upnp:album>";
	    m_didl_metadata_body.Format("%s",m_metadata[0].c_str());
	    m_didl_metadata += m_didl_metadata_body;
	    m_didl_metadata += "</upnp:album>\r\n";
	  }
	  if(metadata["minm"].length())
	  {
	    m_metadata[1] = metadata["minm"];//title
	    //ALOGD( "AirPlayServer~~~~~~~:audio_set_metadata[%s]\n",m_metadata[1].c_str());

	    m_didl_metadata += "<dc:title>";
	    m_didl_metadata_body.Format("%s",m_metadata[1].c_str());
	    m_didl_metadata += m_didl_metadata_body;
	    m_didl_metadata += "</dc:title>\r\n";

	  }
	  if(metadata["asar"].length())
	  {
		m_metadata[2] = metadata["asar"];//artist
	    //ALOGD( "AirPlayServer~~~~~~~:audio_set_metadata[%s]\n",m_metadata[2].c_str());

		m_didl_metadata += "<upnp:artist>";
		m_didl_metadata_body.Format("%s",m_metadata[2].c_str());
		m_didl_metadata += m_didl_metadata_body;
		m_didl_metadata += "</upnp:artist>\r\n";


	  }

	  m_didl_metadata += "<upnp:class>object.item.audioItem.musicTrack</upnp:class>\r\n";
	  m_didl_metadata += "</item>\r\n";
	  m_didl_metadata += "</DIDL-Lite>";

	  ActionInflect(0x10a,NULL,m_didl_metadata.c_str(),NULL);

	  LOGE("=====audio_metadata====\n");
}

static void audio_coverart(void *cls,const void *buffer, int buflen)
{


	ActionInflectByte(0x110, NULL, buffer,buflen,NULL);

	LOGE("=====audio_coverart====\n");
}


static void audio_stop(void *cls)
{
	ActionInflect_AudioDestroy();
	LOGE("=====audio_stop========\n");
}


#define RECEIVEBUFFER 1024
struct proxy_connection_s
{
    int connected;
    int m_socket;
    void *user_data;
	struct sockaddr_storage m_cliaddr;
	socklen_t m_addrlen;

};

typedef struct proxy_connection_s proxy_connection_t;

static proxy_connection_t  m_proxyConnections;
static int 	m_proxySocket = -1;
static int 	m_proxyPort;
static pthread_t mirroring_thread_tid;
static bool    mirroring_stop = true;


int CreateProxyServerSocket(int *port, int use_ipv6, int use_udp)
{
	int family = use_ipv6 ? AF_INET6 : AF_INET;
	int type = use_udp ? SOCK_DGRAM : SOCK_STREAM;
	int proto = use_udp ? IPPROTO_UDP : IPPROTO_TCP;
	int backlog = 5;

	struct sockaddr_storage saddr;
	socklen_t socklen;
	int server_fd;
	int ret;


	server_fd = socket(family, type, proto);
	if (server_fd == -1) {
		goto cleanup;
	}

	memset(&saddr, 0, sizeof(saddr));
	if (use_ipv6) {
		struct sockaddr_in6 *sin6ptr = (struct sockaddr_in6 *)&saddr;
		int v6only = 1;
		const int one = 1;

		/* Initialize sockaddr for bind */
		sin6ptr->sin6_family = family;
		sin6ptr->sin6_addr = in6addr_any;
		sin6ptr->sin6_port = htons(*port);

		/* Make sure we only listen to IPv6 addresses */
		setsockopt(server_fd, IPPROTO_IPV6, IPV6_V6ONLY,
			(char *)&v6only, sizeof(v6only));

		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

		socklen = sizeof(*sin6ptr);
		ret = bind(server_fd, (struct sockaddr *)sin6ptr, socklen);
		if (ret == -1) {
			goto cleanup;
		}

		ret = getsockname(server_fd, (struct sockaddr *)sin6ptr, &socklen);
		if (ret == -1) {
			goto cleanup;
		}
		*port = ntohs(sin6ptr->sin6_port);
	}
	else {
		struct sockaddr_in *sinptr = (struct sockaddr_in *)&saddr;

		const int one = 1;

		/* Initialize sockaddr for bind */
		sinptr->sin_family = family;
		sinptr->sin_addr.s_addr = INADDR_ANY;
		sinptr->sin_port = htons(*port);


		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

		socklen = sizeof(*sinptr);
		ret = bind(server_fd, (struct sockaddr *)sinptr, socklen);
		if (ret == -1) {
			goto cleanup;
		}

		ret = getsockname(server_fd, (struct sockaddr *)sinptr, &socklen);
		if (ret == -1) {
			goto cleanup;
		}
		*port = ntohs(sinptr->sin_port);
	}


	if (listen(server_fd, backlog) < 0)
	{
		LOGE( "CreateProxyServerSocket Server: Failed to set listen");
		goto cleanup;
	}


	return server_fd;

cleanup:
	if (server_fd != -1)
	{
		close(server_fd);
	}
	return -1;
}

static void *mirroring_thread(void *arg)
{


	while (!mirroring_stop)
	{
		int             max_fd = 0;
		fd_set          rfds;
		struct timeval  to = { 1, 0 };
		FD_ZERO(&rfds);

		FD_SET(m_proxySocket, &rfds);
		max_fd = m_proxySocket;

		if(m_proxyConnections.m_socket != INVALID_SOCKET )
		{
			FD_SET(m_proxyConnections.m_socket, &rfds);
			if (m_proxyConnections.m_socket > max_fd)
			max_fd = m_proxyConnections.m_socket;
		}

		int res = select(max_fd + 1, &rfds, NULL, NULL, &to);
		if (res < 0)
		{
			LOGE( "AIRPLAY Server: Select failed");
			sleep(1);
		}
		else if (res > 0)
		{
			int socket = m_proxyConnections.m_socket;
			if ((socket != INVALID_SOCKET) && FD_ISSET(socket, &rfds))
			{
				char buffer[RECEIVEBUFFER] = {};
				int  nread = 0;
				nread = recv(socket, (char*)&buffer, RECEIVEBUFFER, 0);

				if (nread > 0)
				{

					int result;
					int datalen;
					int written;
					unsigned char *res_data = NULL;
					//校验播放器
					buffer[nread] = 0;

					LOGE("=======%s==========recv get quest===================",buffer);

					if(!strstr(buffer,"stlAirPlayer"))
					{

						LOGE("==============no==authorization=======user============");

						continue;
					}


					res_data = (unsigned char *)malloc(512);
					memset(res_data,0,512);
					sprintf(res_data,"HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=--myboundary\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",0x7fffffff);

					datalen = strlen(res_data);
					written = 0;
					while (written < datalen)
					{
						result = send(socket,res_data+written,datalen-written,0);
						written += result;
					}

					if(res_data)
						free(res_data);

					proxy_fd = m_proxyConnections.m_socket;



				}
				if (nread <= 0)
				{
					LOGE( "CFCUPM3U8Play Server: Disconnection detected");
					if (m_proxyConnections.m_socket != INVALID_SOCKET)
					{
						shutdown(m_proxyConnections.m_socket, SHUT_RDWR);
						close(m_proxyConnections.m_socket);
						m_proxyConnections.m_socket = INVALID_SOCKET;

					}

				}

			}

			if ( (m_proxySocket != INVALID_SOCKET)  && FD_ISSET(m_proxySocket, &rfds))
			{
					m_proxyConnections.m_addrlen = sizeof(struct sockaddr_storage);
					m_proxyConnections.m_socket = accept(m_proxySocket, (struct sockaddr*) &m_proxyConnections.m_cliaddr, &m_proxyConnections.m_addrlen);

					if (m_proxyConnections.m_socket == INVALID_SOCKET)
					{
						break;

					}
					else
					{

					}
			}


		}
	}


	if (m_proxyConnections.m_socket != INVALID_SOCKET)
	{
		shutdown(m_proxyConnections.m_socket, SHUT_RDWR);
		close(m_proxyConnections.m_socket);
		m_proxyConnections.m_socket = INVALID_SOCKET;
	}


	if (m_proxySocket != INVALID_SOCKET)
	{
		shutdown(m_proxySocket, SHUT_RDWR);
		close(m_proxySocket);
		m_proxySocket = INVALID_SOCKET;
	}

	return NULL;
}



static jint com_xindawn_jni_PlatinumJniProxy_startMediaRender(JNIEnv* env,jobject thiz, jstring friendname,jstring libpath,jstring activecode,int width,int height,int airtunes_port,int airplay_port,int rcvsize, jobject context)
{


    m_proxyConnections.m_socket =  INVALID_SOCKET;
    m_proxySocket               =  INVALID_SOCKET;
    m_proxyPort                 =  55556;


    if ((m_proxySocket = CreateProxyServerSocket(&m_proxyPort, 0,0 )) == INVALID_SOCKET)
    {
    	LOGE( "AIRPLAY Server: Failed to create mirroring thread.\n");
     	return 0;
    }


    mirroring_stop = false;

	pthread_create(&mirroring_thread_tid,NULL,mirroring_thread,NULL);


    airplay_callbacks_t ao;
    memset(&ao,0,sizeof(airplay_callbacks_t));
    ao.cls						 = NULL;
    ao.AirPlayMirroring_Play     = video_mirroring_open;
    ao.AirPlayMirroring_Process  = video_mirroring_process;
    ao.AirPlayMirroring_Stop     = video_mirroring_stop;

    ao.AirPlayAudio_Init         = audio_open;
    ao.AirPlayAudio_SetVolume    = audio_setvolume;
    ao.AirPlayAudio_Process      = audio_process;
    ao.AirPlayAudio_destroy      = audio_stop;
    ao.AirPlayAudio_SetMetadata  = audio_metadata;
    ao.AirPlayAudio_SetCoverart  = audio_coverart;
    ao.AirPlayAudio_Flush        = audio_flush;


    ao.AirPlayPlayback_Open          = airplay_open;
    ao.AirPlayPlayback_Play          = airplay_play;
    ao.AirPlayPlayback_Pause         = airplay_pause;
    ao.AirPlayPlayback_Stop          = airplay_stop;
    ao.AirPlayPlayback_Seek    		 = airplay_seek;
    ao.AirPlayPlayback_SetVolume     = airplay_setvolume;
	ao.AirPlayPlayback_ShowPhoto	 = airplay_showphoto;
	ao.AirPlayPlayback_GetDuration   = airplay_get_duration;
	ao.AirPlayPlayback_GetPostion    = airplay_get_position;
	ao.AirPlayPlayback_IsPlaying     = airplay_isplaying;
	ao.AirPlayPlayback_IsPaused      = airplay_ispaused;




	char* pFriendname = (char *)env->GetStringUTFChars(friendname, NULL);
	char* pLibPath    = (char *)env->GetStringUTFChars(libpath, NULL);
	char* pActivecode = (char *)env->GetStringUTFChars(activecode, NULL);
	LOGI("native get friendname is %s,pActivecode is %s",pFriendname,pActivecode);
	int ret = XinDawn_StartMediaServer(pFriendname,pLibPath,width,height,pActivecode,&ao);
	LOGI("ret=  %d",ret);
	env->ReleaseStringUTFChars(activecode,pActivecode);
	env->ReleaseStringUTFChars(libpath,pLibPath);
	env->ReleaseStringUTFChars(friendname,pFriendname);

	return 0;

}

static jint com_xindawn_jni_PlatinumJniProxy_stopMediaRender(JNIEnv *env, jobject thiz)
{



	XinDawn_StopMediaServer();

	mirroring_stop = true;

	pthread_join(mirroring_thread_tid, NULL);

	return 0;

}



