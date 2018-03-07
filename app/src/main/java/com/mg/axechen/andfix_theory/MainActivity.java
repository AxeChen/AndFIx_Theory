package com.mg.axechen.andfix_theory;

import android.os.Build;
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
        // TODO 注意 我这边这样写是因为Genymotion的模拟器的低版本和高版本路劲不一样.低版本的时候adp push命令没有权限。所以写死目录。
        String path = "/sdcard/Download";
        DexManager.getInstance().loadFile(new File(path, "out.dex"));
    }

}
