package com.example.mana1991.selfdrivingcar;

import android.Manifest;
import android.app.Activity;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;

import com.google.android.gms.maps.*;
import com.google.android.gms.maps.model.*;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.MapView;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import static android.bluetooth.BluetoothAdapter.getDefaultAdapter;

public class MainActivity extends FragmentActivity implements OnMapReadyCallback {
    GoogleMap googleMap;
    Button connect;
    Button start;
    Button end;
    Context context;
    int REQUEST_ENABLE_BT = 1;
    ListView listView1 ;
    LinearLayout listviewble;
    LinearLayout progressBar;
    ArrayAdapter<BluetoothDevice> arrayAdapter;
    BluetoothDevice device;
    ProgressBar pbheader;
    ArrayList<String> bleList;
    BluetoothAdapter bleAdapter;
    //SupportMapFragment mapFragment;
    MapFragment mapFragment;
    Marker TP;

    ConnectThread mConnectThread;
    ConnectedThread mConnectedThread;
    ConnectThread1 mConnectThread1;
    ConnectedThread1 mConnectedThread1;
    //AcceptThread mAcceptThread;
    //BluetoothDevice mmDevice;
    BluetoothSocket socket = null;
    BluetoothSocket tmp ;
    String message = "Manali";
    String message_1 = "1";
    String message_2 = "0";
    String NAME = "FireFly-BE12";
    public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    ArrayList<BluetoothDevice> pairedDeviceArrayList;
    BluetoothDevice device1;
    int len;



    //static final LatLng TutorialsPoint = new LatLng(21 , 57);

        // Get the message bytes and tell the BluetoothChatService to write



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.activity_main);

        setResult(Activity.RESULT_CANCELED);

        progressBar = (LinearLayout)findViewById(R.id.progressbar);
        listviewble = (LinearLayout)findViewById(R.id.listviewble);

        //listView1 = new ListView()



        pbheader = (ProgressBar)findViewById(R.id.pbHeaderProgress);
        mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);

        mapFragment.getMapAsync(this);
        connect = (Button) findViewById(R.id.button1);
        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                connect();
                //view.setVisibility(View.GONE);
            }
        });

//        start = (Button)findViewById(R.id.button2);
//        start.setOnClickListener(new View.OnClickListener(){
//            @Override
//                    public void onClick(View view){
//                    start();
//            }
//        });
//
//        end = (Button)findViewById(R.id.button3);
//        end.setOnClickListener(new View.OnClickListener(){
//            @Override
//            public void onClick(View view){
//                end();
//            }
//        });

        bleAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bleAdapter == null){
            Toast. makeText(MainActivity.this,"Bluetooth not available",Toast.LENGTH_SHORT).show();
        }
//
//        final Set<BluetoothDevice> pairedDevices = bleAdapter.getBondedDevices();
//        if (pairedDevices.size() > 0) {
//            pairedDeviceArrayList = new ArrayList<BluetoothDevice>();
//
//            for (BluetoothDevice device : pairedDevices) {
//                pairedDeviceArrayList.add(device);
//            }
//        }
//
//        arrayAdapter = new ArrayAdapter<BluetoothDevice>(this,android.R.layout.simple_list_item_1,pairedDeviceArrayList);
//        listView1 = (ListView)findViewById(R.id.listView1);
//        listviewble.setVisibility(View.VISIBLE);
//        listView1.setVisibility(View.VISIBLE);
//        listView1.setAdapter(arrayAdapter);
//        listView1.setOnItemClickListener(new AdapterView.OnItemClickListener(){
//            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l){
//
//                device1 = (BluetoothDevice)adapterView.getItemAtPosition(i);
//
//                mConnectThread = new ConnectThread(device1);
//                mConnectThread.start();
//
//
//
//            }
//        });



    }

    @Override
    protected void onDestroy(){
        super.onDestroy();

        if(bleAdapter != null){
            bleAdapter.cancelDiscovery();
        }

        this.unregisterReceiver(mReceiver);
    }

    @Override
    public void onMapReady(GoogleMap map) {
        LatLng sydney = new LatLng(37.3352, -121.8811);
        //LatLng sydney = new LatLng(-34, 151);
       // mMap.addMarker(new MarkerOptions().position(sydney).title("Marker in Sydney"));
        map.moveCamera(CameraUpdateFactory.newLatLngZoom(sydney, 15));
       // map.moveCamera( CameraUpdateFactory.newLatLngZoom(new LatLng(37.3352, -121.8811) , 14) );

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        map.setMyLocationEnabled(true);
        //map.moveCamera(CameraUpdateFactory.newLatLngZoom(sydney, 13));

        //map.addMarker(new MarkerOptions().title("Sydney").snippet("The most populous city in Australia.").position(sydney));
    }

    public boolean createBond(BluetoothDevice btDevice)
            throws Exception
    {
        Class class1 = Class.forName("android.bluetooth.BluetoothDevice");
        Method createBondMethod = class1.getMethod("createBond");
        Boolean returnValue = (Boolean) createBondMethod.invoke(btDevice);
        return returnValue.booleanValue();
    }

    private void connect()
    {
        progressBar.setVisibility(View.VISIBLE);
        setProgressBarIndeterminateVisibility(true);

        if(!bleAdapter.isEnabled()){
            Intent bleIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(bleIntent, REQUEST_ENABLE_BT);
                       }

        if(bleAdapter.isDiscovering()){
            bleAdapter.cancelDiscovery();
        }
        bleAdapter.startDiscovery();

        final Set<BluetoothDevice> pairedDevices = bleAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            pairedDeviceArrayList = new ArrayList<BluetoothDevice>();

            for (BluetoothDevice device : pairedDevices) {
                pairedDeviceArrayList.add(device);
            }
        }

        progressBar.setVisibility(View.INVISIBLE);
        setProgressBarIndeterminateVisibility(false);

       // Dialog dialog = new Dialog(MainActivity.this);
        //dialog.setContentView(R.layout.activity_main);

        arrayAdapter = new ArrayAdapter<BluetoothDevice>(this,android.R.layout.simple_list_item_1,pairedDeviceArrayList);
        listView1 = (ListView)findViewById(R.id.listView1);
        listviewble.setVisibility(View.VISIBLE);
        listView1.setVisibility(View.VISIBLE);
        listView1.setAdapter(arrayAdapter);
        //dialog.setCancelable(true);
        //dialog.setTitle("Paired Devices");
        //dialog.show();
        listView1.setOnItemClickListener(new AdapterView.OnItemClickListener(){
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l){

                device1 = (BluetoothDevice)adapterView.getItemAtPosition(i);
                mConnectThread = new ConnectThread(device1);
                mConnectThread.start();

                //mConnectThread = new ConnectThread(device1);
                //mConnectThread.start();



            }
        });



    }

    private void start(){

//        mConnectThread = new ConnectThread(device1);
//        mConnectThread.start();



    }

    private void end(){

        mConnectThread1 = new ConnectThread1(device1);
        mConnectThread1.start();


    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
//                progressBar.setVisibility(View.INVISIBLE);
//                setProgressBarIndeterminateVisibility(false);
               // device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                //arrayAdapter.add(device.getName() + "\n" + device.getAddress() + device);
//                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
//                    arrayAdapter.add(device.getName() + "\n" + device.getAddress());
//                   //arrayListBluetoothDevices.add(device);
//                }
//                else {
//
//                }
//
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                setProgressBarIndeterminateVisibility(false);
                if(arrayAdapter.getCount() == 0){
                    String noDevices = getResources().getText(R.string.none_found).toString();
                    //arrayAdapter.add(noDevices);
                }
            }
        }
    };

//    private class AcceptThread extends Thread {
//        private final BluetoothServerSocket mmServerSocket;
//
//        public AcceptThread() {
//            // Use a temporary object that is later assigned to mmServerSocket,
//            // because mmServerSocket is final
//            BluetoothServerSocket tmp = null;
//            try {
//                // MY_UUID is the app's UUID string, also used by the client code
//                tmp = bleAdapter.listenUsingRfcommWithServiceRecord(NAME, MY_UUID);
//            } catch (IOException e) { }
//            mmServerSocket = tmp;
//        }
//
//        public void run() {
//            Log.i("Tag","Reached accept run");
//            // Keep listening until exception occurs or a socket is returned
//            while (true) {
//                try {
//                    Log.i("Tag","Reached accept socket");
//                    socket = mmServerSocket.accept();
//                } catch (IOException e) {
//                    break;
//                }
//                // If a connection was accepted
//                if (socket != null) {
//                    Log.i("Tag","Reached connected accept");
//
//                }
//                else{
//                    Log.i("Tag","socket null");
//                }
//            }
//        }
//
//        /** Will cancel the listening socket, and cause the thread to finish */
//        public void cancel() {
//            try {
//                mmServerSocket.close();
//            } catch (IOException e) { }
//        }
//    }

    public class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;
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
            } catch (IOException e) {
                e.printStackTrace();
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

    public class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            Log.i("Tag","Reached connected thread");
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
            //Log.i("Tag","Reached connected run");

            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while(true) {
                start = (Button) findViewById(R.id.button2);
                start.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        int count1 = 0;
                        while (count1 < 10) {
                            byte[] send = message_1.getBytes();
                            mConnectedThread.write(send);
                            count1++;
                        }
                    }
                });

                end = (Button) findViewById(R.id.button3);
                end.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        int count = 0;
                        while (count <= 10) {
                            Log.i("Tag", "Writing");
                            byte[] send = message_2.getBytes();
                            mConnectedThread.write(send);
                            count++;
                        }
                    }
                });
            }

//            int len1 = message_1.length();
//            Log.i("len1 ", String.valueOf(len1));
//            int count1 = 0;
//            while(count1<10) {
//                byte[] send = message_1.getBytes();
//                mConnectedThread.write(send);
//                count1++;
//            }
        }

        public void write(byte[] bytes) {
            Log.i("Tag","Reached connected write");
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


    public class ConnectThread1 extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;
        public ConnectThread1(BluetoothDevice device) {

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
            } catch (IOException e) {
                e.printStackTrace();
            }
            mConnectedThread1 = new ConnectedThread1(mmSocket);
            mConnectedThread1.start();
        }
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }

    public class ConnectedThread1 extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread1(BluetoothSocket socket) {
            Log.i("Tag","Reached connected thread");
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
            //Log.i("Tag","Reached connected run");

            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs

            int len2 = message_2.length();
            Log.i("len2 ", String.valueOf(len2));
//            int count = 0;
//            while(count<=10){
//                Log.i("Tag","Writing");
//                byte[] send = message_2.getBytes();
//                mConnectedThread.write(send);
//                count++;
//            }
//            len = message.length();
//            while (len>0) {
//                    byte[] send = message.getBytes();
//                    mConnectedThread.write(send);
//                    len--;
//                }
//                try {
//                    // Read from the InputStream
//                    bytes = mmInStream.read(buffer);
//                    // Send the obtained bytes to the UI activity
//                   // mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer).sendToTarget();
//                    Log.i("Bytes", String.valueOf(bytes));
//
//                } catch (IOException e) {
//                    break;
//                }


        }

        public void write(byte[] bytes) {
            Log.i("Tag","Reached connected write");
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





