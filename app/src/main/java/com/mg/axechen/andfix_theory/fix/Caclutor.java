package com.mg.axechen.andfix_theory.fix;

import com.mg.axechen.andfix_theory.Replace;

/**
 * 异常产生的类
 */
public class Caclutor {
    @Replace(clazz = "com.mg.axechen.andfix_theory.Caclutor", method = "caculator")
    public int caculator() {
        //10/0
        int i = 1;
        int j = 10;
        return j / i;
    }
}
