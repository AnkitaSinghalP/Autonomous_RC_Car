package com.example.mana1991.selfdrivingcar;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.support.v4.app.ActivityCompat;
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
import android.support.v4.app.FragmentActivity;
import com.google.android.gms.maps.model.*;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
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

public class MainActivity extends FragmentActivity implements OnMapReadyCallback{
    GoogleMap map1_main;
    Button connect;
    Button start;
    Button end;
    Button check_pt;
    int REQUEST_ENABLE_BT = 1;
    ListView listView1 ;
    LinearLayout listviewble;
    LinearLayout progressBar;
    LinearLayout status;
    ArrayAdapter<BluetoothDevice> arrayAdapter;

    ProgressBar pbheader;

    BluetoothAdapter bleAdapter;

    MapFragment mapFragment;

    ArrayList markers;
    String data = "";
    InputStream iStream;
    HttpURLConnection urlConnection = null;

    ConnectThread mConnectThread;
    ConnectedThread mConnectedThread;

    BluetoothSocket tmp ;

    String message_1 = "b";
    String message_2 = "a";
    String lng_delimiter = "l";
    String lat_delimiter = "c";
    String len_delimiter = "%";
    // String uart_mark = "b";
    String dest_limit = "$";

    public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    ArrayList<BluetoothDevice> pairedDeviceArrayList;
    BluetoothDevice device1;


    ArrayList points = null;
    PolylineOptions lineoptions;
    MarkerOptions markerOptions = null;
    MarkerOptions options = null;
    boolean mark = false;

    boolean enter_flag = false;

    byte[] buffer = null;

    List<LatLng> list = null;
    List<LatLng> list1 = null;
    List<LatLng>pack_checkpt = null;

    int readBufferPosition = 0;
    char delimiter = 0x23;
    char delimiter1 = 0x2D;
    char com_delimiter = 0x2C;
    char delimiter2 = 0x62;
    char delimiter3 = 0x69;
    char delimiter4 = 0x73;
    char delimiter5 = 0x6D;
    char delimiter6 = 0x6C;
    byte[] readBuffer;
    int bytesAvailable=0;
    TextView text_display;
    String data1;
    String data2;

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

    String lat;
    String lng;
    int i = 0;

    Double start_lat = 37.336575;//37.336838;
    Double start_lng = -121.882256;//-121.881508;
    LatLng start_pt;
    LatLng sanjose;

    LatLng polyline;
    Double lat_new;
    Double lng_new;
    Double polyline_lat;
    Double polyline_lng;
    Double polyline_end_lat;
    Double polyline_end_lng;
    final int chk_pt_precision = 6;
    byte[] chk_pt_byte_lat = null;
    byte[] chk_pt_byte_lng = null;
    byte[] chk_pt_byte_end_lat = null;
    byte[] chk_pt_byte_end_lng = null;

    boolean enter_map1 = false;
    boolean enter_map2 = false;



    Context context;

    Handler handler = new Handler();
    Runnable r = new Runnable() {
        public void run() {
            handler.postDelayed(this, 100);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.activity_main);

        setResult(Activity.RESULT_CANCELED);

        progressBar = (LinearLayout) findViewById(R.id.progressbar);
        listviewble = (LinearLayout) findViewById(R.id.listviewble);
        status = (LinearLayout) findViewById(R.id.status);

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
        check_pt = (Button) findViewById(R.id.button4);


        bleAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bleAdapter == null) {
            Toast.makeText(MainActivity.this, "Bluetooth not available", Toast.LENGTH_SHORT).show();
        }

        connect = (Button) findViewById(R.id.button1);
        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                connect();
            }
        });


        mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);
        mapFragment.getView().setVisibility(View.VISIBLE);
        mapFragment.getMapAsync(this);


    }



    @Override
    protected void onDestroy () {
        super.onDestroy();

        if (bleAdapter != null) {
            bleAdapter.cancelDiscovery();
        }

        try {
            mmInStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }


        try {
            mmOutStream.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            mmOutStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            mmSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    @Override
    public void onMapReady (final GoogleMap map1) {

        check_pt = (Button) findViewById(R.id.button4);
        check_pt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                map1_main = map1;

                markers = new ArrayList();
                sanjose = new LatLng(37.3352, -121.8811);


                start_pt = new LatLng(start_lat, start_lng);


                if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                    return;
                }

                map1_main.moveCamera(CameraUpdateFactory.newLatLngZoom(sanjose, 15));
                if (map1_main != null) {
                    map1_main.setMyLocationEnabled(true);

                    markers.add(start_pt);

                    options = new MarkerOptions();

                    options.position(start_pt);

                    options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN));

                    map1_main.setOnMapClickListener(new GoogleMap.OnMapClickListener(){
                        @Override
                        public void onMapClick(LatLng latLng) {

                            markers.add(latLng);

                            options = new MarkerOptions();

                            options.position(latLng);

                            if (markers.size() == 2) {
                                options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED));
                            }

                            map1_main.addMarker(options);

                            if (markers.size() >= 2) {
                                LatLng start = (LatLng) markers.get(0);
                                LatLng dest = (LatLng) markers.get(1);

                                Log.i("start Lat", String.valueOf(start.latitude));
                                Log.i("start Lng", String.valueOf(start.longitude));
                                String url =  getDirectionURL(start,dest);// "https://maps.googleapis.com/maps/api/directions/json?origin=37.336838,-121.881508&destination=37.33557357608086,-121.88304740935564&sensor=false&mode=walking";//getDirectionURL(start, dest);

                                DownloadTask downloadTask = new DownloadTask();

                                downloadTask.execute(url);
                            }

                        }
                    });

                }
            }
        });
    }


    private String getDirectionURL(LatLng start, LatLng dest){

        String str_start = "origin="+start.latitude+","+start.longitude;
        String str_dest = "destination="+dest.latitude+","+dest.longitude;
        String sensor = "sensor=false";
        String mode = "mode=walking";
        String key = "key=AIzaSyAMhbUYMVutFkNJL40g9950Zj5iAbjOAr8";
        String parameters = str_start+"&"+str_dest+"&"+sensor+"&"+mode;
        String output = "json";
        // String url = "https://maps.googleapis.com/maps/api/directions/json?origin=37.336838,-121.881508&destination=37.33557357608086,-121.88304740935564&sensor=false&mode=walking";
        String url = "https://maps.googleapis.com/maps/api/directions/"+output+"?"+parameters;
        Log.i("url",url);

        return url;

    }

    private class DownloadTask extends AsyncTask<String, String, String>{
        @Override
        protected String doInBackground(String... url){
//            String data = "{\n" +
//                    "   \"geocoded_waypoints\" : [\n" +
//                    "      {\n" +
//                    "         \"geocoder_status\" : \"OK\",\n" +
//                    "         \"place_id\" : \"ChIJa0V3N7_Mj4AR8A63gwlVLgY\",\n" +
//                    "         \"types\" : [ \"street_address\" ]\n" +
//                    "      },\n" +
//                    "      {\n" +
//                    "         \"geocoder_status\" : \"OK\",\n" +
//                    "         \"place_id\" : \"ChIJKbMkBbnMj4ARb4fpPCstffA\",\n" +
//                    "         \"types\" : [ \"route\" ]\n" +
//                    "      }\n" +
//                    "   ],\n" +
//                    "   \"routes\" : [\n" +
//                    "      {\n" +
//                    "         \"bounds\" : {\n" +
//                    "            \"northeast\" : {\n" +
//                    "               \"lat\" : 37.3379471,\n" +
//                    "               \"lng\" : -121.8810118\n" +
//                    "            },\n" +
//                    "            \"southwest\" : {\n" +
//                    "               \"lat\" : 37.3355818,\n" +
//                    "               \"lng\" : -121.8836403\n" +
//                    "            }\n" +
//                    "         },\n" +
//                    "         \"copyrights\" : \"Map data ©2016 Google\",\n" +
//                    "         \"legs\" : [\n" +
//                    "            {\n" +
//                    "               \"distance\" : {\n" +
//                    "                  \"text\" : \"0.3 mi\",\n" +
//                    "                  \"value\" : 492\n" +
//                    "               },\n" +
//                    "               \"duration\" : {\n" +
//                    "                  \"text\" : \"6 mins\",\n" +
//                    "                  \"value\" : 363\n" +
//                    "               },\n" +
//                    "               \"end_address\" : \"Paseo De San Antonio, San Jose, CA 95112, USA\",\n" +
//                    "               \"end_location\" : {\n" +
//                    "                  \"lat\" : 37.3355818,\n" +
//                    "                  \"lng\" : -121.8830454\n" +
//                    "               },\n" +
//                    "               \"start_address\" : \"398 E San Fernando St, San Jose, CA 95112, USA\",\n" +
//                    "               \"start_location\" : {\n" +
//                    "                  \"lat\" : 37.3371669,\n" +
//                    "                  \"lng\" : -121.8810118\n" +
//                    "               },\n" +
//                    "               \"steps\" : [\n" +
//                    "                  {\n" +
//                    "                     \"distance\" : {\n" +
//                    "                        \"text\" : \"331 ft\",\n" +
//                    "                        \"value\" : 101\n" +
//                    "                     },\n" +
//                    "                     \"duration\" : {\n" +
//                    "                        \"text\" : \"1 min\",\n" +
//                    "                        \"value\" : 72\n" +
//                    "                     },\n" +
//                    "                     \"end_location\" : {\n" +
//                    "                        \"lat\" : 37.3379471,\n" +
//                    "                        \"lng\" : -121.8815899\n" +
//                    "                     },\n" +
//                    "                     \"html_instructions\" : \"Head \\u003cb\\u003enorthwest\\u003c/b\\u003e toward \\u003cb\\u003eE San Fernando St\\u003c/b\\u003e\",\n" +
//                    "                     \"polyline\" : {\n" +
//                    "                        \"points\" : \"il{bFh{{fV{CrB\"\n" +
//                    "                     },\n" +
//                    "                     \"start_location\" : {\n" +
//                    "                        \"lat\" : 37.3371669,\n" +
//                    "                        \"lng\" : -121.8810118\n" +
//                    "                     },\n" +
//                    "                     \"travel_mode\" : \"WALKING\"\n" +
//                    "                  },\n" +
//                    "                  {\n" +
//                    "                     \"distance\" : {\n" +
//                    "                        \"text\" : \"0.1 mi\",\n" +
//                    "                        \"value\" : 211\n" +
//                    "                     },\n" +
//                    "                     \"duration\" : {\n" +
//                    "                        \"text\" : \"3 mins\",\n" +
//                    "                        \"value\" : 156\n" +
//                    "                     },\n" +
//                    "                     \"end_location\" : {\n" +
//                    "                        \"lat\" : 37.3369702,\n" +
//                    "                        \"lng\" : -121.8836403\n" +
//                    "                     },\n" +
//                    "                     \"html_instructions\" : \"Turn \\u003cb\\u003eleft\\u003c/b\\u003e onto \\u003cb\\u003eE San Fernando St\\u003c/b\\u003e\",\n" +
//                    "                     \"maneuver\" : \"turn-left\",\n" +
//                    "                     \"polyline\" : {\n" +
//                    "                        \"points\" : \"eq{bF|~{fVP`@d@nAXv@b@jA\\\\~@n@bB\"\n" +
//                    "                     },\n" +
//                    "                     \"start_location\" : {\n" +
//                    "                        \"lat\" : 37.3379471,\n" +
//                    "                        \"lng\" : -121.8815899\n" +
//                    "                     },\n" +
//                    "                     \"travel_mode\" : \"WALKING\"\n" +
//                    "                  },\n" +
//                    "                  {\n" +
//                    "                     \"distance\" : {\n" +
//                    "                        \"text\" : \"0.1 mi\",\n" +
//                    "                        \"value\" : 163\n" +
//                    "                     },\n" +
//                    "                     \"duration\" : {\n" +
//                    "                        \"text\" : \"2 mins\",\n" +
//                    "                        \"value\" : 123\n" +
//                    "                     },\n" +
//                    "                     \"end_location\" : {\n" +
//                    "                        \"lat\" : 37.3356599,\n" +
//                    "                        \"lng\" : -121.8828806\n" +
//                    "                     },\n" +
//                    "                     \"html_instructions\" : \"Turn \\u003cb\\u003eleft\\u003c/b\\u003e\",\n" +
//                    "                     \"maneuver\" : \"turn-left\",\n" +
//                    "                     \"polyline\" : {\n" +
//                    "                        \"points\" : \"ak{bFvk|fVp@e@@A@?@A@?@?@?F@B?@?@AJETOZSPKDABABAZSPMDCFADCFANC\"\n" +
//                    "                     },\n" +
//                    "                     \"start_location\" : {\n" +
//                    "                        \"lat\" : 37.3369702,\n" +
//                    "                        \"lng\" : -121.8836403\n" +
//                    "                     },\n" +
//                    "                     \"travel_mode\" : \"WALKING\"\n" +
//                    "                  },\n" +
//                    "                  {\n" +
//                    "                     \"distance\" : {\n" +
//                    "                        \"text\" : \"56 ft\",\n" +
//                    "                        \"value\" : 17\n" +
//                    "                     },\n" +
//                    "                     \"duration\" : {\n" +
//                    "                        \"text\" : \"1 min\",\n" +
//                    "                        \"value\" : 12\n" +
//                    "                     },\n" +
//                    "                     \"end_location\" : {\n" +
//                    "                        \"lat\" : 37.3355818,\n" +
//                    "                        \"lng\" : -121.8830454\n" +
//                    "                     },\n" +
//                    "                     \"html_instructions\" : \"Turn \\u003cb\\u003eright\\u003c/b\\u003e\",\n" +
//                    "                     \"maneuver\" : \"turn-right\",\n" +
//                    "                     \"polyline\" : {\n" +
//                    "                        \"points\" : \"{b{bF~f|fVN`@\"\n" +
//                    "                     },\n" +
//                    "                     \"start_location\" : {\n" +
//                    "                        \"lat\" : 37.3356599,\n" +
//                    "                        \"lng\" : -121.8828806\n" +
//                    "                     },\n" +
//                    "                     \"travel_mode\" : \"WALKING\"\n" +
//                    "                  }\n" +
//                    "               ],\n" +
//                    "               \"traffic_speed_entry\" : [],\n" +
//                    "               \"via_waypoint\" : []\n" +
//                    "            }\n" +
//                    "         ],\n" +
//                    "         \"overview_polyline\" : {\n" +
//                    "            \"points\" : \"il{bFh{{fV{CrBP`@~@fC`AjCn@bBp@e@BABAJ@FAnAu@HC^Ud@WVEN`@\"\n" +
//                    "         },\n" +
//                    "         \"summary\" : \"E San Fernando St\",\n" +
//                    "         \"warnings\" : [\n" +
//                    "            \"Walking directions are in beta.    Use caution – This route may be missing sidewalks or pedestrian paths.\"\n" +
//                    "         ],\n" +
//                    "         \"waypoint_order\" : []\n" +
//                    "      }\n" +
//                    "   ],\n" +
//                    "   \"status\" : \"OK\"\n" +
//                    "}";
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

            try {
                routes = directionParser.parse(jsonObject);
            } catch (IOException e) {
                e.printStackTrace();
            }

            //  }
            return routes;
        }

        @Override
        protected void onPostExecute(List<List<HashMap<String, String>>> result) {

            markerOptions = new MarkerOptions();

            int i, j, index = 1;
            for (i = 0; i < result.size(); i++) {
                points = new ArrayList<LatLng>();
                pack_checkpt = new ArrayList<LatLng>();

                List<HashMap<String, String>> path = result.get(i);
                for (j = 0; j < path.size(); j++) {

                    HashMap<String, String> point = path.get(j);

                    double lat = Double.parseDouble(point.get("lat"));
                    Log.i("Lat", String.valueOf(lat));
                    double lng = Double.parseDouble(point.get("lng"));
                    Log.i("Lng", String.valueOf(lng));
                    LatLng position = new LatLng(lat, lng);

                    while (index < path.size()) {
                        HashMap<String, String> point1 = path.get(index);

                        DecimalFormat df = new DecimalFormat("#.######");
                        df.setRoundingMode(RoundingMode.CEILING);

                        double lat1 = Double.parseDouble(point1.get("lat"));
                        Log.i("Lat", String.valueOf(lat1));
                        double lng1 = Double.parseDouble(point1.get("lng"));
                        Log.i("Lng", String.valueOf(lng1));
                        LatLng position1 = new LatLng(lat1, lng1);
                        pack_checkpt.add(new LatLng(lat1, lng1));

                        Log.i("Path size", String.valueOf(path.size()));
                        for (int r = 0; r < pack_checkpt.size(); r++) {
                            Log.i("Check pt list", String.valueOf(pack_checkpt.get(r)));
                        }

                        map1_main.addMarker(new MarkerOptions().position(position1).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE)));
                        index = index + 1;
                    }

                    points.add(position);
                }

            }
        }

    }

    // Drawing polyline in the Google Map for the i-th route
    public class DirectionParser {

        /** Receives a JSONObject and returns a list of lists containing latitude and longitude */
        public List<List<HashMap<String,String>>> parse(JSONObject jObject) throws IOException {
            chk_pt_byte_lat=new byte[9];
            chk_pt_byte_lng=new byte[13];
            OutputStream temp = mmSocket.getOutputStream();
            List<List<HashMap<String, String>>> routes = new ArrayList<List<HashMap<String,String>>>() ;
            JSONArray jRoutes = null;
            JSONArray jLegs = null;
            JSONArray jSteps = null;

            try {

                jRoutes = jObject.getJSONArray("routes");

                /** Traversing all routes */
                jLegs = ( (JSONObject)jRoutes.get(0)).getJSONArray("legs");
                List path = new ArrayList<HashMap<String, String>>();

                /** Traversing all legs */


                jSteps = ( (JSONObject)jLegs.get(0)).getJSONArray("steps");

                int jlen = jSteps.length();
                int jStepslen = jSteps.length() - 1;
                String len = String.valueOf(jlen);

                /** Traversing all steps */
                temp.write(len_delimiter.getBytes());

                temp.write(len.getBytes());
               // int jStepslen = jSteps.length() - 1;
              //  mmSocket.getOutputStream().flush();

                for (int k = 1; k < jSteps.length(); k++) {
//                                for(int l=0; l<10; l++){
//                                    mmSocket.getOutputStream().write(uart_mark.getBytes());
//                                    Log.i("mark",uart_mark);
//                                }

                    //  handler.postDelayed(r, 5);
                    String poly_step = "";
                    polyline_lat = (Double) ((JSONObject) ((JSONObject) jSteps.get(k)).get("start_location")).get("lat");
                    polyline_lng = (Double) ((JSONObject) ((JSONObject) jSteps.get(k)).get("start_location")).get("lng");

                    chk_pt_byte_lat = convert_to_byte_chk_pt(polyline_lat, 8, chk_pt_precision);
                    chk_pt_byte_lng = convert_to_byte_chk_pt(polyline_lng, 11, chk_pt_precision);//change to 11

                    // handler.postDelayed(r, 10);
                    temp.write(lat_delimiter.getBytes());
                   // mmSocket.getOutputStream().flush();
                    for (int num = 0; num < 8; num++) {
                        temp.write(chk_pt_byte_lat[num]);
                       // mmSocket.getOutputStream().flush();

                        Log.i("check_pt_lat", String.valueOf(chk_pt_byte_lat[num]));
                    }
                    //mmSocket.getOutputStream().flush();
                    // mmSocket.getOutputStream().write(message_2.getBytes());

                    // handler.postDelayed(r, 10);

                    temp.write(lng_delimiter.getBytes());

                    for (int num = 0; num < 11; num++) {
                        temp.write(chk_pt_byte_lng[num]);
                       // mmSocket.getOutputStream().flush();

                        Log.i("check_pt_lng", String.valueOf(chk_pt_byte_lng[num]));
                    }


                    // mmSocket.getOutputStream().write(message_2.getBytes());
                    // handler.postDelayed(r, 1000);
                }
                polyline_end_lat = (Double) ((JSONObject) ((JSONObject) jSteps.get(jStepslen)).get("end_location")).get("lat");
                polyline_end_lng = (Double) ((JSONObject) ((JSONObject) jSteps.get(jStepslen)).get("end_location")).get("lng");
                chk_pt_byte_end_lat = convert_to_byte_chk_pt(polyline_end_lat, 8, chk_pt_precision);
                chk_pt_byte_end_lng = convert_to_byte_chk_pt(polyline_end_lng, 11, chk_pt_precision);

                temp.write(lat_delimiter.getBytes());
                for (int num = 0; num < 8; num++) {
                    temp.write(chk_pt_byte_end_lat[num]);
                  //  mmSocket.getOutputStream().flush();

                    Log.i("check_pt_lat", String.valueOf(chk_pt_byte_end_lat[num]));
                }
                // mmSocket.getOutputStream().write(message_2.getBytes());

                // handler.postDelayed(r, 10);

                temp.write(lng_delimiter.getBytes());
              //  mmSocket.getOutputStream().flush();

                for (int num = 0; num < 11; num++) {
                    temp.write(chk_pt_byte_end_lng[num]);
                 //   mmSocket.getOutputStream().flush();

                    Log.i("check_pt_lng", String.valueOf(chk_pt_byte_end_lng[num]));
                }

                temp.write(dest_limit.getBytes());
                chk_pt_byte_lng = null;
                chk_pt_byte_end_lat = null;
                chk_pt_byte_lat = null;
                chk_pt_byte_end_lng = null;
               // Log.i("check_pt_lng", String.valueOf(chk_pt_byte_end_lng[0]));
               // Log.i("check_pt_lat", String.valueOf(chk_pt_byte_end_lat[0]));
               // mmSocket.getOutputStream().flush();
                routes.add(path);
                temp.flush();

            } catch (JSONException e) {
                e.printStackTrace();
            }catch (Exception e){
            }
            return routes;
        }

        public byte[] convert_to_byte_chk_pt(double input, int size, int precision) {
            double tempInput = input;
            tempInput *= Math.pow(10,precision);
            int output = (int) tempInput;
            String strOut = String.format("%0"+size+"d", output);
            return strOut.getBytes();
        }

        private List<LatLng> decodePoly(String encoded) {

            Log.i("encode",encoded);
            List<LatLng> poly = new ArrayList<LatLng>();
            int index = 0, len = encoded.length();
            int lat = 0, lng = 0;

            while (index < len) {
                int b, shift = 0, result = 0;
                do {
                    b = encoded.charAt(index++) - 63;
                    Log.i("b_lat", String.valueOf(b));
                    result |= (b & 0x1f) << shift;
                    Log.i("result_lat", String.valueOf(result));
                    shift += 5;
                } while (b >= 0x20);
                int dlat = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
                Log.i("dlat", String.valueOf(dlat));
                lat += dlat;
                Log.i("decode_lat", String.valueOf(lat));

                shift = 0;
                result = 0;
                do {
                    b = encoded.charAt(index++) - 63;
                    Log.i("b_lng", String.valueOf(b));
                    result |= (b & 0x1f) << shift;
                    Log.i("result_lng", String.valueOf(result));
                    shift += 5;
                } while (b >= 0x20);
                int dlng = ((result & 1) != 0 ? ~(result >> 1) : (result >> 1));
                Log.i("dlng", String.valueOf(dlng));
                lng += dlng;
                Log.i("decode_lng", String.valueOf(lng));

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

            }
        });
    }

    public class ConnectThread extends Thread {
       // private final BluetoothSocket mmSocket;
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


            buffer = new byte[1024];  // buffer store for the stream
            String a;
            String received = null;

            // Keep listening to the InputStream until an exception occurs
            while(true) {

                start.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        enter_flag = false;
                        mapFragment.getView().setVisibility(View.VISIBLE);
                        int count1 = 0;
                        while (count1 <= 5) {
                            byte[] send = message_1.getBytes();
                            mConnectedThread.write(send);
                            count1++;
                        }
                       // handler.postDelayed(r, 1000);
                    }
                });


                end.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        enter_flag = false;
                        int count = 0;
                        while (count <= 5) {
                            byte[] send = message_2.getBytes();
                            mConnectedThread.write(send);
                            count++;
                        }
                    }
                });


                try {
                    readBuffer = new byte[1024];
                    bytesAvailable = mmInStream.available();
                    if(bytesAvailable > 0 && enter_flag == true) {
                        byte[] packetBytes = new byte[bytesAvailable];
                        mmInStream.read(packetBytes);
                        byte b = packetBytes[0];
                        if (b == delimiter1 && b != 0x00 && enter_flag == true) {
                            for (i = 1; i < 32 && enter_flag == true; i++) {
                                b = packetBytes[i];
                                if (b == com_delimiter && b != 0x00) {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    data2 = new String(encodedBytes, "US-ASCII");
                                    start_lat = Double.parseDouble(data2);
                                    enter_map1 = true;
                                    readBufferPosition = 0;
                                    enter_map1 = true;
                                    Log.i("Buffer1 data", String.valueOf(start_lat));
                                } else if (b == delimiter && b != 0x00) {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    data1 = new String(encodedBytes, "US-ASCII");
                                    start_lng = Double.parseDouble(data1);
                                    enter_map2 = true;
                                    readBufferPosition = 0;

                                    Log.i("Buffer2 data", String.valueOf(start_lng));

                                } else if (b >= 0x30 || b <= 0x39 || b == 0x2E || b != delimiter1 || b != delimiter) {

                                    readBuffer[readBufferPosition++] = b;
                                }

                            }

                            enter_flag = false;
                        }

                    }



                } catch (IOException e) {
                    e.printStackTrace();
                }


            }

        }

        public void write(byte[] bytes) {
            Log.i("Tag","Reached connected write");
            try {
                mmOutStream.write(bytes);
                mmOutStream.flush();
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
}
