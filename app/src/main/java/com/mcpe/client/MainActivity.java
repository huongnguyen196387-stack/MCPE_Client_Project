package com.mcpe.client;

import android.app.Activity;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {

    private ClientView view;

    @Override
    protected void onCreate(Bundle state) {
        super.onCreate(state);

        view = new ClientView();
        setContentView(view);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (view != null) view.onResume();
    }

    @Override
    protected void onPause() {
        if (view != null) view.onPause();
        super.onPause();
    }

    private class ClientView extends GLSurfaceView {

        ClientView() {
            super(MainActivity.this);

            setEGLContextClientVersion(3);
            setPreserveEGLContextOnPause(true);
            setRenderer(new ClientRenderer());
            setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        }

        @Override
        public boolean onTouchEvent(MotionEvent e) {
            float x = e.getX();
            float y = e.getY();

            switch (e.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                    NativeBridge.nativeTouch(x, y, true);
                    return true;

                case MotionEvent.ACTION_MOVE:
                    NativeBridge.nativeMove(x, y);
                    return true;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    NativeBridge.nativeTouch(x, y, false);
                    return true;
            }

            return true;
        }
    }

    private static class ClientRenderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            NativeBridge.nativeInit();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            NativeBridge.nativeResize(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            NativeBridge.nativeRender();
        }
    }

    public static class NativeBridge {

        static {
            System.loadLibrary("mcpe_client");
        }

        public static native void nativeInit();
        public static native void nativeResize(int width, int height);
        public static native void nativeRender();
        public static native void nativeTouch(float x, float y, boolean down);
        public static native void nativeMove(float x, float y);
    }
}
