#include <jni.h>
#include "dalvik.h"

extern "C"

typedef Object *(*FindObject)(void *thread, jobject jobject1);

typedef void *(*FindThread)();

FindObject findObject;
FindThread findThread;


extern "C"
JNIEXPORT void JNICALL
Java_com_mg_axechen_andfix_1theory_DexManager_replace(JNIEnv *env, jobject instance, jint sdk,
                                                      jobject wrongMethod, jobject rightMethod) {

    // 获取到错误的Method。
    Method *wrong = (Method *) env->FromReflectedMethod(wrongMethod);

    // 获取到补丁包中的正确的方法。
    Method *right = (Method *) env->FromReflectedMethod(rightMethod);

    // 获取虚拟机句柄
    void *dvm_hand = dlopen("libdvm.so", RTLD_NOW);

    // 获取findObject 和 FindThread
    findObject = (FindObject) dlsym(dvm_hand, sdk > 10 ?
                                              "_Z20dvmDecodeIndirectRefP6ThreadP8_jobject" :
                                              "dvmDecodeIndirectRef");
    findThread = (FindThread) dlsym(dvm_hand, sdk > 10 ? "_Z13dvmThreadSelfv" : "dvmThreadSelf");

    // 获取需要method方法
    jclass methodClaz = env->FindClass("java/lang/reflect/Method");
    jmethodID rightMethodId = env->GetMethodID(methodClaz, "getDeclaringClass",
                                               "()Ljava/lang/Class;");

    // 修改正运行时的属性，讲错误的属性进行修改
    jobject ndkObject = env->CallObjectMethod(rightMethod, rightMethodId);
    ClassObject *firstFiled = (ClassObject *) findObject(findThread(), ndkObject);
    firstFiled->status = CLASS_INITIALIZED;
    wrong->accessFlags |= ACC_PUBLIC;
    wrong->methodIndex = right->methodIndex;
    wrong->jniArgInfo = right->jniArgInfo;
    wrong->registersSize = right->registersSize;
    wrong->outsSize = right->outsSize;
//    方法参数 原型
    wrong->prototype = right->prototype;
    wrong->insns = right->insns;
    wrong->nativeFunc = right->nativeFunc;

}