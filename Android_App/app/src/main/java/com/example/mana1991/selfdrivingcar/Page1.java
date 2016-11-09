package com.example.mana1991.selfdrivingcar;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Message;
import android.os.ParcelUuid;
import android.os.Parcelable;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.UUID;
import java.util.logging.Handler;


public class Page1 extends AppCompatActivity {

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */

    String send_device;
    BluetoothDevice mDevice;
   // BluetoothSocket socket;
    InputStream instream;
    OutputStream outstream;
    String message = "1";
    TextView data;
    BluetoothAdapter send_adapter;
    ConnectThread mConnectThread;
    ConnectedThread mConnectedThread;
    BluetoothDevice mmDevice;
    BluetoothSocket tmp ;
    Parcelable[] uuidExtra;

//    Handler mHandler = new Handler() {
//        @Override
//        public void handleMessage(Message msg) {
//            byte[] writeBuf = (byte[]) msg.obj;
//            int begin = (int)msg.arg1;
//            int end = (int)msg.arg2;
//
//            switch(msg.what) {
//                case 1:
//                    String writeMessage = new String(writeBuf);
//                    writeMessage = writeMessage.substring(begin, end);
//                    break;
//            }
//        }
//    };

    @RequiresApi(api = Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_page1);

        data = (TextView) findViewById(R.id.data1);

        mDevice = getIntent().getExtras().getParcelable("send_device");

        mConnectThread = new ConnectThread(mDevice);
        mConnectThread.start();

    }

    @RequiresApi(api = Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1)
    private class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;
        public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        public ConnectThread(BluetoothDevice device) {

            if(device == null)
            {
                Log.i("Status","Device is null");
            }

            mmDevice = device;
            try {
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) { }
            mmSocket = tmp;
        }
        public void run() {
            Log.i("Tag","Reached connect run");
            try {
                mmSocket.connect();
            } catch (IOException connectException) {
                try {
                    mmSocket.close();
                } catch (IOException closeException) { }
                return;
            }

            mConnectedThread = new ConnectedThread(mmSocket);
            mConnectedThread.start();
        }
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }

    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
            }
            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            Log.i("Tag","Reached connected run");
            byte[] buffer = new byte[1024];
            int begin = 0;
            int bytes = 0;
            int len = message.length();
            while (len>0) {
                write(message.getBytes());
                data.setText(message);
                len--;
//                    bytes += mmInStream.read(buffer, bytes, buffer.length - bytes);
//                    for (int i = begin; i < bytes; i++) {
//                        if (buffer[i] == "#".getBytes()[0]) {
//                            mHandler.handleMessage(1, begin, i, buffer).sendToTarget();
//                            begin = i + 1;
//                            if (i == bytes - 1) {
//                                bytes = 0;
//                                begin = 0;
//                            }
//                        }
//                    }
            }
            mConnectedThread = new ConnectedThread(mmSocket);
            mConnectedThread.start();

        }

        public void write(byte[] bytes) {
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) {
            }
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
            }

        }
    }

    }

