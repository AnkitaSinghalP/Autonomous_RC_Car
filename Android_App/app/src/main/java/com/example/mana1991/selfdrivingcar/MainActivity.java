package com.example.mana1991.selfdrivingcar;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.IntentSender;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.location.Location;
import android.location.LocationManager;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;
import android.support.v4.app.FragmentActivity;

import com.cardiomood.android.controls.gauge.BatteryIndicatorGauge;
import com.cardiomood.android.controls.gauge.SpeedometerGauge;
import com.github.clans.fab.FloatingActionButton;
import com.github.clans.fab.FloatingActionMenu;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationServices;
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
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import android.os.Handler;

import static com.example.mana1991.selfdrivingcar.R.id.sys_ble_statusimage;
import static com.example.mana1991.selfdrivingcar.R.id.systemstatus;

public class MainActivity extends FragmentActivity implements OnMapReadyCallback, GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener,
        LocationListener {
    GoogleMap map1_main;
    FloatingActionMenu materialDesignFAM;
    FloatingActionButton start, stop, connect, checkpoint, display;
    int REQUEST_ENABLE_BT = 1;
    ListView listViewPairedDevice;

    LinearLayout bluetoothcontrol, maplayout, sensorstatus;
    LinearLayout progressBar;
    LinearLayout sys_status;
    ArrayAdapter<BluetoothDevice> arrayAdapter;
    private SpeedometerGauge speedometer;
    private BatteryIndicatorGauge batteryindicator;
    ProgressBar pbheader;

    BluetoothAdapter bleAdapter;

    MapFragment mapFragment;

    ArrayList markers = null;
    ArrayList markers1;
    String data = "";
    InputStream iStream;
    HttpURLConnection urlConnection = null;

    ConnectThread mConnectThread;
    ConnectedThread mConnectedThread;

    BluetoothSocket tmp;

    String message_1 = "b";
    String message_2 = "a";
    String lng_delimiter = "l";
    String lat_delimiter = "c";
    // String uart_mark = "b";
    String dest_limit = "$";
    String byte_string[] = new String[17];
    public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    ArrayList<BluetoothDevice> pairedDeviceArrayList;
    BluetoothDevice device1;


    ArrayList points = null;
    PolylineOptions lineoptions;
    MarkerOptions markerOptions = null;
    MarkerOptions options = null;
    MarkerOptions options1 = null;
    boolean mark = false;

    boolean enter_flag = false;

    byte[] buffer = null;

    List<LatLng> list = null;
    List<LatLng> list1 = null;
    List<LatLng> pack_checkpt = null;

    int readBufferPosition = 0;
    char delimiter_dollar = 0x24;
    char delimiter1_at = 0x40;
    char com_delimiter = 0x2C;
    char delimiter2 = 0x62;
    char delimiter3 = 0x69;
    char delimiter4 = 0x73;
    char delimiter5 = 0x6D;
    char delimiter6 = 0x6C;
    //byte[] readBuffer = null;
    ArrayList<Byte>readBuffer = null;
    int bytesAvailable = 0;
    TextView text_display;
    String data1;
    String data2;

    Intent bleIntent;
    Set<BluetoothDevice> pairedDevices;
    BluetoothSocket mmSocket;
    InputStream mmInStream;
    OutputStream mmOutStream;

    TextView lstatus,rstatus,mstatus,rearstatus,criticalstatus;
    ImageView lstatus_image,rstatus_image,cstatus_image,rearstatus_image,mstatus_image;
    LinearLayout system_display;
TableRow trow;
    LinearLayout leftsensor,rightsensor,middlesensor,rearsensor,critical;
LinearLayout llble_status,llio_status,llgeo_status,llmaster_status,llmotor_status,llsensor_status;
    LinearLayout misc,speedll,syscmdll,destrchdll,batterystatusll;
    TextView blevalue;
    TextView iovalue;
    TextView mastervalue;
    TextView motorvalue;
    TextView sensorrvalue;
    TextView geovalue;

    TextView speed_label,syscmd_label,dstrchd,batrylabel,speed_val,btryval;
    ImageView bleimage,geoimage,sensorimage,masterimage,motorimage,ioimage,sysval_val;

    String ble_status;
    String io_status;
    String mas_status;
    String mot_status;
    String sen_status;

    String lat;
    String lng;
    int i = 0;

    Double start_lat = 0.0;//37.336575;//37.336838;
    Double start_lng = 0.0;//-121.882256;//-121.881508;
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
    byte[] chk_pt_byte_lat;
    byte[] chk_pt_byte_lng;
    byte[] chk_pt_byte_end_lat;
    byte[] chk_pt_byte_end_lng;

    boolean enter_map1 = false;
    boolean enter_map2 = false;

    LocationRequest mLocationRequest;
    GoogleApiClient mGoogleApiClient;
    Location location;
    Marker start_marker;
    LocationServices locationServices;
    LocationManager locationManager;
    Context mContext;
    boolean isGPSEnabled = false;
    boolean isNetworkEnabled = false;
    boolean canGetLocation = false;

    private static final long MIN_DISTANCE_CHANGE_FOR_UPDATES = 10;
    private static final long MIN_TIME_BW_UPDATES = 1000 * 60 * 1;

    double currentLatitude;
    double currentLongitude;
    private final static int CONNECTION_FAILURE_RESOLUTION_REQUEST = 9000;
    JSONArray jRoutes = null;
    JSONArray jLegs = null;
    JSONArray jSteps = null;
    List<LatLng> check_path_pt = null;
    LatLng check_pt_new;
    List<HashMap<String, String>> path;

    String len_delimiter = "%";
    boolean flag = false;


    Context context;
    int j = 0;


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

        setupUI();

        // this.mContext = getApplicationContext();

        bleAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bleAdapter == null) {
            Toast.makeText(MainActivity.this, "Bluetooth not available", Toast.LENGTH_SHORT).show();
        }


        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sensorstatus.setVisibility(View.INVISIBLE);
                sys_status.setVisibility(View.INVISIBLE);
                misc.setVisibility(View.INVISIBLE);
                maplayout.setVisibility(View.INVISIBLE);

                ConnectTask connectTask = new ConnectTask();
                connectTask.execute();

            }
        });
        display.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {


                maplayout.setVisibility(View.INVISIBLE);
                DisplayTask displayTask = new DisplayTask();
                displayTask.execute();

            }
        });

        buildGoogleApiClient();
        mLocationRequest = LocationRequest.create()
                .setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY)
                .setInterval(10 * 1000)        // 10 seconds, in milliseconds
                .setFastestInterval(1 * 1000);
        // getLocation();


    }

    private void setupUI() {
        bluetoothcontrol = (LinearLayout) findViewById(R.id.bluetoothcontrol);
        maplayout = (LinearLayout) findViewById(R.id.maplayout);
        //dashboard_layout = (LinearLayout) findViewById(R.id.dashboardlayout);
        listViewPairedDevice = (ListView) findViewById(R.id.pairedlist);

        materialDesignFAM = (FloatingActionMenu) findViewById(R.id.material_design_android_floating_action_menu);
        start = (FloatingActionButton) findViewById(R.id.start);
        stop = (FloatingActionButton) findViewById(R.id.stop);
        connect = (FloatingActionButton) findViewById(R.id.connect);
        display = (FloatingActionButton) findViewById(R.id.display);
        checkpoint = (FloatingActionButton) findViewById(R.id.checkpoint);

        mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);

        sensorstatus= (LinearLayout) findViewById(R.id.sensorstatus);
        leftsensor = (LinearLayout) findViewById(R.id.leftsensor);
        rightsensor = (LinearLayout) findViewById(R.id.rightsensor);
        rearsensor = (LinearLayout) findViewById(R.id.rearsensor);
        critical = (LinearLayout) findViewById(R.id.critical);
        middlesensor = (LinearLayout) findViewById(R.id.middlesensor);

        lstatus= (TextView)findViewById(R.id.lstatus) ;
        rstatus= (TextView)findViewById(R.id.rstatus) ;
        mstatus= (TextView)findViewById(R.id.mstatus) ;
        criticalstatus= (TextView)findViewById(R.id.criticalstatus) ;
        rearstatus= (TextView)findViewById(R.id.rearstatus) ;

        lstatus_image = (ImageView) findViewById(R.id.lstatus_image) ;
        rstatus_image = (ImageView) findViewById(R.id.rstatus_image) ;
        mstatus_image = (ImageView) findViewById(R.id.mstatus_image) ;
        cstatus_image = (ImageView) findViewById(R.id.criticalstatus_image) ;
        rearstatus_image = (ImageView) findViewById(R.id.rearstatus_image) ;

        sys_status=(LinearLayout) findViewById(systemstatus);
        llble_status=(LinearLayout) findViewById(R.id.ble_status);
        llgeo_status=(LinearLayout) findViewById(R.id.geo_status);
        llio_status=(LinearLayout) findViewById(R.id.io_status);
        llmaster_status=(LinearLayout) findViewById(R.id.master_status);
        llmotor_status=(LinearLayout) findViewById(R.id.motor_status);
        llsensor_status=(LinearLayout) findViewById(R.id.sensor_status);

        blevalue=(TextView)findViewById(R.id.sys_ble_status) ;
        iovalue=(TextView)findViewById(R.id.sys_io_status) ;
        motorvalue=(TextView)findViewById(R.id.sys_motor_status) ;
        mastervalue=(TextView)findViewById(R.id.sys_master_status) ;
        sensorrvalue=(TextView)findViewById(R.id.sys_sensor_status) ;
        geovalue=(TextView)findViewById(R.id.sys_geo_status) ;

        bleimage = (ImageView) findViewById(R.id.sys_ble_statusimage) ;
        geoimage=(ImageView) findViewById(R.id.sys_geo_statusimage) ;
        ioimage=(ImageView) findViewById(R.id.sys_io_statusimage) ;
        masterimage=(ImageView) findViewById(R.id.sys_io_statusimage) ;
        motorimage=(ImageView) findViewById(R.id.sys_motor_statusimage) ;
        sensorimage=(ImageView) findViewById(R.id.sys_sensor_statusimage) ;

        misc=(LinearLayout) findViewById(R.id.miscll);
        speedll=(LinearLayout) findViewById(R.id.speedll);
        syscmdll=(LinearLayout) findViewById(R.id.syscmdll);
        destrchdll=(LinearLayout) findViewById(R.id.destrchdll);


        dstrchd=(TextView)findViewById(R.id.dstrchd) ;
        syscmd_label=(TextView)findViewById(R.id.syscmd_label) ;
        speed_label=(TextView)findViewById(R.id.speed_label) ;


        sysval_val=(ImageView)findViewById(R.id.sysval_val) ;
        speed_val=(TextView)findViewById(R.id.speed_val) ;

    }

    private void buildGoogleApiClient() {
        mGoogleApiClient = new GoogleApiClient.Builder(this)
                // The next two lines tell the new client that “this” current class will handle connection stuff
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                //fourth line adds the LocationServices API endpoint from GooglePlayServices
                .addApi(LocationServices.API)
                .build();
    }

    @Override
    protected void onStart() {
        super.onStart();
        if (!bleAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);

        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        mapFragment.getMapAsync(this);
        //Now lets connect to the API
        mGoogleApiClient.connect();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        try {
            mmInStream.close();
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

        if (bleAdapter != null) {
            bleAdapter.cancelDiscovery();
        }

    }


    @Override
    public void onMapReady(final GoogleMap map1) {
        checkpoint.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                maplayout.setVisibility(View.VISIBLE);

                sensorstatus.setVisibility(View.INVISIBLE);
                sys_status.setVisibility(View.INVISIBLE);
                misc.setVisibility(View.INVISIBLE);

                map1_main = map1;

                markers = new ArrayList();
                sanjose = new LatLng(37.3352, -121.8811);

                start_pt = new LatLng(currentLatitude, currentLongitude);
                Log.i("lat_start", String.valueOf(start_pt.latitude));
                Log.i("lat_start", String.valueOf(start_pt.longitude));
                // if (start_pt.latitude != 0 && start_pt.longitude != 0) {
                map1_main.addMarker(new MarkerOptions().position(new LatLng(start_pt.latitude, start_pt.longitude)).title("Current location").icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN)));
                // }


                if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                    return;
                }

                map1_main.moveCamera(CameraUpdateFactory.newLatLngZoom(sanjose, 15));
                if (map1_main != null) {
                    map1_main.setMyLocationEnabled(true);
                    map1_main.setOnMapClickListener(new GoogleMap.OnMapClickListener() {
                        @Override
                        public void onMapClick(LatLng latLng) {

                            map1_main.addMarker(new MarkerOptions().position(new LatLng(latLng.latitude, latLng.longitude)).title("Destination location").icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)));

                            String url = getDirectionURL(start_pt, latLng);// "https://maps.googleapis.com/maps/api/directions/json?origin=37.336838,-121.881508&destination=37.33557357608086,-121.88304740935564&sensor=false&mode=walking";//getDirectionURL(start, dest);

                            DownloadTask downloadTask = new DownloadTask();

                            downloadTask.execute(url);
                            //  }

                        }

                    });

                }
            }
        });
    }


    private String getDirectionURL(LatLng start, LatLng dest) {

        String str_start = "origin=" + start.latitude + "," + start.longitude;
        String str_dest = "destination=" + dest.latitude + "," + dest.longitude;
        String sensor = "sensor=false";
        String mode = "mode=walking";
        String key = "key=AIzaSyAMhbUYMVutFkNJL40g9950Zj5iAbjOAr8";
        String parameters = str_start + "&" + str_dest + "&" + sensor + "&" + mode;
        String output = "json";
        // String url = "https://maps.googleapis.com/maps/api/directions/json?origin=37.336838,-121.881508&destination=37.33557357608086,-121.88304740935564&sensor=false&mode=walking";
        String url = "https://maps.googleapis.com/maps/api/directions/" + output + "?" + parameters;
        Log.i("url", url);

        return url;

    }

    @Override
    public void onConnected(@Nullable Bundle bundle) {
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
        Location location = LocationServices.FusedLocationApi.getLastLocation(mGoogleApiClient);

        if (location == null) {
            LocationServices.FusedLocationApi.requestLocationUpdates(mGoogleApiClient, mLocationRequest, this);

        } else {
            //If everything went fine lets get latitude and longitude
            currentLatitude = location.getLatitude();
            currentLongitude = location.getLongitude();

            Toast.makeText(this, currentLatitude + " WORKS " + currentLongitude + "", Toast.LENGTH_LONG).show();
        }

    }

    @Override
    public void onConnectionSuspended(int i) {

    }

    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {
        if (connectionResult.hasResolution()) {
            try {
                // Start an Activity that tries to resolve the error
                connectionResult.startResolutionForResult(this, CONNECTION_FAILURE_RESOLUTION_REQUEST);
                    /*
                     * Thrown if Google Play services canceled the original
                     * PendingIntent
                     */
            } catch (IntentSender.SendIntentException e) {
                // Log the error
                e.printStackTrace();
            }
        } else {
                /*
                 * If no resolution is available, display a dialog to the
                 * user with the error.
                 */
            Log.e("Error", "Location services connection failed with code " + connectionResult.getErrorCode());
        }

    }

    @Override
    public void onLocationChanged(Location location) {
        currentLatitude = location.getLatitude();
        currentLongitude = location.getLongitude();

        Toast.makeText(this, currentLatitude + " WORKS " + currentLongitude + "", Toast.LENGTH_LONG).show();


    }

    private String downloadUrl(String str_url) throws IOException {

        try {
            URL url = new URL(str_url);
            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.connect();

            iStream = urlConnection.getInputStream();

            BufferedReader br = new BufferedReader(new InputStreamReader(iStream));

            StringBuffer sb = new StringBuffer();
            String line = "";
            while ((line = br.readLine()) != null) {
                sb.append(line);
            }

            data = sb.toString();

            br.close();
        } catch (Exception e) {
            Log.d("Exception while downloading url", e.toString());
        } finally {
        }
        return data;
    }

    private class DownloadTask extends AsyncTask<String, String, String> {
        @Override
        protected String doInBackground(String... url) {

            try {
                data = downloadUrl(url[0]);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return data;
        }

        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);

            ParserTask parserTask = new ParserTask();
            parserTask.execute(result);
        }
    }


    private class ParserTask extends AsyncTask<String, Integer, List<List<HashMap<String, String>>>> {
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
                lineoptions = new PolylineOptions();

                path = result.get(i);
                for (j = 0; j < path.size(); j++) {

                    HashMap<String, String> point = path.get(j);

                    double lat = Double.parseDouble(point.get("lat"));
                    //  Log.i("Lat", String.valueOf(lat));
                    double lng = Double.parseDouble(point.get("lng"));
                    // Log.i("Lng", String.valueOf(lng));
                    LatLng position = new LatLng(lat, lng);

                    for (int l = 1; l < jSteps.length(); l++) {

                        try {
                            polyline_lat = (Double) ((JSONObject) ((JSONObject) jSteps.get(l)).get("start_location")).get("lat");
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                        try {
                            polyline_lng = (Double) ((JSONObject) ((JSONObject) jSteps.get(l)).get("start_location")).get("lng");
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                        check_pt_new = new LatLng(polyline_lat, polyline_lng);
                        map1_main.addMarker(new MarkerOptions().position(check_pt_new).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_CYAN)));
                    }

                    while (index < path.size()) {
                        HashMap<String, String> point1 = path.get(index);

                        // DecimalFormat df = new DecimalFormat("#.######");
                        //  df.setRoundingMode(RoundingMode.CEILING);

                        double lat1 = Double.parseDouble(point1.get("lat"));
                        //   Log.i("Lat", String.valueOf(lat1));
                        double lng1 = Double.parseDouble(point1.get("lng"));
                        // Log.i("Lng", String.valueOf(lng1));
                        LatLng position1 = new LatLng(lat1, lng1);
                        pack_checkpt.add(new LatLng(lat1, lng1));

                        // Log.i("Path size", String.valueOf(path.size()));
                        for (int r = 0; r < pack_checkpt.size(); r++) {
                            Log.i("Check pt list", String.valueOf(pack_checkpt.get(r)));
                        }
                        // map1_main.addMarker(new MarkerOptions().position(position1).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE)));
                        index = index + 5;
                    }

                    points.add(position);
                }
                lineoptions.addAll(points);
                lineoptions.width(10);
                lineoptions.color(Color.MAGENTA);
            }
            if (lineoptions != null) {
                map1_main.addPolyline(lineoptions);
            }
        }

    }

    // Drawing polyline in the Google Map for the i-th route
    public class DirectionParser {

        /**
         * Receives a JSONObject and returns a list of lists containing latitude and longitude
         */
        public List<List<HashMap<String, String>>> parse(JSONObject jObject) {
            markerOptions = new MarkerOptions();

            List<List<HashMap<String, String>>> routes = new ArrayList<List<HashMap<String, String>>>();
            // List path1;

            try {

                jRoutes = jObject.getJSONArray("routes");

                /** Traversing all routes */
                jLegs = ((JSONObject) jRoutes.get(0)).getJSONArray("legs");
                List path1 = new ArrayList<HashMap<String, String>>();

                /** Traversing all legs */
                jSteps = ((JSONObject) jLegs.get(0)).getJSONArray("steps");

                for (int k = 0; k < jSteps.length(); k++) {
                    String polyline = "";
                    polyline = (String) ((JSONObject) ((JSONObject) jSteps.get(k)).get("polyline")).get("points");
                    List<LatLng> list = decodePoly(polyline);

                    /** Traversing all points */
                    for (int l = 0; l < list.size(); l++) {
                        HashMap<String, String> hm = new HashMap<>();
                        hm.put("lat", Double.toString((list.get(l)).latitude));
                        hm.put("lng", Double.toString((list.get(l)).longitude));
                        path1.add(hm);
                    }


                }
                //routes.add(path);

                /** Traversing all steps */
                int jlen = jSteps.length();
                int jStepslen = jSteps.length() - 1;
                String len = String.valueOf(jlen);
                /** Traversing all steps */
                mmSocket.getOutputStream().write(len_delimiter.getBytes());
                mmSocket.getOutputStream().write(len.getBytes());
                //int jStepslen = jSteps.length() - 1;

                for (int k = 1; k < jSteps.length(); k++) {
//
                    //String poly_step = "";
                    polyline_lat = (Double) ((JSONObject) ((JSONObject) jSteps.get(k)).get("start_location")).get("lat");
                    polyline_lng = (Double) ((JSONObject) ((JSONObject) jSteps.get(k)).get("start_location")).get("lng");
                    // check_pt_new = new LatLng(polyline_lat,polyline_lng);
                    // map1_main.addMarker(new MarkerOptions().position(check_pt_new).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_CYAN)));
//                }
                    // check_path_pt.add(check_pt_new);

                    chk_pt_byte_lat = convert_to_byte_chk_pt(polyline_lat, 8, chk_pt_precision);
                    chk_pt_byte_lng = convert_to_byte_chk_pt(polyline_lng, 11, chk_pt_precision);//change to 11

                    // handler.postDelayed(r, 10);
                    mmSocket.getOutputStream().write(lat_delimiter.getBytes());
                    for (int num = 0; num < 8; num++) {
                        mmSocket.getOutputStream().write(chk_pt_byte_lat[num]);

                        Log.i("check_pt_lat", String.valueOf(chk_pt_byte_lat[num]));
                    }
                    // mmSocket.getOutputStream().write(message_2.getBytes());

                    // handler.postDelayed(r, 10);

                    mmSocket.getOutputStream().write(lng_delimiter.getBytes());

                    for (int num = 0; num < 11; num++) {
                        mmSocket.getOutputStream().write(chk_pt_byte_lng[num]);

                        Log.i("check_pt_lng", String.valueOf(chk_pt_byte_lng[num]));
                    }


                    // mmSocket.getOutputStream().write(message_2.getBytes());
                    // handler.postDelayed(r, 1000);
                }
                polyline_end_lat = (Double) ((JSONObject) ((JSONObject) jSteps.get(jStepslen)).get("end_location")).get("lat");
                polyline_end_lng = (Double) ((JSONObject) ((JSONObject) jSteps.get(jStepslen)).get("end_location")).get("lng");
                chk_pt_byte_end_lat = convert_to_byte_chk_pt(polyline_end_lat, 8, chk_pt_precision);
                chk_pt_byte_end_lng = convert_to_byte_chk_pt(polyline_end_lng, 11, chk_pt_precision);

                mmSocket.getOutputStream().write(lat_delimiter.getBytes());
                for (int num = 0; num < 8; num++) {
                    mmSocket.getOutputStream().write(chk_pt_byte_end_lat[num]);

                    Log.i("check_pt_lat", String.valueOf(chk_pt_byte_end_lat[num]));
                }
                // mmSocket.getOutputStream().write(message_2.getBytes());

                // handler.postDelayed(r, 10);

                mmSocket.getOutputStream().write(lng_delimiter.getBytes());

                for (int num = 0; num < 11; num++) {
                    mmSocket.getOutputStream().write(chk_pt_byte_end_lng[num]);

                    Log.i("check_pt_lng", String.valueOf(chk_pt_byte_end_lng[num]));
                }

                mmSocket.getOutputStream().write(dest_limit.getBytes());

                //            for (int num1 = 0; num1 < check_path_pt.size(); num1++){
                //   map1_main.addMarker(new MarkerOptions().position(check_pt_new).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE)));
//                }

                routes.add(path1);

            } catch (JSONException e) {
                e.printStackTrace();
            } catch (Exception e) {
            }
            return routes;
        }

        public byte[] convert_to_byte_chk_pt(double input, int size, int precision) {
            double tempInput = input;
            tempInput *= Math.pow(10, precision);
            int output = (int) tempInput;
            String strOut = String.format("%0" + size + "d", output);
            return strOut.getBytes();
        }

        private List<LatLng> decodePoly(String encoded) {

            Log.i("encode", encoded);
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

    public void connect() {


        if (!bleAdapter.isEnabled()) {
            bleIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(bleIntent, REQUEST_ENABLE_BT);
        }

        if (bleAdapter.isDiscovering()) {
            bleAdapter.cancelDiscovery();
        }
        bleAdapter.startDiscovery();


    }

    public class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {

            if (device == null) {
                Log.i("Status", "Device is null");
            }

            mmDevice = device;
            try {
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
            }
            mmSocket = tmp;
        }

        public void run() {
            Log.i("Tag", "Reached connect run");
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
            } catch (IOException e) {
            }
        }
    }

    public class ConnectedThread extends Thread {


        public ConnectedThread(BluetoothSocket socket) {
            Log.i("Tag", "Reached connected thread");
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
            //    while (true) {


//                try {
//                    readBuffer = new byte[1024];
//                    bytesAvailable = mmSocket.getInputStream().available();
//                 //   if(bytesAvailable > 0 && enter_flag == true) {
//                        byte[] packetBytes = new byte[bytesAvailable];
//                        mmSocket.getInputStream().read(packetBytes);
//                        byte b ;//= packetBytes[0];
//                    for(i=1;i<45;i++){
//                        b = packetBytes[i];
//                        Log.i("bytes", String.valueOf(b));
//                    }
//                        if (b == delimiter1 && b != 0x00 && enter_flag == true) {
//                            for (i = 1; i < 32 && enter_flag == true; i++) {
//                                b = packetBytes[i];
//                                if (b == com_delimiter && b != 0x00) {
//                                    byte[] encodedBytes = new byte[readBufferPosition];
//                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
//                                    data2 = new String(encodedBytes, "US-ASCII");
//                                    start_lat = Double.parseDouble(data2);
//                                    enter_map1 = true;
//                                    readBufferPosition = 0;
//
//                                    enter_map1 = true;
//                                    Log.i("Buffer1 data", String.valueOf(start_lat));
//                                } else if (b == delimiter && b != 0x00) {
//                                    byte[] encodedBytes = new byte[readBufferPosition];
//                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
//                                    data1 = new String(encodedBytes, "US-ASCII");
//                                    start_lng = Double.parseDouble(data1);
//                                    enter_map2 = true;
//                                    readBufferPosition = 0;
//
//                                    Log.i("Buffer2 data", String.valueOf(start_lng));
//
//                                } else if (b >= 0x30 || b <= 0x39 || b == 0x2E || b != delimiter1 || b != delimiter) {
//
//                                    readBuffer[readBufferPosition++] = b;
//                            }
//
            //   }

            //  enter_flag = false;
            //    }

            //   }


//                } catch (IOException e) {
//                    e.printStackTrace();
//                }


            //    }

        }

        public void write(byte[] bytes) {
            Log.i("Tag", "Reached connected write");
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

    void system_status() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //listviewble.setVisibility(View.INVISIBLE);
                // status.setVisibility(View.VISIBLE);
                //text_display.setText(data1);
                blevalue.setText(ble_status);
                iovalue.setText(io_status);
                mastervalue.setText(mas_status);
                motorvalue.setText(mot_status);
                sensorrvalue.setText(sen_status);
            }
        });
    }

    private class ConnectTask extends AsyncTask<String, Void, Void> {
        @Override
        protected Void doInBackground(String... url) {
            connect();

            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);

            pairedDevices = bleAdapter.getBondedDevices();
            if (pairedDevices.size() > 0) {
                pairedDeviceArrayList = new ArrayList<BluetoothDevice>();

                for (BluetoothDevice device : pairedDevices) {
                    pairedDeviceArrayList.add(device);
                }
            }
            arrayAdapter = new ArrayAdapter<BluetoothDevice>(MainActivity.this, R.layout.listview_item, pairedDeviceArrayList);
            bluetoothcontrol.setVisibility(View.VISIBLE);
            listViewPairedDevice.setAdapter(arrayAdapter);
            listViewPairedDevice.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {


                    device1 = (BluetoothDevice) adapterView.getItemAtPosition(i);

                    mConnectThread = new ConnectThread(device1);
                    mConnectThread.start();
                    flag = true;


                    bluetoothcontrol.setVisibility(View.INVISIBLE);
                    // listViewPairedDevice.setVisibility(View.INVISIBLE);

                }
            });


            start.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                   // enter_flag = false;
                    sensorstatus.setVisibility(View.INVISIBLE);
                    sys_status.setVisibility(View.INVISIBLE);
                    misc.setVisibility(View.INVISIBLE);
                    maplayout.setVisibility(View.INVISIBLE);
                    //   mapFragment.getView().setVisibility(View.VISIBLE);
                    int count1 = 0;
                    while (count1 <= 5) {
                        byte[] send = message_1.getBytes();
                        mConnectedThread.write(send);
                        count1++;
                    }
                    enter_flag = true;
//                    DisplayTask displayTask = new DisplayTask();
//                    displayTask.execute();
                   // handler.postDelayed(r, 1000);
                }
            });


            stop.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                   // enter_flag = false;
                    sensorstatus.setVisibility(View.INVISIBLE);
                    sys_status.setVisibility(View.INVISIBLE);
                    misc.setVisibility(View.INVISIBLE);
                    maplayout.setVisibility(View.INVISIBLE);
                    int count = 0;
                    while (count <= 5) {
                        byte[] send = message_2.getBytes();
                        mConnectedThread.write(send);
                        count++;
                    }
                }
            });
        }

    }

    HashMap<Integer, byte[]> displayMap;

    private class DisplayTask extends AsyncTask<String, Void, ArrayList<Byte>> {
        @Override
        protected ArrayList<Byte> doInBackground(String... url) {

            displayMap = new HashMap<Integer, byte[]>();

            if(enter_flag) {

                readBuffer = new ArrayList<>();
                try {
                    bytesAvailable = mmInStream.available();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                if (bytesAvailable > 0) {
                    byte[] packetBytes = new byte[bytesAvailable];
                    try {
                        mmInStream.read(packetBytes);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    byte b = packetBytes[0];

                    if (b == delimiter1_at) {
//                    flag = true;
//                  //  enter_flag = false;
//                }
//                if(flag) {
                        Log.i("bytes_1", String.valueOf(b));

                        for (int i = 1; i < bytesAvailable; i++) {
                            // i = 1;
                            b = packetBytes[i];
                            Log.i("bytesAvailable", String.valueOf(bytesAvailable));
                            Log.i("index", String.valueOf(i));
                            Log.i("bytes", String.valueOf(b));
                            if (b == delimiter_dollar) {
                                bytesAvailable = 0;
                               // enter_flag = false;
                                //  readBufferPosition = 0;
                            }

                            // while (b != delimiter_dollar) {

                            else if (b != com_delimiter & b != delimiter1_at) {
                                readBuffer.add(b);
                                // readBufferPosition++;

                            }
                            // readBufferPosition++;
                        }
                    }


                    // Log.i("bytes", String.valueOf(b));

//                        if (b == delimiter_dollar && b != 0x00) {
//                          //  byte[] encodedBytes = new byte[readBufferPosition];
//                           // System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
//                            for(int k = 0; k<readBuffer.length; k++){
//                                byte_string[k] = String.valueOf(readBuffer[k]);
//                           }
////                            displayMap.put(j, readBuffer);
////                            Log.i("displayMap", String.valueOf(displayMap.get(j)));
////                            j++;
//                            readBufferPosition = 0;
//                        }
//                        } else if (b == delimiter && b != 0x00) {
//                            byte[] encodedBytes = new byte[readBufferPosition];
//                            System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
//                            data1 = new String(encodedBytes, "US-ASCII");
//                            start_lng = Double.parseDouble(data1);
//                            enter_map2 = true;
//                            readBufferPosition = 0;
//
//                            Log.i("Buffer2 data", String.valueOf(start_lng));
//
//                        }
//                        else if ( b != com_delimiter) {
//
//                            readBuffer[readBufferPosition++] = b;
//                        }
                    // i++;


                    //  }


                }
                enter_flag = false;
            }



            return readBuffer;
        }

        @Override
        protected void onPostExecute(ArrayList<Byte> result) {
            super.onPostExecute(result);
            sensorstatus.setVisibility(View.VISIBLE);
            sys_status.setVisibility(View.VISIBLE);
            misc.setVisibility(View.VISIBLE);

            String left_sensor = readBuffer.get(0).toString();
            String middle_sensor = readBuffer.get(1).toString();
            String right_sensor = readBuffer.get(2).toString();
            String rear_sensor = readBuffer.get(3).toString();
            String critical_sensor = readBuffer.get(4).toString();

            if(left_sensor.contentEquals("48"))
                lstatus_image.setImageResource(R.drawable.sys_down);
            if(left_sensor.contentEquals("49"))
                lstatus_image.setImageResource(R.drawable.sys_up);
            if(middle_sensor.contentEquals("48"))
                mstatus_image.setImageResource(R.drawable.sys_down);
            if(middle_sensor.contentEquals("49"))
                mstatus_image.setImageResource(R.drawable.sys_up);
            if(right_sensor.contentEquals("48"))
                rstatus_image.setImageResource(R.drawable.sys_down);
            if(right_sensor.contentEquals("49"))
                rstatus_image.setImageResource(R.drawable.sys_up);
            if(rear_sensor.contentEquals("48"))
                rearstatus_image.setImageResource(R.drawable.sys_down);
            if(rear_sensor.contentEquals("49"))
                rearstatus_image.setImageResource(R.drawable.sys_up);
            if(critical_sensor.contentEquals("48"))
                cstatus_image.setImageResource(R.drawable.sys_down);
            if(critical_sensor.contentEquals("49"))
                cstatus_image.setImageResource(R.drawable.sys_up);


            String ble_status = readBuffer.get(5).toString();
            String geo_status = readBuffer.get(6).toString();
            String io_status = readBuffer.get(7).toString();
            String master_status = readBuffer.get(8).toString();
            String motor_status = readBuffer.get(9).toString();
            String sensor_status = readBuffer.get(10).toString();

            if(ble_status.contentEquals("48"))
                bleimage.setImageResource(R.drawable.sys_down);
            if(ble_status.contentEquals("49"))
                bleimage.setImageResource(R.drawable.sys_up);
            if(geo_status.contentEquals("48"))
                geoimage.setImageResource(R.drawable.sys_down);
            if(geo_status.contentEquals("49"))
                geoimage.setImageResource(R.drawable.sys_up);
            if(io_status.contentEquals("48"))
                ioimage.setImageResource(R.drawable.sys_down);
            if(io_status.contentEquals("49"))
                ioimage.setImageResource(R.drawable.sys_up);
            if(master_status.contentEquals("48"))
                masterimage.setImageResource(R.drawable.sys_down);
            if(master_status.contentEquals("49"))
                masterimage.setImageResource(R.drawable.sys_up);
            if(motor_status.contentEquals("48"))
                motorimage.setImageResource(R.drawable.sys_down);
            if(motor_status.contentEquals("49"))
                motorimage.setImageResource(R.drawable.sys_up);
            if(sensor_status.contentEquals("48"))
                sensorimage.setImageResource(R.drawable.sys_down);
            if(sensor_status.contentEquals("49"))
                sensorimage.setImageResource(R.drawable.sys_up);

            String sys_cmd = readBuffer.get(11).toString();
            if(sys_cmd.contentEquals("48"))
                sysval_val.setImageResource(R.drawable.sys_down);
            if(sys_cmd.contentEquals("49"))
                sysval_val.setImageResource(R.drawable.sys_up);


            String speed = readBuffer.get(12).toString() + readBuffer.get(13).toString();
            speed_val.setText(speed);


            String des_rchd = readBuffer.get(14).toString();

            if(des_rchd.contentEquals("49"))
                dstrchd.setText("Destinaltion Reached");

         //   String battery_stat = readBuffer.get(15).toString() + readBuffer.get(16).toString();
          //  btryval.setText(battery_stat);


//
//
//            }


        }
    }
}




