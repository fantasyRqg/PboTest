package rqg.fantasy.pbotest;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceView;

/**
 * *Created by rqg on 7/13/17 1:52 PM.
 */

public class VideoSurfaceView extends SurfaceView {
    private static final String TAG = "VideoSurfaceView";

    private static final float W_H_RATIO = 16.0f / 9.0f;

    public VideoSurfaceView(Context context) {
        super(context);
    }

    public VideoSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public VideoSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public VideoSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }


    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {

        float w = MeasureSpec.getSize(widthMeasureSpec);
        float h = MeasureSpec.getSize(heightMeasureSpec);
        if (w / h >= W_H_RATIO) {
            w = W_H_RATIO * h;
        } else {
            h = w / W_H_RATIO;
        }

        Log.d(TAG, "onMeasure() called with: widthMeasureSpec = [" + w + "], heightMeasureSpec = [" + h + "]");
        setMeasuredDimension((int) w, (int) h);

    }
}
