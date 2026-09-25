package com.mcpe.client;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("mcpe_client");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
}
