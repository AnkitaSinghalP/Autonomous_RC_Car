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
import android.graphics.Color;
import android.os.AsyncTask;
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

import com.google.android.gms.location.LocationListener;
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

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.math.RoundingMode;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import android.os.Handler;
import java.util.logging.LogRecord;

public class MainActivity extends FragmentActivity implements OnMapReadyCallback{
    GoogleMap map1_main;
    Button connect;
    Button start;
    Button end;
    int REQUEST_ENABLE_BT = 1;
    ListView listView1 ;
    LinearLayout listviewble;
    LinearLayout progressBar;
    LinearLayout status;
    ArrayAdapter<BluetoothDevice> arrayAdapter;
   // BluetoothDevice device;
    ProgressBar pbheader;
    //ArrayList<String> bleList;
    BluetoothAdapter bleAdapter;
    //SupportMapFragment mapFragment;
    MapFragment mapFragment;
   // Marker TP;
    ArrayList markers;
    String data = "";
    InputStream iStream;
    HttpURLConnection urlConnection = null;

    ConnectThread mConnectThread;
    ConnectedThread mConnectedThread;
//    ConnectThread1 mConnectThread1;
//    ConnectedThread1 mConnectedThread1;
   // BluetoothSocket socket = null;
    BluetoothSocket tmp ;
   // String message = "Manali";
    String message_1 = "1";
    String message_2 = "0";
   // String NAME = "FireFly-BE12";
    public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    ArrayList<BluetoothDevice> pairedDeviceArrayList;
    BluetoothDevice device1;
   // int len;

    ArrayList points = null;
    PolylineOptions lineoptions = null;
    MarkerOptions markerOptions = null;
    MarkerOptions options = null;

    boolean enter_flag = false;
  //  bool exit_flag =
   // MarkerOptions check_pt = null;

    byte[] buffer = null;
   // int bytes;
    List<LatLng> list = null;
    List<LatLng>pack_checkpt = null;

    int readBufferPosition = 0;
    char delimiter = 0x23;
    char delimiter1 = 0x2A;
    char delimiter2 = 0x62;
    char delimiter3 = 0x69;
    char delimiter4 = 0x73;
    char delimiter5 = 0x6D;
    char delimiter6 = 0x6C;
    byte[] readBuffer;
    int bytesAvailable=0;
    TextView text_display;
    String data1;

    Intent bleIntent;
    Set<BluetoothDevice> pairedDevices;
    BluetoothSocket mmSocket;
    InputStream mmInStream;
    OutputStream mmOutStream;

    TextView bleid;
    TextView iostatus;
    TextView masterstatus;
    TextView motorstatus;
    TextView sensorstatus;
    TextView blevalue;
    TextView iovalue;
    TextView mastervalue;
    TextView motorvalue;
    TextView sensorrvalue;

    String ble_status;
    String io_status;
    String mas_status;
    String mot_status;
    String sen_status;

    //public final Handler handler = null;
    //public final Handler handler = new Handler(){

    Handler handler = new Handler();
    Runnable r = new Runnable() {
        public void run() {

            status.setVisibility(View.VISIBLE);
            //text_display.setText(data1);
            blevalue.setText(ble_status);
            iovalue.setText(io_status);
            mastervalue.setText(mas_status);
            motorvalue.setText(mot_status);
            sensorrvalue.setText(sen_status);

           // system_status();
            //tv.append("Hello World");
           // handler.postDelayed(this, 1000);
        }
    };






    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.activity_main);

        setResult(Activity.RESULT_CANCELED);

        Handler handler = new Handler();
        handler.postDelayed(new Runnable(){
            @Override
            public void run(){

            }
        }, 5);

        progressBar = (LinearLayout) findViewById(R.id.progressbar);
        listviewble = (LinearLayout) findViewById(R.id.listviewble);
        status = (LinearLayout) findViewById(R.id.status);

        //text_display = (TextView) findViewById(R.id.display1);

        bleid =(TextView) findViewById(R.id.bleid);
        iostatus = (TextView) findViewById(R.id.iostatus);
        masterstatus = (TextView) findViewById(R.id.masterstatus);
        motorstatus = (TextView) findViewById(R.id.motorstatus);
        sensorstatus = (TextView) findViewById(R.id.sensorstatus);

        blevalue = (TextView) findViewById(R.id.blevalue);
        iovalue = (TextView) findViewById(R.id.iovalue);
        mastervalue = (TextView) findViewById(R.id.mastervalue);
        motorvalue = (TextView) findViewById(R.id.motorvalue);
        sensorrvalue = (TextView) findViewById(R.id.sensorrvalue);

        pbheader = (ProgressBar) findViewById(R.id.pbHeaderProgress);
        start = (Button) findViewById(R.id.button2);
        end = (Button) findViewById(R.id.button3);

        mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);
        mapFragment.getView().setVisibility(View.GONE);
        mapFragment.getMapAsync(this);

        bleAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bleAdapter == null) {
            Toast.makeText(MainActivity.this, "Bluetooth not available", Toast.LENGTH_SHORT).show();
        }

//        if(!bleAdapter.isEnabled()){
//            bleIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
//            startActivityForResult(bleIntent, REQUEST_ENABLE_BT);
//        }

        connect = (Button) findViewById(R.id.button1);
        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mapFragment.getView().setVisibility(View.GONE);



//                bleAdapter = BluetoothAdapter.getDefaultAdapter();
//                if (bleAdapter == null) {
//                    Toast.makeText(MainActivity.this, "Bluetooth not available", Toast.LENGTH_SHORT).show();
//                }
                connect();
            }
        });
//        start = (Button) findViewById(R.id.button2);
//        start.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
////                int count1 = 0;
////                while (count1 < 10) {
////                    byte[] send = message_1.getBytes();
////                    mConnectedThread.write(send);
////                    count1++;
////                }
//                for(int i=0; i<pack_checkpt.size(); i++){
//                    LatLng send1 = pack_checkpt.get(i);
//                    double send2 = send1.latitude;
//                    double send3 = send1.longitude;
//                    StringBuffer buffer1 = new StringBuffer();
//                    buffer1.append(",");
//                    buffer1.append(send2);
//                    buffer1.append(",");
//                    buffer1.append(send3);
//
//
//                    Log.i("Buffer", String.valueOf(buffer1));
//
//                    //  String sending_lat = String.valueOf(send2);
//                    // String sending_lng = String.valueOf(send3);
//                    //byte[] sending2 = sending_lat.getBytes();
//                    String sending3 = buffer1.toString();
//                    Log.i("Sending3",sending3);
//                    //byte[] sending2 = sending3.getBytes();
//                    // mConnectedThread.write(sending2);
//                    //  mConnectedThread.write(sending3);
//                }
//            }
//        });
    }



        @Override
        protected void onDestroy () {
            super.onDestroy();

            if (bleAdapter != null) {
                bleAdapter.cancelDiscovery();
            }

            //this.unregisterReceiver(mReceiver);
        }


        @Override
        public void onMapReady (GoogleMap map1){

            map1_main =map1;

            markers = new ArrayList();
            LatLng sydney = new LatLng(37.3352, -121.8811);


            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                return;
            }

            map1_main.moveCamera(CameraUpdateFactory.newLatLngZoom(sydney, 15));
            if (map1_main != null) {
                map1_main.setMyLocationEnabled(true);

                map1_main.setOnMapClickListener(new GoogleMap.OnMapClickListener() {
                    @Override
                    public void onMapClick(LatLng latLng) {

                        if (markers.size() > 1) {
                            markers.clear();
                            map1_main.clear();
                        }
                       // Log.i("Latlng", String.valueOf(latLng.latitude));
                        // Log.i("Latlng1", String.valueOf(latLng.longitude));
                        markers.add(latLng);

                        options = new MarkerOptions();

                        options.position(latLng);

                        if (markers.size() == 1) {
                            options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN));
                        } else if (markers.size() == 2) {
                            options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED));
                        }

                        map1_main.addMarker(options);

                        if (markers.size() >= 2) {
                            LatLng start = (LatLng) markers.get(0);
                            LatLng dest = (LatLng) markers.get(1);

                            Log.i("start Lat", String.valueOf(start.latitude));
                            Log.i("start Lng", String.valueOf(start.longitude));
                            String url = getDirectionURL(start, dest);

                            DownloadTask downloadTask = new DownloadTask();

                            downloadTask.execute(url);
                        }

                    }
                });

            }
        }


    private String getDirectionURL(LatLng start, LatLng dest){

        String str_start = "origin="+start.latitude+","+start.longitude;
        String str_dest = "destination="+dest.latitude+","+dest.longitude;
        String sensor = "sensor=false";
        String mode = "mode=walking";
        String key = "key=AIzaSyAMhbUYMVutFkNJL40g9950Zj5iAbjOAr8";
        String parameters = str_start+"&"+str_dest+"&"+sensor+"&"+mode;
        String output = "json";

       // String url = "https://www.google.com/maps/dir/101+E+San+Fernando+St,+San+Jose,+CA/Santa+Clara+North,+San+Jose,+CA+95113/@37.3357457,-121.8908007,17z/data=!3m1!4b1!4m13!4m12!1m5!1m1!1s0x808fccbc70d3be87:0x91aecd27fd05d553!2m2!1d-121.8867076!2d37.3357192!1m5!1m1!1s0x808fccbccd1971f3:0x24c81ef1bfaa5435!2m2!1d-121.8901564!2d37.33564";

       String url = "https://maps.googleapis.com/maps/api/directions/"+output+"?"+parameters;
        Log.i("url",url);

        return url;

    }

    private class DownloadTask extends AsyncTask<String, String, String>{
        @Override
        protected String doInBackground(String... url){
            String data = "";
            try {
                data = downloadUrl(url[0]);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return data;
        }

        @Override
        protected void onPostExecute(String result){
            super.onPostExecute(result);

            ParserTask parserTask = new ParserTask();
            parserTask.execute(result);
        }
    }

    private String downloadUrl(String str_url)throws IOException{

        try{
            URL url = new URL(str_url);
            urlConnection = (HttpURLConnection)url.openConnection();
            urlConnection.connect();

            iStream = urlConnection.getInputStream();

            BufferedReader br = new BufferedReader(new InputStreamReader(iStream));

            StringBuffer sb = new StringBuffer();
            String line = "";
            while((line = br.readLine())!= null){
                sb.append(line);
            }

            data = sb.toString();

            br.close();
        }catch (Exception e){
            Log.d("Exception while downloading url", e.toString());
        }finally{
        }
        return data;
        }

    private class ParserTask extends AsyncTask<String,Integer,List<List<HashMap<String,String>>> >{
        @Override
        protected List<List<HashMap<String, String>>> doInBackground(String... jsonData) {
            JSONObject jsonObject = null;
            List<List<HashMap<String, String>>> routes = null;

            try {
                jsonObject = new JSONObject(jsonData[0]);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            DirectionParser directionParser = new DirectionParser();

            routes = directionParser.parse(jsonObject);
            return routes;
        }

        @Override
        protected void onPostExecute(List<List<HashMap<String, String>>> result){

            markerOptions = new MarkerOptions();

            int i,j,index=1;
            for(i=0;i<result.size();i++) {
                points = new ArrayList<LatLng>();
                lineoptions = new PolylineOptions();
                pack_checkpt = new ArrayList<LatLng>();

                List<HashMap<String, String>> path = result.get(i);
                for (j = 0; j < path.size() ; j++) {

                    HashMap<String, String> point = path.get(j);

                    double lat = Double.parseDouble(point.get("lat"));
                    Log.i("Lat", String.valueOf(lat));
                    double lng = Double.parseDouble(point.get("lng"));
                    Log.i("Lng", String.valueOf(lng));
                    LatLng position = new LatLng(lat, lng);

                    while(index < path.size()) {
                        HashMap<String, String> point1 = path.get(index);

                        DecimalFormat df = new DecimalFormat("#.######");
                        df.setRoundingMode(RoundingMode.CEILING);

                        double lat1 = Double.parseDouble(point1.get("lat"));
                        Log.i("Lat", String.valueOf(lat));
                        double lng1 = Double.parseDouble(point1.get("lng"));
                        Log.i("Lng", String.valueOf(lng));
                        LatLng position1 = new LatLng(lat1, lng1);
                        pack_checkpt.add(new LatLng(lat1,lng1));

                        Log.i("Path size", String.valueOf(path.size()));
                        for(int r=0; r<pack_checkpt.size();r++){
                            Log.i("Check pt list", String.valueOf(pack_checkpt.get(r)));
                        }

                        map1_main.addMarker(new MarkerOptions().position(position1).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE)));
                    index = index + 1;
                    }

                    points.add(position);
                }

                // Adding all the points in the route to LineOptions
                lineoptions.addAll(points);
                lineoptions.width(10);
                lineoptions.color(Color.RED);

                }
            map1_main.addPolyline(lineoptions);
        }
    }

    // Drawing polyline in the Google Map for the i-th route
    public class DirectionParser {

        /** Receives a JSONObject and returns a list of lists containing latitude and longitude */
        public List<List<HashMap<String,String>>> parse(JSONObject jObject){

            List<List<HashMap<String, String>>> routes = new ArrayList<List<HashMap<String,String>>>() ;
            JSONArray jRoutes = null;
            JSONArray jLegs = null;
            JSONArray jSteps = null;

            try {

                jRoutes = jObject.getJSONArray("routes");

                /** Traversing all routes */
                for(int i=0;i<jRoutes.length();i++){
                    jLegs = ( (JSONObject)jRoutes.get(i)).getJSONArray("legs");
                    List path = new ArrayList<HashMap<String, String>>();

                    /** Traversing all legs */
                    for(int j=0;j<jLegs.length();j++){
                        jSteps = ( (JSONObject)jLegs.get(j)).getJSONArray("steps");

                        /** Traversing all steps */
                        for(int k=0;k<jSteps.length();k++){
                            String polyline = "";
                            polyline = (String)((JSONObject)((JSONObject)jSteps.get(k)).get("polyline")).get("points");
                            list = decodePoly(polyline);

                            /** Traversing all points */
                            for(int l=0;l<list.size();l++){

                                HashMap<String, String> hm = new HashMap<String, String>();
                                hm.put("lat", Double.toString(((LatLng)list.get(l)).latitude) );
                                hm.put("lng", Double.toString(((LatLng)list.get(l)).longitude) );
                                path.add(hm);
                            }
                            Log.i("Inside check point", String.valueOf(k));
                        }
                        routes.add(path);
                    }
                }

            } catch (JSONException e) {
                e.printStackTrace();
            }catch (Exception e){
            }
            return routes;
        }

        private List<LatLng> decodePoly(String encoded) {

            List<LatLng> poly = new ArrayList<LatLng>();
            int index = 0, len = encoded.length();
            int lat = 0, lng = 0;

            while (index < len) {
                int b, shift = 0, result = 0;
                do {
                    b = encoded.charAt(index++) - 63;
                    result |= (b & 0x1f) << shift;
                    shift += 5;
                } while (b >= 0x20);
                int dlat = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
                lat += dlat;

                shift = 0;
                result = 0;
                do {
                    b = encoded.charAt(index++) - 63;
                    result |= (b & 0x1f) << shift;
                    shift += 5;
                } while (b >= 0x20);
                int dlng = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
                lng += dlng;

                LatLng p = new LatLng((((double) lat / 1E5)),
                        (((double) lng / 1E5)));
                poly.add(p);
            }
            return poly;
        }
    }

    private void connect()
    {
        progressBar.setVisibility(View.VISIBLE);
        setProgressBarIndeterminateVisibility(true);

        if(!bleAdapter.isEnabled()){
            bleIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(bleIntent, REQUEST_ENABLE_BT);
        }

        if(bleAdapter.isDiscovering()){
            bleAdapter.cancelDiscovery();
        }
        bleAdapter.startDiscovery();


        pairedDevices = bleAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            pairedDeviceArrayList = new ArrayList<BluetoothDevice>();

            for (BluetoothDevice device : pairedDevices) {
                pairedDeviceArrayList.add(device);
            }
        }

        progressBar.setVisibility(View.INVISIBLE);
        setProgressBarIndeterminateVisibility(false);

        arrayAdapter = new ArrayAdapter<BluetoothDevice>(this,android.R.layout.simple_list_item_1,pairedDeviceArrayList);
        listView1 = (ListView)findViewById(R.id.listView1);
        listviewble.setVisibility(View.VISIBLE);
        listView1.setVisibility(View.VISIBLE);
        listView1.setAdapter(arrayAdapter);
        listView1.setOnItemClickListener(new AdapterView.OnItemClickListener(){
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l){

                status.setVisibility(View.INVISIBLE);
                device1 = (BluetoothDevice)adapterView.getItemAtPosition(i);
                mConnectThread = new ConnectThread(device1);
                mConnectThread.start();
                listviewble.setVisibility(View.INVISIBLE);
                listView1.setVisibility(View.INVISIBLE);
                mapFragment.getView().setVisibility(View.GONE);


            }
        });
    }

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
//        private final BluetoothSocket mmSocket;
//        private final InputStream mmInStream;
//        private final OutputStream mmOutStream;

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

            buffer = new byte[1024];  // buffer store for the stream
            //byte buffer;
            String a;
            String received = null;
             // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while(true) {

                start.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        enter_flag = true;
                        int count1 = 0;
                        while (count1 <= 5) {
                            byte[] send = message_1.getBytes();
                            mConnectedThread.write(send);
                            count1++;
                            //  Log.i("Sending 1", String.valueOf(send));
                        }
//
                    }
                });


                end.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        enter_flag = false;
                        int count = 0;
                        while (count <= 5) {
                            // Log.i("Tag", "Writing");
                            byte[] send = message_2.getBytes();
                            mConnectedThread.write(send);
                            count++;
                            // Log.i("Sending 0", String.valueOf(send));
                        }
                    }
                });

                //status.setVisibility(View.VISIBLE);

//                try {
//                   // bytes = mmInStream.read(buffer);
//
//                   // readBuffer = new byte[1024];
//                    bytesAvailable = mmInStream.available();
//                   // Log.i("bytes", String.valueOf(bytesAvailable));
//                    if(bytesAvailable > 0 && enter_flag == true) {
//                        byte[] packetBytes = new byte[bytesAvailable];
//                        mmInStream.read(packetBytes);
//                        byte b = packetBytes[0];
//                        if (b == delimiter1) {
//                            for (int i = 1; i < bytesAvailable; i++) {
//                                b = packetBytes[i];
//                                switch(i){
//                                    case 1: ble_status = new String(String.valueOf(b));
//                                        //blevalue.setText(ble_status);
//                                            //Log.i("Ble status ", ble_status);
//                                            break;
//
//                                    case 2: io_status = new String(String.valueOf(b));
//                                       // iovalue.setText(io_status);
//                                       // Log.i("IO status ", io_status);
//                                            break;
//
//                                    case 3: mas_status = new String(String.valueOf(b));
//                                       // mastervalue.setText(mas_status);
//                                       // Log.i("Master status ", mas_status);
//                                            break;
//
//                                    case 4: mot_status = new String(String.valueOf(b));
//                                       // motorvalue.setText(mot_status);
//                                       // Log.i("Motor status ", mot_status);
//                                            break;
//
//                                    case 5: sen_status = new String(String.valueOf(b));
//                                       // sensorrvalue.setText(sen_status);
//                                        //Log.i("Sensor status ", sen_status);
//                                            break;
//
//                                    default: i = 0;
//                                             break;
//                                }
//                            }
////                                b = packetBytes[i];
////                                if(b == delimiter2){
////                                     b = packetBytes[i++];
////                                     String ble_status = new String(String.valueOf(b));
////                                }
////                                if(b == delimiter3){
////
////                                }
//
////                                if (b == delimiter && b != 0x00) {
////                                    byte[] encodedBytes = new byte[readBufferPosition];
////                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
////                                    //final String data = new String(encodedBytes, "US-ASCII");
////                                    data1 = new String(encodedBytes, "US-ASCII");
////                                    readBufferPosition = 0;
////                                    Log.i("Buffer data", String.valueOf(data1));
////                                    //text_display.setText(data);
////                                    // Thread.sleep(1000);
////
////                                }
////                                else {
////
////                                    readBuffer[readBufferPosition++] = b;
////                                    //Log.i("read", String.valueOf(readBuffer[readBufferPosition]));
////                                }
//                            }
//                        }
//                    } catch (IOException e) {
//                    e.printStackTrace();
//                }
//
//                handler.postDelayed(r, 50);

//                runOnUiThread(new Runnable() {
//                                      @Override
//                                      public void run() {
//                                          listviewble.setVisibility(View.INVISIBLE);
//                                          status.setVisibility(View.VISIBLE);
//                                          //text_display.setText(data1);
//                                          blevalue.setText(ble_status);
//                                          iovalue.setText(io_status);
//                                          mastervalue.setText(mas_status);
//                                          motorvalue.setText(mot_status);
//                                          sensorrvalue.setText(sen_status);
//                                      }
//                                  });
////                         do
//                         {
//                             bytes = mmInStream.read(buffer);
//                             received += new String(buffer, 0, bytes);
//                             Log.i("Received bytes", received);
//                             //received.a
//                         }while(!(received.endsWith("#")));



                        //received += "";
//                    } catch (IOException e) {
//                    e.printStackTrace();
//                }


                //  received =  new String(buffer,0,bytes);




//                start.setOnClickListener(new View.OnClickListener() {
//                    @Override
//                    public void onClick(View view) {
//                        enter_flag = true;
//                        int count1 = 0;
//                        while (count1 <= 5) {
//                            byte[] send = message_1.getBytes();
//                            mConnectedThread.write(send);
//                            count1++;
//                          //  Log.i("Sending 1", String.valueOf(send));
//                        }
////
//                        }
//                    });
//
//
//                end.setOnClickListener(new View.OnClickListener() {
//                    @Override
//                    public void onClick(View view) {
//                        enter_flag = false;
//                        int count = 0;
//                        while (count <= 5) {
//                           // Log.i("Tag", "Writing");
//                            byte[] send = message_2.getBytes();
//                            mConnectedThread.write(send);
//                            count++;
//                           // Log.i("Sending 0", String.valueOf(send));
//                        }
//                    }
//                });
            }

        }



       // for(int i=0; i<pack_checkpt.size(); i++){
//                            LatLng send1 = pack_checkpt.get(i);
//                            double send2 = send1.latitude;
//                            double send3 = send1.longitude;
//                            StringBuffer buffer1 = new StringBuffer();
//                            buffer1.append(send2);
//                            buffer1.append("#");
//                            buffer1.append(send3);
//                            buffer1.append("#");
//
//                            Log.i("Buffer", String.valueOf(buffer1));
//
//                          //  String sending_lat = String.valueOf(send2);
//                           // String sending_lng = String.valueOf(send3);
//                            //byte[] sending2 = sending_lat.getBytes();
//                            String sending3 = buffer1.toString();
//                            Log.i("Sending3",sending3);
//                            byte[] sending2 = sending3.getBytes();
//                           // mConnectedThread.write(sending2);
//                          //  mConnectedThread.write(sending3);

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

    void system_status(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //listviewble.setVisibility(View.INVISIBLE);
                status.setVisibility(View.VISIBLE);
                //text_display.setText(data1);
                blevalue.setText(ble_status);
                iovalue.setText(io_status);
                mastervalue.setText(mas_status);
                motorvalue.setText(mot_status);
                sensorrvalue.setText(sen_status);
            }
        });
    }


//    public class ConnectThread1 extends Thread {
//        private final BluetoothSocket mmSocket;
//        private final BluetoothDevice mmDevice;
//        public ConnectThread1(BluetoothDevice device) {
//
//            if(device == null)
//            {
//                Log.i("Status","Device is null");
//            }
//
//            mmDevice = device;
//            try {
//                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
//            } catch (IOException e) { }
//            mmSocket = tmp;
//        }
//        public void run() {
//            Log.i("Tag","Reached connect run");
//            try {
//                mmSocket.connect();
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//            mConnectedThread1 = new ConnectedThread1(mmSocket);
//            mConnectedThread1.start();
//        }
//        public void cancel() {
//            try {
//                mmSocket.close();
//            } catch (IOException e) { }
//        }
//    }
//
//    public class ConnectedThread1 extends Thread {
//        private final BluetoothSocket mmSocket;
//        private final InputStream mmInStream;
//        private final OutputStream mmOutStream;
//
//        public ConnectedThread1(BluetoothSocket socket) {
//            Log.i("Tag","Reached connected thread");
//            mmSocket = socket;
//            InputStream tmpIn = null;
//            OutputStream tmpOut = null;
//            try {
//                tmpIn = socket.getInputStream();
//                tmpOut = socket.getOutputStream();
//            } catch (IOException e) {
//            }
//            mmInStream = tmpIn;
//            mmOutStream = tmpOut;
//        }
//
//        public void run() {
//            //Log.i("Tag","Reached connected run");
//
//            byte[] buffer = new byte[1024];  // buffer store for the stream
//            int bytes; // bytes returned from read()
//
//            // Keep listening to the InputStream until an exception occurs
//
//            int len2 = message_2.length();
//            Log.i("len2 ", String.valueOf(len2));
//
//
//        }
//
//        public void write(byte[] bytes) {
//            Log.i("Tag","Reached connected write");
//            try {
//                mmOutStream.write(bytes);
//            } catch (IOException e) {
//            }
//        }
//
//        public void cancel() {
//            try {
//                mmSocket.close();
//            } catch (IOException e) {
//            }
//
//        }
//   }

}





