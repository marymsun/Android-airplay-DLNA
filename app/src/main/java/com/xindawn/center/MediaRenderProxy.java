package com.xindawn.center;

import com.xindawn.RenderApplication;
import com.xindawn.service.MediaRenderService;
import com.xindawn.util.CommonLog;
import com.xindawn.util.LogFactory;

import android.content.Context;
import android.content.Intent;

public class MediaRenderProxy implements IBaseEngine{

	private static final CommonLog log = LogFactory.createLog();
	
	private static  MediaRenderProxy mInstance;
	private Context mContext;
	
	private MediaRenderProxy(Context context) {
		mContext = context;
	}

	public static synchronized MediaRenderProxy getInstance() {
		if (mInstance == null){
			mInstance  = new MediaRenderProxy(RenderApplication.getInstance());
		}
		return mInstance;
	}

	@Override
	public boolean startEngine() {

		//mContext.startService(new Intent(MediaRenderService.START_RENDER_ENGINE));
		Intent mIntent = new Intent();
		mIntent.setAction(MediaRenderService.START_RENDER_ENGINE);
		mIntent.setPackage(mContext.getPackageName());//"com.xindawn"
		mContext.startService(mIntent);
		return true;
	}

	@Override
	public boolean stopEngine() {
		mContext.stopService(new Intent(mContext, MediaRenderService.class));
		return true;
	}

	@Override
	public boolean restartEngine() {
		mContext.startService(new Intent(MediaRenderService.RESTART_RENDER_ENGINE));
		return true;
	}

}
