#include <jni.h>
#include <string>
#include "dalvik.h"
#include "art_7_0.h"

typedef Object *(*FindObject)(void *thread, jobject jobject1);

typedef void *(*FindThread)();

FindObject findObject;
FindThread findThread;

extern "C"
/**
 * Dalvik虚拟机修复原理
 * @param env
 * @param instance
 * @param sdk
 * @param wrongMethod
 * @param rightMethod
 */
JNIEXPORT void JNICALL
Java_com_mg_axechen_andfix_1theory_DexManager_replaceDalvik(JNIEnv *env, jobject instance, jint sdk,
                                                            jobject wrongMethod,
                                                            jobject rightMethod) {

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
    wrong->prototype = right->prototype;
    wrong->insns = right->insns;
    wrong->nativeFunc = right->nativeFunc;

}



extern "C"
/**
 * ART虚拟机修复原理
 * @param env
 * @param instance
 * @param wrongMethod
 * @param rightMethod
 */
JNIEXPORT void JNICALL
Java_com_mg_axechen_andfix_1theory_DexManager_replaceArt(JNIEnv *env, jobject instance,
                                                         jobject wrongMethod, jobject rightMethod) {

    //    art虚拟机替换  art  ArtMethod  ---》Java方法
    art::mirror::ArtMethod *wrong = (art::mirror::ArtMethod *) env->FromReflectedMethod(
            wrongMethod);
    art::mirror::ArtMethod *right = (art::mirror::ArtMethod *) env->FromReflectedMethod(
            rightMethod);

    wrong->declaring_class_ = right->declaring_class_;

    wrong->dex_code_item_offset_ = right->dex_code_item_offset_;
    wrong->method_index_ = right->method_index_;
    wrong->dex_method_index_ = right->dex_method_index_;

    wrong->ptr_sized_fields_.entry_point_from_jni_ = right->ptr_sized_fields_.entry_point_from_jni_;
    //    机器码模式
    wrong->ptr_sized_fields_.entry_point_from_quick_compiled_code_ = right->ptr_sized_fields_.entry_point_from_quick_compiled_code_;

    wrong->ptr_sized_fields_.entry_point_from_jni_ = right->ptr_sized_fields_.entry_point_from_jni_;
    wrong->ptr_sized_fields_.dex_cache_resolved_methods_ = right->ptr_sized_fields_.dex_cache_resolved_methods_;
    wrong->ptr_sized_fields_.dex_cache_resolved_types_ = right->ptr_sized_fields_.dex_cache_resolved_types_;
    wrong->hotness_count_ = right->hotness_count_;

}