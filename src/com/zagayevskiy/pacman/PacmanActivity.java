package com.zagayevskiy.pacman;

import android.os.Bundle;
import android.app.Activity;
import android.view.KeyEvent;

public class PacmanActivity extends Activity{
	
	PacmanView pacmanView;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
       super.onCreate(savedInstanceState);
      
       pacmanView = new PacmanView(getApplication());
       setContentView(pacmanView);   
       
    }

    @Override
    protected void onPause() {
        super.onPause();
        PacmanLib.stop();
    }
    

    @Override
    protected void onResume() {
        super.onResume();
    }
    
    @Override
    protected void onStop() {
        super.onStop();
        PacmanLib.free();
        finish();
    }
    
    @Override
    protected void onDestroy(){	
    	super.onDestroy();
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
