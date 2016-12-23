package com.example.mana1991.selfdrivingcar;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import static com.example.mana1991.selfdrivingcar.R.layout.activity_splash_screen;

public class SplashScreenActivity extends AppCompatActivity {

    protected int splashTime = 2500;
    private Context context;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_splash_screen);
        this.context = SplashScreenActivity.this;

        new Handler().postDelayed(new Runnable() {
            public void run() {
                Intent intent = new Intent(SplashScreenActivity.this, MainActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                if (isAlive(context)) {
                    startActivity(intent);
                    ((Activity) context).finish();

                }
            }

        }, splashTime);


    }
    public static boolean isAlive(Context context) {
        try {
            if (context != null && !((Activity) context).isFinishing()) {
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;

    }

}

