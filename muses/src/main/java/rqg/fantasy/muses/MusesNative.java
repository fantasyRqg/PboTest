package rqg.fantasy.muses;

import android.content.res.AssetManager;
import android.util.Log;
import android.view.Surface;

/**
 * *Created by rqg on 7/7/17 10:57 AM.
 */

public class MusesNative {
    private static final String TAG = "MusesNative";

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native static String stringFromJNI();
//
//    public native static void testMuses(AssetManager assetManager);
    public native static void onSurfaceCreated(Surface surface, AssetManager assetManager);

    public native static void onFrameAvailable(long nstHandler);

    public native static boolean nativeInit();

    static {
        System.loadLibrary("native-lib");

        boolean ok = nativeInit();

        Log.d(TAG, "static initializer: " + Boolean.toString(ok));
    }
}
