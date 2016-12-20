
package com.example.shilpa.selfdrivingcar;

import android.Manifest;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentSender;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentActivity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.github.clans.fab.FloatingActionButton;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;

import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PolylineOptions;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class NavigateActivity extends FragmentActivity implements OnMapReadyCallback,GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener,LocationListener {


        GoogleMap map1_main;

Intent navigateIntent;
        MapFragment mapFragment;

        ArrayList markers = null;
        ArrayList markers1;
        String data = "";

        HttpURLConnection urlConnection = null;

        FloatingActionButton start;
        FloatingActionButton end;
        FloatingActionButton check_pt;
        String message_1 = "b";
        String message_2 = "a";
        String lng_delimiter = "l";
        String lat_delimiter = "c";
        // String uart_mark = "b";
        String dest_limit = "$";

    BluetoothConnect connect;
    Boolean navigateservice = false;
    BluetoothSocket navigate_socket;
    BluetoothDevice navigate_device;
        InputStream iStream;
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
        char delimiter = 0x23;
        char delimiter1 = 0x2D;
        char com_delimiter = 0x2C;
        char delimiter2 = 0x62;
        char delimiter3 = 0x69;
        char delimiter4 = 0x73;
        char delimiter5 = 0x6D;
        char delimiter6 = 0x6C;
        byte[] readBuffer;
        int bytesAvailable = 0;
        TextView text_display;
        String data1;
        String data2;


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

    Bundle bnavigate;
        Context context;

        Handler handler = new Handler();
        Runnable r = new Runnable() {
            public void run() {
                handler.postDelayed(this, 100);
            }
        };
    ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            connect = ((BluetoothConnect.LocalBinder)service).getService();

        }

        public void onServiceDisconnected(ComponentName className) {

            connect = null;
        }
    };



        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_navigate);
            context = getApplicationContext();

            navigateIntent = new Intent(context,BluetoothConnect.class);
            //navigate_device= getIntent().getExtras().getParcelable("BTDevice");
             //bnavigate =  navigateIntent.getExtras();

            if(bindService(new Intent(context,
                    BluetoothConnect.class), mConnection, Context.BIND_AUTO_CREATE))
                // bindService(myIntent,mConnection,Context.BIND_NOT_FOREGROUND))
                navigateservice=true;
            mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);
            mapFragment.getView().setVisibility(View.VISIBLE);

            start = (FloatingActionButton) findViewById(R.id.button2);
            end =  (FloatingActionButton) findViewById(R.id.button3);
            check_pt = (FloatingActionButton) findViewById(R.id.button4);

            buildGoogleApiClient();
            mLocationRequest = LocationRequest.create()
                    .setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY)
                    .setInterval(10 * 1000)        // 10 seconds, in milliseconds
                    .setFastestInterval(1 * 1000);
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
        protected void onResume() {
            super.onResume();
            mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.map);
            mapFragment.getView().setVisibility(View.VISIBLE);
            mapFragment.getMapAsync(this);
            //Now lets connect to the API
            mGoogleApiClient.connect();
        }

        @Override
        public void onMapReady(final GoogleMap map1) {
            // Location location1;

            /*((BluetoothApplication)this.getApplicationContext()).bluetoothConnect.startThreadConnectBTdevice(navigate_device);
            ((BluetoothApplication)this.getApplicationContext()).bluetoothConnect.startThreadConnected(navigate_socket);*/
            start.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    enter_flag = false;
                    mapFragment.getView().setVisibility(View.VISIBLE);
                    int count1 = 0;
                    while (count1 <= 5) {
                        byte[] send = message_1.getBytes();

                        try {

                            navigate_socket.getOutputStream().write(send);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        count1++;
                    }
                    handler.postDelayed(r, 1000);
                }
            });


            end.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    enter_flag = false;
                    int count = 0;
                    while (count <= 5) {
                        byte[] send = message_2.getBytes();
                        connect.startThreadConnected(navigate_socket);
                        try {
                            navigate_socket.getOutputStream().write(send);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        count++;
                    }
                }
            });
            check_pt.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {

                    map1_main = map1;

                    markers = new ArrayList();
                    sanjose = new LatLng(37.3352, -121.8811);


                    start_pt = new LatLng(currentLatitude, currentLongitude);
                    Log.i("lat_start", String.valueOf(start_pt.latitude));
                    Log.i("lat_start", String.valueOf(start_pt.longitude));
                    // if (start_pt.latitude != 0 && start_pt.longitude != 0) {
                    map1_main.addMarker(new MarkerOptions().position(new LatLng(start_pt.latitude, start_pt.longitude)).title("Current location").icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN)));
                    // }


                    if (ActivityCompat.checkSelfPermission(NavigateActivity.this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(NavigateActivity.this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
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
                LocationServices.FusedLocationApi.requestLocationUpdates(mGoogleApiClient, mLocationRequest, (com.google.android.gms.location.LocationListener) this);

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
        public boolean onCreateOptionsMenu(Menu menu) {
            // Inflate the menu; this adds items to the action bar if it is present.
            getMenuInflater().inflate(R.menu.menu_navigate, menu);
            return true;
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

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {

        }

        @Override
        public void onProviderEnabled(String provider) {

        }

        @Override
        public void onProviderDisabled(String provider) {

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
               // Log.d("Exception while downloading url", e.toString());
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

    public class DirectionParser {

        /** Receives a JSONObject and returns a list of lists containing latitude and longitude */
        public List<List<HashMap<String,String>>> parse(JSONObject jObject){
            markerOptions = new MarkerOptions();

            List<List<HashMap<String, String>>> routes = new ArrayList<List<HashMap<String,String>>>() ;
            // List path1;

            try {

                jRoutes = jObject.getJSONArray("routes");

                /** Traversing all routes */
                jLegs = ( (JSONObject)jRoutes.get(0)).getJSONArray("legs");
                List path1 = new ArrayList<HashMap<String, String>>();

                /** Traversing all legs */
                jSteps = ( (JSONObject)jLegs.get(0)).getJSONArray("steps");

                for(int k=0;k<jSteps.length();k++){
                    String polyline = "";
                    polyline = (String)((JSONObject)((JSONObject)jSteps.get(k)).get("polyline")).get("points");
                    List<LatLng> list = decodePoly(polyline);

                    /** Traversing all points */
                    for(int l=0;l<list.size();l++){
                        HashMap<String, String> hm = new HashMap<>();
                        hm.put("lat", Double.toString((list.get(l)).latitude) );
                        hm.put("lng", Double.toString((list.get(l)).longitude) );
                        path1.add(hm);
                    }


                }
                //routes.add(path);

                /** Traversing all steps */
                int jStepslen = jSteps.length() - 1;

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
                    navigate_socket.getOutputStream().write(lat_delimiter.getBytes());
                    for (int num = 0; num < 8; num++) {
                        navigate_socket.getOutputStream().write(chk_pt_byte_lat[num]);

                        Log.i("check_pt_lat", String.valueOf(chk_pt_byte_lat[num]));
                    }
                    // mmSocket.getOutputStream().write(message_2.getBytes());

                    // handler.postDelayed(r, 10);

                    navigate_socket.getOutputStream().write(lng_delimiter.getBytes());

                    for (int num = 0; num < 11; num++) {
                        navigate_socket.getOutputStream().write(chk_pt_byte_lng[num]);

                        Log.i("check_pt_lng", String.valueOf(chk_pt_byte_lng[num]));
                    }


                    // mmSocket.getOutputStream().write(message_2.getBytes());
                    // handler.postDelayed(r, 1000);
                }
                polyline_end_lat = (Double) ((JSONObject) ((JSONObject) jSteps.get(jStepslen)).get("end_location")).get("lat");
                polyline_end_lng = (Double) ((JSONObject) ((JSONObject) jSteps.get(jStepslen)).get("end_location")).get("lng");
                chk_pt_byte_end_lat = convert_to_byte_chk_pt(polyline_end_lat, 8, chk_pt_precision);
                chk_pt_byte_end_lng = convert_to_byte_chk_pt(polyline_end_lng, 11, chk_pt_precision);

                navigate_socket.getOutputStream().write(lat_delimiter.getBytes());
                for (int num = 0; num < 8; num++) {
                    navigate_socket.getOutputStream().write(chk_pt_byte_end_lat[num]);

                    Log.i("check_pt_lat", String.valueOf(chk_pt_byte_end_lat[num]));
                }
                // mmSocket.getOutputStream().write(message_2.getBytes());

                // handler.postDelayed(r, 10);

                navigate_socket.getOutputStream().write(lng_delimiter.getBytes());

                for (int num = 0; num < 11; num++) {
                    navigate_socket.getOutputStream().write(chk_pt_byte_end_lng[num]);

                    Log.i("check_pt_lng", String.valueOf(chk_pt_byte_end_lng[num]));
                }

                navigate_socket.getOutputStream().write(dest_limit.getBytes());

                //            for (int num1 = 0; num1 < check_path_pt.size(); num1++){
                //   map1_main.addMarker(new MarkerOptions().position(check_pt_new).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE)));
//                }

                routes.add(path1);

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

        @Override
        public boolean onOptionsItemSelected(MenuItem item) {
            // Handle action bar item clicks here. The action bar will
            // automatically handle clicks on the Home/Up button, so long
            // as you specify a parent activity in AndroidManifest.xml.
            int id = item.getItemId();

            //noinspection SimplifiableIfStatement
            if (id == R.id.action_settings) {
                return true;
            }

            return super.onOptionsItemSelected(item);
        }
    }


