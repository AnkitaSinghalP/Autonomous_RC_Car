package com.example.shilpa.selfdrivingcar;

import android.app.Application;

/**
 * Created by shilpa on 19-Dec-16.
 */
public class BluetoothApplication extends Application {
    public BluetoothConnect bluetoothConnect;
    @Override
    public void onCreate()
    {
        super.onCreate();
        bluetoothConnect = new BluetoothConnect();
    }
}
