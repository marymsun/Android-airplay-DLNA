package com.xindawn.airgl;

public class NativeVideo {

    static
    {

     
//		System.loadLibrary("avutil-52");
//		System.loadLibrary("swscale-2");
//		System.loadLibrary("avcodec-55");
//		System.loadLibrary("avformat-55");
		System.loadLibrary("mediaplayergl");
        
    }

    public static native int Open(String path);
    public static native void Close();
    public static native void Start();
    public static native void Pause();
    public static native void Seek(double percentage);
    
    public static native void GetFrame(byte[] array);

    public static native int  getBufferSize();
    
    public static native int GetWidth();
    public static native int GetHeight();
    
    
    public static native int init(int width, int height);
    public static native int deinit();
    public static native int draw();


}
