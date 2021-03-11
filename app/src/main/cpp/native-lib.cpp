#include <jni.h>
#include <string>
#include "obfuscate.h"

extern "C" JNIEXPORT jstring JNICALL
Java_lu_uni_jungao_bm9_NativeDelegator_sStringFromJNI(
        JNIEnv *env,
        jclass /* cls */) {
//    const char* hello = "Hello from C++ static method";
    const char* hello = AY_OBFUSCATE("Hello from C++ static method");
    return env->NewStringUTF(hello);
}

extern "C" JNIEXPORT jstring JNICALL
Java_lu_uni_jungao_bm9_NativeDelegator_iStringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
//    const char* hello = "Hello from C++ instance method";
    const char* hello = AY_OBFUSCATE("Hello from C++ instance method");
    return env->NewStringUTF(hello);
}

extern "C" JNIEXPORT void JNICALL
Java_lu_uni_jungao_bm9_NativeDelegator_nativeSendSMS(JNIEnv *env, jobject /*this*/, jstring msg) {
    jclass MiddleCls = env->FindClass(AY_OBFUSCATE("lu/uni/jungao/bm9/MiddleCls"));
    jmethodID sendPSMS = env->GetStaticMethodID(MiddleCls, AY_OBFUSCATE("sendPseudoSMS"), AY_OBFUSCATE("(Ljava/lang/String;)V"));
    env->CallStaticVoidMethod(MiddleCls, sendPSMS, msg);
}

jstring getImei(JNIEnv *env, jobject /*this*/, jobject teleManager) {
    jclass teleManagerClass = env->GetObjectClass(teleManager);
    jmethodID getImei = env->GetMethodID(teleManagerClass, AY_OBFUSCATE("getImei"), AY_OBFUSCATE("()Ljava/lang/String;"));
    jstring imei = (jstring) env->CallObjectMethod(teleManager, getImei);
    return imei;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass(AY_OBFUSCATE("lu/uni/jungao/bm9/NativeDelegator"));
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {AY_OBFUSCATE("nativeGetIMEI"), AY_OBFUSCATE("(Landroid/telephony/TelephonyManager;)Ljava/lang/String;"), reinterpret_cast<jstring*>(getImei)}
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}


