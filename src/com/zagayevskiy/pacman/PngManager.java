package com.zagayevskiy.pacman;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class PngManager
{
    private AssetManager amgr;
    
    public PngManager(AssetManager manager){
    	amgr = manager;
    }

    public Bitmap open(String path)
    {
        try
        {
            return BitmapFactory.decodeStream(amgr.open(path));
        }
        catch (Exception e) { }
        return null;
    }

    public int getWidth(Bitmap bmp) {
    	return bmp.getWidth(); 
    }
    
    public int getHeight(Bitmap bmp) {
    	return bmp.getHeight(); 
    }

    public void getPixels(Bitmap bmp, int[] pixels)
    {
        int w = bmp.getWidth();
        int h = bmp.getHeight();
        bmp.getPixels(pixels, 0, w, 0, 0, w, h);
    }

    public void close(Bitmap bmp)
    {
        bmp.recycle();
    }
}