package com.mg.axechen.andfix_theory;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    //    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("native-lib");
//    }
    private TextView tvResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        DexManager.getInstance().setContext(this);
        tvResult = findViewById(R.id.tvResult);
    }

    public void calculate(View view) {
        Caclutor caclutor = new Caclutor();
        tvResult.setText("" + caclutor.caculator());
    }

    public void fixBug(View view) {
        String path = Environment.getExternalStorageDirectory().getAbsolutePath();
        Log.i("fixBug", path);
        File file = new File("/sdcard/Download", "out.dex");
        if (file.exists()) {
            DexManager.getInstance().loadFile(file);
        }
    }

}
