package com.example.shilpa.selfdrivingcar;

import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import com.cardiomood.android.controls.gauge.BatteryIndicatorGauge;
import com.cardiomood.android.controls.gauge.SpeedometerGauge;
import com.cardiomood.android.controls.progress.CircularProgressBar;

public class Dashboard extends AppCompatActivity {
    private SpeedometerGauge speedometer;
    private BatteryIndicatorGauge batteryindicator;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dashboard);
        speedometer = (SpeedometerGauge) findViewById(R.id.speedometer);
        batteryindicator = (BatteryIndicatorGauge) findViewById(R.id.batteryindicator);
        speedometer.setMaxSpeed(50);
        speedometer.setLabelConverter(new SpeedometerGauge.LabelConverter() {
            @Override
            public String getLabelFor(double progress, double maxProgress) {
                return String.valueOf((int) Math.round(progress));
            }
        });
        speedometer.setMaxSpeed(50);
        speedometer.setMajorTickStep(5);
        speedometer.setMinorTicks(4);
        speedometer.addColoredRange(0, 30, Color.GREEN);
        speedometer.addColoredRange(30, 45, Color.YELLOW);
        speedometer.addColoredRange(45, 50, Color.RED);
        speedometer.setSpeed(33, 1000, 300);

        batteryindicator.setValue(80, 1000, 300);

        CircularProgressBar circ = (CircularProgressBar) findViewById(R.id.circularprogress);
        circ.setProgress(90, 1000);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_dashboard, menu);
        return true;
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
