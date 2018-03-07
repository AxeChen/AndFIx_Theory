package com.mg.axechen.andfix_theory;

import android.content.Context;
import android.os.Build;

import java.io.File;
import java.lang.reflect.Method;
import java.util.Enumeration;

import dalvik.system.DexFile;

/**
 * Created by AxeChen on 2018/3/5.
 * 修复的管理类
 */

public class DexManager {

    private static DexManager mInstance;

    public static DexManager getInstance() {
        if (mInstance == null) {
            synchronized (DexManager.class) {
                if (mInstance == null) {
                    mInstance = new DexManager();
                }
            }
        }
        return mInstance;
    }
    private Context context;

    public void setContext(Context context) {
        this.context = context;
    }

    /**
     * 加载补丁文件，
     */
    public void loadFile(File file) {
        try {
            // 加载补丁文件
            DexFile dexFile = DexFile.loadDex(file.getAbsolutePath(),
                    new File(context.getCacheDir(), "opt").getAbsolutePath(), Context.MODE_PRIVATE);
            // 找到class -- 获取修复好的Method
            Enumeration<String> entry = dexFile.entries();
            while (entry.hasMoreElements()) {
                String className = entry.nextElement();
                Class clz = dexFile.loadClass(className, context.getClassLoader());
                if (clz != null) {
                    fixClass(clz);
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 获取到修复好的class，修复class
     *
     * @param realClazz
     */
    public void fixClass(Class realClazz) {
        Method[] methods = realClazz.getDeclaredMethods();
        // 遍历集合，寻找正确的方法
        for (Method rightMethod : methods) {
            Replace replace = rightMethod.getAnnotation(Replace.class);
            if (replace == null) {
                continue;
            }
            // 获取错误的类
            String wrongClass = replace.clazz();
            // 获取错误的方法
            String wrongMethodName = replace.method();
            try {
                // 因为这个是已经安装到系统的APK，所以使用Class类加载。
                Class clazz = Class.forName(wrongClass);
                Method wrongMethod = clazz.getDeclaredMethod(wrongMethodName, rightMethod.getParameterTypes());
                if (Build.VERSION.SDK_INT <= 18) {
                    replace(Build.VERSION.SDK_INT ,wrongMethod, rightMethod);
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }


    public native void replace(int sdk, Method wrongMethod, Method rightMethod);

    static {
        System.loadLibrary("native-lib");
    }
}
