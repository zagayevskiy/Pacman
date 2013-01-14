package com.zagayevskiy.pacman;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.app.Activity;
import android.content.res.AssetManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.widget.TextView;

public class PacmanActivity extends Activity{
	
	PacmanView pacmanView;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
       super.onCreate(savedInstanceState);
      
       pacmanView = new PacmanView(getApplication());
       setContentView(pacmanView);   
       
    }

	/*
	 GL2JNIView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new GL2JNIView(getApplication());
        setContentView(mView);
    }
	*/
    @Override
    protected void onPause() {
        //  is called when a different Activity instance is going to be visible 
        //        and the current Activity has stopped interacting with the user
        super.onPause();
        Log.i(PacmanLib.tag, "PacmanActivity.onPause()");
        PacmanLib.stop();
        //pacmanView.onPause();

    }
    

    @Override
    protected void onResume() {
        //is called when the Activity object and its views become interactive with the user
        super.onResume();
        //pacmanView.onResume();
    }
    
    @Override
    protected void onStop() {
        //is called when an activity is no longer visible to, or interacting with, the user
        super.onStop();
        Log.i(PacmanLib.tag, "PacmanActivity.onStop()");
        PacmanLib.stop();
    }
    
    @Override
    protected void onDestroy(){	
    	super.onDestroy();
    	Log.i(PacmanLib.tag, "PacmanActivity.onDestroy()");
    	PacmanLib.free();
    }
    
    public boolean onKeyDown(int keyCode, KeyEvent event) {
	        if (keyCode == KeyEvent.KEYCODE_BACK) {
	        		if(PacmanLib.stop()){
	        			finish();
	        		}
	                return true;
	        }
	    return super.onKeyDown(keyCode, event);
	}
}
