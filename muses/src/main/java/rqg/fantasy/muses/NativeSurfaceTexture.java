package rqg.fantasy.muses;

import android.graphics.SurfaceTexture;

/**
 * * Created by rqg on 11/07/2017.
 */

public class NativeSurfaceTexture extends SurfaceTexture implements SurfaceTexture.OnFrameAvailableListener {
    public NativeSurfaceTexture(int texName) {
        super(texName);
        setOnFrameAvailableListener(this);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {

    }
}
