package rqg.fantasy.muses;

import android.content.res.AssetManager;
import android.view.Surface;

/**
 * *Created by rqg on 7/7/17 10:57 AM.
 */

public class Native {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native static String stringFromJNI();

    public native static void testMuses(AssetManager assetManager);

    public native static void onSurfaceCreated(AssetManager assetManager, Surface surface);


    static {
        System.loadLibrary("native-lib");
    }
}
