package com.xindawn;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.wifi.p2p.WifiP2pManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.xindawn.center.MediaRenderProxy;
import com.xindawn.datastore.LocalConfigSharePreference;
import com.xindawn.util.CommonLog;
import com.xindawn.util.DlnaUtils;
import com.xindawn.util.LogFactory;
import android.content.Context;
import com.xindawn.util.FileUtil;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import android.database.Cursor;
import android.provider.MediaStore;
import com.xindawn.dms.ContentTree;
import java.util.HashMap;
import java.util.Set;
import java.util.Iterator;
import java.util.Map.Entry;
import com.xindawn.util.ImageUtil;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiInfo;
import android.widget.Toast;
import com.xindawn.application.BaseApplication;
import android.content.ComponentName;


/**
 * @author lance
 * @csdn  http://blog.csdn.net/geniuseoe2012
 * @github https://github.com.xindawn
 */
public class MainActivity extends BaseActivity implements OnClickListener, DeviceUpdateBrocastFactory.IDevUpdateListener{

private static final CommonLog log = LogFactory.createLog();
	
	private Button mBtnStart;
	private Button mBtnReset;
	private Button mBtnStop;
	
	private Button mBtnEditName;
	private EditText mETName;
	private TextView mTVDevInfo;


	private MediaRenderProxy mRenderProxy;
	private RenderApplication mApplication;
	private DeviceUpdateBrocastFactory mBrocastFactory;

	private Context mContext;
	private List<Map<String, String>> mVideoFilePaths;
	private String hostName;

	private String hostAddress;
	public static final int GET_IP_FAIL = 0;

	public static final int GET_IP_SUC = 1;

	private Handler mHandle = new Handler() {

	@Override
	public void handleMessage(Message msg) {
			switch (msg.what) {
				case GET_IP_FAIL: {
					Toast.makeText(mContext, R.string.ip_get_fail, Toast.LENGTH_SHORT);
					break;
				}
				case GET_IP_SUC: {
					if (null != msg.obj) {
						InetAddress inetAddress = (InetAddress) msg.obj;
						if (null != inetAddress) {
							setIp(inetAddress);
							setIpInfo();
							jumpToMain();
						}
					} else {
						Toast.makeText(mContext, R.string.ip_get_fail, Toast.LENGTH_SHORT);
					}
					break;
				}

			}

			super.handleMessage(msg);
		}

	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mContext = this;

		setupView();
		initData();
		start();

		createFolder();
		getVideoFilePaths();
		createVideoThumb();
		getIp();

		}


	@Override
	protected void onDestroy() {
		unInitData();	
		super.onDestroy();
	}

	private void setupView(){
		mBtnStart = (Button) findViewById(R.id.btn_init);
    	mBtnReset = (Button) findViewById(R.id.btn_reset);
    	mBtnStop = (Button) findViewById(R.id.btn_exit);
    	mBtnEditName = (Button) findViewById(R.id.bt_dev_name);
    	mBtnStart.setOnClickListener(this);
    	mBtnReset.setOnClickListener(this);
    	mBtnStop.setOnClickListener(this);
    	mBtnEditName.setOnClickListener(this);
    	
    	mTVDevInfo = (TextView) findViewById(R.id.tv_dev_info);
    	mETName = (EditText) findViewById(R.id.et_dev_name);
    	
	}
	
	private void initData(){
		mApplication = RenderApplication.getInstance();
		mRenderProxy = MediaRenderProxy.getInstance();
		mBrocastFactory = new DeviceUpdateBrocastFactory(this);
		
		String dev_name = DlnaUtils.getDevName(this);
		mETName.setText(dev_name);
		mETName.setEnabled(false);
		
		updateDevInfo(mApplication.getDevInfo());
		mBrocastFactory.register(this);
	}

	private void unInitData(){
		mBrocastFactory.unregister();
	}

	private void updateDevInfo(DeviceInfo object){
		String status = object.status ? "open" : "close";
		String text = "status: " + status + "\n" +
						"friend name: " + object.dev_name + "\n" + 
					   "uuid: " + object.uuid;
		mTVDevInfo.setText(text);
	}

	@Override
	public void onClick(View v) {
		switch(v.getId()){
		case R.id.btn_init:
			start();
			break;
		case R.id.btn_reset:
			reset();
			break;
		case R.id.btn_exit:
			stop();
			finish();
			break;
		case R.id.bt_dev_name:
			change();
			break;
		}
	}

	private void createFolder() {
		FileUtil.createSDCardDir(true);
	}

	private void getVideoFilePaths() {
		mVideoFilePaths = new ArrayList<Map<String, String>>();
		Cursor cursor;
		String[] videoColumns = {
				MediaStore.Video.Media._ID, MediaStore.Video.Media.TITLE,
				MediaStore.Video.Media.DATA, MediaStore.Video.Media.ARTIST,
				MediaStore.Video.Media.MIME_TYPE, MediaStore.Video.Media.SIZE,
				MediaStore.Video.Media.DURATION, MediaStore.Video.Media.RESOLUTION
		};
		cursor = mContext.getContentResolver().query(MediaStore.Video.Media.EXTERNAL_CONTENT_URI,
				videoColumns, null, null, null);
		if (null != cursor && cursor.moveToFirst()) {
			do {
				String id = ContentTree.VIDEO_PREFIX
						+ cursor.getInt(cursor.getColumnIndex(MediaStore.Video.Media._ID));
				String filePath = cursor.getString(cursor
						.getColumnIndexOrThrow(MediaStore.Video.Media.DATA));
				Map<String, String> fileInfoMap = new HashMap<String, String>();
				fileInfoMap.put(id, filePath);
				mVideoFilePaths.add(fileInfoMap);
				// Log.v(LOGTAG, "added video item " + title + "from " +
				// filePath);
			} while (cursor.moveToNext());
		}
		if (null != cursor) {
			cursor.close();
		}

	}

	private void createVideoThumb() {
		if (null != mVideoFilePaths && mVideoFilePaths.size() > 0) {
			new Thread(new Runnable() {

				@Override
				public void run() {
					for (int i = 0; i < mVideoFilePaths.size(); i++) {
						Set entries = mVideoFilePaths.get(i).entrySet();
						if (entries != null) {
							Iterator iterator = entries.iterator();
							while (iterator.hasNext()) {
								Map.Entry entry = (Entry) iterator.next();
								Object id = entry.getKey();
								Object filePath = entry.getValue();

								Bitmap videoThumb = ImageUtil.getThumbnailForVideo(filePath
										.toString());
								String videoSavePath = ImageUtil.getSaveVideoFilePath(
										filePath.toString(), id.toString());
								try {
									ImageUtil.saveBitmapWithFilePathSuffix(videoThumb,
											videoSavePath);
								} catch (Exception e) {
									e.printStackTrace();
								}
							}
						}
					}
				}

			}).start();
		}
	}

	private void getIp() {
		new Thread(new Runnable() {

			@Override
			public void run() {
				WifiManager wifiManager = (WifiManager) mContext.getSystemService(WIFI_SERVICE);
				WifiInfo wifiInfo = wifiManager.getConnectionInfo();
				int ipAddress = wifiInfo.getIpAddress();

				InetAddress inetAddress;
				Message message = new Message();
				try {
					inetAddress = InetAddress.getByName(String.format("%d.%d.%d.%d",
							(ipAddress & 0xff), (ipAddress >> 8 & 0xff), (ipAddress >> 16 & 0xff),
							(ipAddress >> 24 & 0xff)));

					hostName = inetAddress.getHostName();
					hostAddress = inetAddress.getHostAddress();
					message.obj = inetAddress;
					message.what = GET_IP_SUC;
					mHandle.sendMessage(message);
				} catch (UnknownHostException e) {
					mHandle.sendEmptyMessage(GET_IP_FAIL);
				}
			}
		}).start();

	}

	private void setIp(InetAddress inetAddress) {
		BaseApplication.setLocalIpAddress(inetAddress);
	}

	private void setIpInfo() {
		BaseApplication.setHostName(hostName);
		BaseApplication.setHostAddress(hostAddress);
	}

	private void start(){
		mRenderProxy.startEngine();
	}
	
	private void reset(){
		mRenderProxy.restartEngine();
	}
	
	private void stop(){
		mRenderProxy.stopEngine();
	}
	
	private void change(){
		if (mETName.isEnabled()){
			mETName.setEnabled(false);
			DlnaUtils.setDevName(this, mETName.getText().toString());
		}else{
			mETName.setEnabled(true);
		}
	}

	private void jumpToMain() {
		Intent intent = new Intent(MainActivity.this, DevicesActivity.class);
		startActivity(intent);
		this.finish();
	}


	@Override
	public void onUpdate() {
		updateDevInfo(mApplication.getDevInfo());
	}

}
