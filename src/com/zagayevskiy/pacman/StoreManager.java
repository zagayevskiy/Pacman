package com.zagayevskiy.pacman;

import android.content.Context;
import android.content.SharedPreferences;

public class StoreManager {
	
	public static final String PACMAN_PREFERENCES = "com_zagayevskiy_pacman_store";
	
	private Context context;
	
	public StoreManager(Context _context){
		context = _context;
	}
	
	public void saveBoolean(String key, boolean value){
		SharedPreferences sp = context.getSharedPreferences(PACMAN_PREFERENCES, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sp.edit();
		editor.putBoolean(key, value);
		editor.commit();
	}
	
	public boolean loadBoolean(String key, boolean defValue){
		SharedPreferences sp = context.getSharedPreferences(PACMAN_PREFERENCES, Context.MODE_PRIVATE);
		return sp.getBoolean(key, defValue);
	}
	
	public void saveInt(String key, int value){
		SharedPreferences sp = context.getSharedPreferences(PACMAN_PREFERENCES, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sp.edit();
		editor.putInt(key, value);
		editor.commit();
	}
	
	public int loadInt(String key, int defValue){
		SharedPreferences sp = context.getSharedPreferences(PACMAN_PREFERENCES, Context.MODE_PRIVATE);
		return sp.getInt(key, defValue);
	}
	
	public void saveString(String key, String value){
		SharedPreferences sp = context.getSharedPreferences(PACMAN_PREFERENCES, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sp.edit();
		editor.putString(key, value);
		editor.commit();
	}
	
	public String loadString(String key, String defValue){
		SharedPreferences sp = context.getSharedPreferences(PACMAN_PREFERENCES, Context.MODE_PRIVATE);
		return sp.getString(key, defValue);
	}
}
