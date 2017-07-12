package rqg.fantasy.muses;

import android.graphics.SurfaceTexture;
import android.util.Log;
import android.view.Surface;

/**
 * * Created by rqg on 11/07/2017.
 */


/**
 * used in native , keep class name and method on Proguard
 */
public class NativeSurfaceTexture extends SurfaceTexture implements SurfaceTexture.OnFrameAvailableListener {
    private static final String TAG = "NativeSurfaceTexture";

    private long mNativeHandler;
    private Surface mSurface;

    public NativeSurfaceTexture(int texName) {
        super(texName);
        setOnFrameAvailableListener(this);
        mSurface = new Surface(this);
        Log.d(TAG, "NativeSurfaceTexture: " + texName);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        Log.d(TAG, "onFrameAvailable: " + surfaceTexture.getTimestamp());
        MusesNative.onFrameAvailable(mNativeHandler);
    }
}
