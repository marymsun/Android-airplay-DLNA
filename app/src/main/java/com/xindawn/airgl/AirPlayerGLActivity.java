package com.xindawn.airgl;


import com.xindawn.center.DlnaMediaModel;
import com.xindawn.center.DlnaMediaModelFactory;
import com.xindawn.center.MediaControlBrocastFactory;
import com.xindawn.util.CommonLog;
import com.xindawn.util.CommonUtil;
import com.xindawn.util.DlnaUtils;
import com.xindawn.util.LogFactory;


import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class AirPlayerGLActivity extends Activity implements MediaControlBrocastFactory.IMediaControlListener 
{

	
    static NativeVideo mf = new NativeVideo();

    private GlView     mGlView;
    
	private String mFilePath;
	
	private static final CommonLog log = LogFactory.createLog();
	private DlnaMediaModel mMediaInfo = new DlnaMediaModel();
	private MediaControlBrocastFactory mMediaControlBorcastFactor;
	public static Handler mHandler;
	
	private static final int REFRESH_SPEED= 0x0001;
	private static final int EXIT_ACTIVITY = 0x0002;
	
	private int isOpened = 0;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		

		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		getFilePath();
		
		
		mMediaControlBorcastFactor = new MediaControlBrocastFactory(this);
		mMediaControlBorcastFactor.register(this);
		
		mHandler = new Handler(){

			@Override
			public void handleMessage(Message msg) {
				switch(msg.what){
				case EXIT_ACTIVITY:
					log.d("EXIT_ACTIVITY");
					finish();
					break;
				}
			}
			
		};
		
		
		try{
			initViews();
		}catch (NullPointerException e) {
			finish();
		}
		
		
		//mFilePath = "http://192.168.1.106:55556/airmirror-760ba1810b647cb0.flv";
		
		
		if((isOpened = mf.Open(mFilePath)) != 0)
		{
			log.d("excute onStopCommand" + "[" + isOpened +"]");
			
			finish();
		}
		
		log.d("mf.Open" + "[" + isOpened +"]");
		
		mf.Start();
		
	    Window window = getWindow();
        WindowManager.LayoutParams params = window.getAttributes();
        params.systemUiVisibility = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        window.setAttributes(params);
		
		setContentView(mGlView);
	}
	
    @Override protected void onPause() {
        super.onPause();
        mGlView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mGlView.onResume();
    }
	
	
	private void removeExitMessage(){
		mHandler.removeMessages(EXIT_ACTIVITY);
	}
	
	private static final int EXIT_DELAY_TIME = 200;
	private void delayToExit(){
		removeExitMessage();
		//mHandler.sendEmptyMessageDelayed(EXIT_ACTIVITY, EXIT_DELAY_TIME);
		mHandler.sendEmptyMessage(EXIT_ACTIVITY);
	}
	

	private void getFilePath() {
		
		Intent intent = getIntent();
	    mMediaInfo = DlnaMediaModelFactory.createFromIntent(intent);
	    mFilePath = mMediaInfo.getUrl();
		if(mFilePath == null)
		{
			finish();
		}
	}

	private void initViews() throws NullPointerException{
		
		mGlView = new GlView(this);
		
		if(mGlView == null)
			throw new NullPointerException();
	}
//  保持屏幕常亮	
//	   @Override  
//	 protected void onResume() 
//	 {  
//	        super.onResume();  
//	        pManager = ((PowerManager) getSystemService(POWER_SERVICE));  
//	        mWakeLock = pManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK  
//	                | PowerManager.ON_AFTER_RELEASE, TAG);  
//	        mWakeLock.acquire();  
//	 }  
//	      
//	    @Override  
//	 protected void onPause() 
//	 {  
//	        super.onPause();  
//	          
//	        if(null != mWakeLock)
//	        {  
//	            mWakeLock.release();  
//	        }  
//	 }  
	
	@Override
	protected void onDestroy() {
		
		log.d("onDestroy 1");
		
		//mGlView.destroyDrawingCache();
		
		
	
		//mGlView = null;
		
		mMediaControlBorcastFactor.unregister();
		super.onDestroy();
	}
	
	@Override
    public boolean onKeyUp(int i1, KeyEvent keyevent)
    {

        return true;
    }


	

	@Override
	public void finish() {
		// TODO Auto-generated method stub

		log.d("finish 1");
		super.finish();
		log.d("finish 2");
		
		mf.Close();
		log.d("finish 3");

	}

	
	@Override
	public void onPlayCommand() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onPauseCommand() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onStopCommand(int type) {
		if(1 == type)
		{
			log.d("excute onStopCommand" + "[" + type +"]");
			delayToExit();
		}
		else 
		{
			log.d("ignore onStopCommand" + "[" + type +"]");		
		}

	}

	@Override
	public void onSeekCommand(int time) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void onCoverCommand(byte data[])
	{
		
	}
	
	@Override
	public void onMetaDataCommand(String data)
	{
		
	}
	
	@Override
	public void onIPAddrCommand(String data)
	{

	}
	
//	
}
