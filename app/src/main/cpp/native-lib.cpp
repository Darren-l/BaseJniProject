#include <jni.h>
#include <string>
#include <iostream>


//todo 通常会封装Log日志。
#include "Log.h"

//todo 头文件可以直接导入，但通常会在makefile中申明一个专门放.h文件的目录，
// cpp文件则需要在makefile中申明加入编译。
#include "CPlusDemo.h"


extern "C" {
#include "include/libavutil/avutil.h"
}

/**
 * extern "C"： 使用C的环境。
 *
 * jstring JNICALL：返回值为jni层的string类型。
 *
 */
extern "C" JNIEXPORT jstring JNICALL
Java_cn_gd_snm_basejnipro_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "DarrentTest...";
    return env->NewStringUTF(hello.c_str());
}

/**
 * darrenNote -- 01:测试接受java端的基础数据类型。
 *
 *  env：java上下文，调用java方法需要通过该上下文来访问。
 *  thiz：java层调用该jni接口的对象，这里是MainActivity。
 *
 * 注意：
 *  每个jni接口必定有两个默认参数：
 *      1. env：作为java环境的上下文，作用是调用java接口时，需要借助env。
 *      2. thiz:如果jni接口是由对象调用，则thiz对应的是该对象，也就是jobject类型。
 *      若Jni接口是静态类调用，则thiz对应的是jclass。
 *
 *  当前接口是由MainActivity中申明的非静态方法，所以thiz对应的是jobject。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_MainActivity_sendBaseDataToJni(JNIEnv *env, jobject thiz,
                                                         jint i,
                                                         jshort sh, jfloat fl, jdouble dou,
                                                         jboolean bo, jstring str) {
    LOGD("sendIntDataToJni...");
    //todo 基础类型可以直接接收使用。
    jint ji = i;
    jshort js = sh;
    jfloat jf = fl;
    jdouble jd = dou;

    LOGD("输出基础类型，ji=%d , js=%d , jf=%f, jd=%lf \n", ji, js, jf, jd);

    //todo boolean类型在jni层是unsigned char,但是也不能直接输出...
//    LOGD("输出jboolean类型：bo=%s \n",bo);     //输出会报错。
    if (bo == JNI_TRUE) {     //但可以使用jni封装的变量进行判断。
        LOGD("jboolean 的值为true... \n");
    }

    //todo 针对非基础类型，需要就借助env。返回值是const char*，但可以使用const_cast转化为普通指针。
    const char *strC = env->GetStringUTFChars(str, NULL);
    char *strC2 = const_cast<char *>(env->GetStringUTFChars(str, NULL));
    strcpy(strC2, "jniModify..");
    LOGD("输出str类型：str=%s ,str2=%s", strC, strC2);

}

/**
 * darrenNote：测试访问对象成员。
 *
 *  ##### 在jni层，调用java对象成员或方法的过程类似于java中的反射，首先要记住几个基础对象。
 *
 * 基础对象：
 *      jclass：相当于反射中的Class对象，代表当前类。
 *      jfieldID：相当于反射中的Field，代表对象成员。
 *      jmethodID：相当于反射中的Method，代表方法成员。
 *
 * 基础对象的运用：
 *      jclass：可以在jni层直接创建，也可以通过对象获取。（和反射一样）
 *      jfieldID：如果要访问java的成员，必须先获取id，然后再通过GetXXXField的方式获取实际的值。
 *  基础类型XXX是对应的类型，非基础类型则是Object需要强转。
 *      jmethodID：同理，如果要调用对象方法，同样先要获取id，然后再使用接口进行方法调用。
 *
 *  总之jni层对象的操作与反射几乎一致。
 *
 *  ##### 静态与非静态的调用区别
 *
 *      静态：jclass + GetStaticXXXX就能获取到对应的成员或方法。
 *      动态：jclass + obj + GetXXXX
 *
 * ##### java与Kotlin区别
 *
 *  最主要的区别还是在于静态类成员使用：
 *      静态成员：java和kotlin一致。
 *      静态方法：kotlin需要获取到companion静态对象进行操作。
 *
 *  java层的区别主要在于方法声明方式：
 *      java：native XXX
 *      kotlin：extern XXX
 *
 *
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_MainActivity_sendObjectDataToJni(JNIEnv *env,
                                                           jobject thiz,
                                                           jobject person) {
    LOGD("sendObjectDataToJni...");
    /**
     * 针对类，同样也要借助env，现将对象转化为jclass，接下来都要借助env来操作jclass。
     *
     */
    //TODO: 测试获取对象的成员
    jclass jp1 = env->GetObjectClass(person);
    jfieldID jfid1 = env->GetFieldID(jp1, "name", "Ljava/lang/String;");
    jstring js1 = (jstring) env->GetObjectField(person, jfid1);
    const char *javaName = env->GetStringUTFChars(js1, NULL);
    LOGD("person filed name=%s \n", javaName);


    LOGD("####################  \n");

    //TODO: 测试获取对象的静态成员 -- java静态
    jclass jp11 = env->GetObjectClass(person);
    jfieldID jd11 = env->GetStaticFieldID(jp11, "staticNameJava", "Ljava/lang/String;");
    jstring js11 = (jstring) env->GetStaticObjectField(jp11, jd11);
    const char *javaStaticName = env->GetStringUTFChars(js11, NULL);
    LOGD("person static java filed=%s \n", javaStaticName);

    LOGD("####################  \n");


    //TODO：测试获取Kotlin companion成员 -- 与java调用静态一样。
    jclass jp12 = env->GetObjectClass(person);
    jfieldID jd12 = env->GetStaticFieldID(jp12, "staticNameKt", "Ljava/lang/String;");
    jstring js12 = (jstring) env->GetStaticObjectField(jp12, jd12);
    const char *javaStaticName2 = env->GetStringUTFChars(js12, NULL);
    LOGD("person static kotlin filed=%s \n", javaStaticName2);


    LOGD("####################  \n");


    //TODO: 测试调用对象带参数的public方法
    jclass jp2 = env->GetObjectClass(person);
    jmethodID j_pub_fun = env->GetMethodID(jp2, "pubFun", "(ILjava/lang/String;)I");
    jstring jst = env->NewStringUTF("jni 发送数据到java...");
    jint result = env->CallIntMethod(person, j_pub_fun, 100, jst);
    LOGD("jni层接受pub方法的返回值为 result=%d \n", result);

    LOGD("####################  \n");

    //TODO: 测试调用对象的private方法 -- 可以正常调用私有方法并传递值。
    jclass jp3 = env->GetObjectClass(person);
    jmethodID j_pri_fun = env->GetMethodID(jp3, "priFun", "(I)V");
    env->CallVoidMethod(person, j_pri_fun, 100);

    LOGD("####################  \n");


    //TODO: 测试调用对象的java静态方法
    jclass jp4 = env->GetObjectClass(person);
    jmethodID jd4 = env->GetStaticMethodID(jp4, "staticFunAboutJava", "()V");
    env->CallStaticVoidMethod(jp4, jd4);
//
    LOGD("####################  \n");


    /**
     * companion的实质：在kotlin中是个静态的内部类，申明也是个静态的成员。
     *
     *  1. 通过GetStaticFieldID获取companion静态成员id。
     *  2. 通过GetStaticObjectField + id获取到静态成员
     *  3. 获取Companion中的函数。
     *  4. 调用函数。
     *
     * 可以看出，kotlin的“静态”函数实际调用起来比较麻烦，还是直接使用@JvmStatic 标记为java静态jni会更好操作一些。
     *
     */
    //TODO: 测试获取Kotlin companion静态方法
    jclass jp9 = env->GetObjectClass(person);
    jclass jp10 = env->FindClass("cn/gd/snm/basejnipro/Person$Companion");

    jfieldID jf9 = env->GetStaticFieldID(jp9, "Companion",
                                         "Lcn/gd/snm/basejnipro/Person$Companion;");
    jobject jo9 = env->GetStaticObjectField(jp9, jf9);
    jmethodID jd9 = env->GetMethodID(jp10, "staticFunAboutKt", "()V");
    env->CallVoidMethod(jo9, jd9);


    //TODO: 测试在jni层创建Person对象，并调用Person构造，回调object传递对象。
    jclass jc5 = env->FindClass("cn/gd/snm/basejnipro/Person");

    //实例化对象，不会调用构造
    jobject jo = env->AllocObject(jc5);
    jfieldID jf6 = env->GetFieldID(jc5, "name", "Ljava/lang/String;");
    jstring js6 = env->NewStringUTF("set jni Str");
    env->SetObjectField(jo, jf6, js6);    //设置下值
    jmethodID jm6 = env->GetMethodID(jc5, "printf", "()V");
    env->CallVoidMethod(jo, jm6);    //调用输出下刚才设置的值

    //TODO: 调用构造 -- 默认构造
    jclass jcc1 = env->FindClass("cn/gd/snm/basejnipro/Person");
    jmethodID jmm1 = env->GetMethodID(jcc1, "<init>", "()V");
    jobject joo1 = env->NewObject(jcc1, jmm1);
    jfieldID jff1 = env->GetFieldID(jcc1, "name", "Ljava/lang/String;");
    jstring jss1 = (jstring) env->GetObjectField(joo1, jff1);
    const char *cha = env->GetStringUTFChars(jss1, NULL);
    LOGD("调用默认构造生成对象，name = %s", cha);

    //TODO: 调用构造 -- 其他构造
    jclass jcc2 = env->FindClass("cn/gd/snm/basejnipro/Person");
    jmethodID jmm2 = env->GetMethodID(jcc2, "<init>", "(Ljava/lang/String;)V");
    jstring jst22 = env->NewStringUTF("jni name..");
    jobject joo2 = env->NewObject(jcc2, jmm2, jst22);
    jfieldID jff2 = env->GetFieldID(jcc2, "name", "Ljava/lang/String;");
    jstring jss2 = (jstring) env->GetObjectField(joo2, jff2);
    const char *cha2 = env->GetStringUTFChars(jss2, NULL);
    LOGD("调用带参数构造生成对象，name = %s", cha2);

    //TODO:正常情况下，局部引用的j对象并不需要手动释放，但最好也手动调用实现。
    env->DeleteLocalRef(joo2);

}


/**
 * 这个文件可以看做就是个C文件，支持extern，将具体的业务逻辑封装在其他.c文件中，这是C语言做法。
 *
 * 实际开发过程中，业务逻辑使用C++来封装更为方便，通过引入C++
 *
 */
extern int age;

extern void printf_name();

//todo 全局对象，需要调用NewGlobalRef才是真正的全局。
jobject j_global_obj;
/**
 * jni层作用域：
 *      与C不同，Jni层作用域并非写在全局的位置它就是全局，当一个jni的方法栈走完后，会自动释放在方法栈中申明的对象。
 *  如果想申明全局，必须调用env的global接口。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_MainActivity_testJniOther(JNIEnv *env,
                                                    jobject thiz) {
    LOGD("testJniOther...");
    //todo jni层的作用域 -- 全局的申明
    jclass j_global_class = env->FindClass("cn/gd/snm/basejnipro/Person");
    j_global_obj = env->NewGlobalRef(j_global_class);  //通过class初始化全局。

    //todo jni层对象的释放
    env->DeleteGlobalRef(j_global_obj);

    //todo 使用C的方式编写业务逻辑代码在.c中并输出。
    LOGD("输出C文件中的age=%d \n", age);
    printf_name();

    //todo 使用C++的方式编写业务逻辑代码并输出。
    CPlusDemo cPlusDemo;
    LOGD("输出C++文件中的age=%d \n", cPlusDemo.age);
    cPlusDemo.printf_name();
}



extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_MainActivity_printfFFmpegVersonFromJni(JNIEnv *env, jobject thiz) {
    unsigned int verson = avutil_version();
    LOGD("printfFFmpegVersonFromJni ver=%d \n", verson);
}


//todo ################  以下测试动态注册  ##################

/**
 *  动态注册总体分为两步：
 *      1. 实现与java层调用的jni函数。
 *      2. 编写java层到jni层的函数映射表。
 *      3. 在JNI_Onload中将该映射表进行注册。
 *
 *  实际动态注册和静态注册差不多，但效率会更
 *
 */

void testDynamicFunJni(JNIEnv *env, jobject thiz, jint age, jstring name);

/**
 * JNI映射表数组
 *
 */
static const JNINativeMethod jniNativeMethod[] = {
        {"testDynamicFun", "(ILjava/lang/String;)V", (void *) (testDynamicFunJni)}
};

/**
 * 与java对应的jni函数，这里必须要添加两个默认参数，如果是非静态则是jobject，如果是静态则是jclass。
 *
 */
void testDynamicFunJni(JNIEnv *env, jobject thiz, jint age, jstring name) {
    LOGD("testDynamicFun...");
    const char* jname = env->GetStringUTFChars(name, nullptr);
    LOGD("testDynamicFun,age=%d , name=%s",age,jname);
}

JavaVM* jvm = nullptr;

/**
 *  jni层加载该jni文件时，会先调用该函数，该函数默认存在，这里相当于重写。
 *
 */
extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVm, void *) {
    LOGD("GetEnv JNI_OnLoad...");

    //将jvm存储到全局
    jvm = javaVm;


    //todo 首先通过javaVm获取env上下文
    JNIEnv *jniEnv = nullptr;
    //注意，参数是二级指针，所以需要取址一级指针

    int ret = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);

    if (0 != ret) {
        LOGE("GetEnv 获取失败！");
        return -1;  //返回-1，程序会运行崩溃。
    }

    LOGD("GetEnv 获取成功...");

    /**
     * 参数有三个：
     *  1. java层映射的jclass。
     *  2. jni层的映射表。
     *  3. 映射表中方法的个数。
     *
     */
    const char* javaClassName = "cn/gd/snm/basejnipro/DanActivity";
    int methodsSize = sizeof(jniNativeMethod) /sizeof (JNINativeMethod);
    jclass javaClaz = jniEnv->FindClass(javaClassName);
    jniEnv->RegisterNatives(javaClaz,jniNativeMethod,methodsSize);

    LOGD("GetEnv java层方法注册完毕...");

    return JNI_VERSION_1_6; //貌似java1.6会比较稳定。
}



//todo ##################### 下面测试jni层开辟线程及evn和thiz的传递  ###############


#include <pthread.h>
#include <unistd.h>

// 抽象java层对应的对象相关引用
class ThreadActivityJni{
public:
    JNIEnv* env = nullptr;
    jobject actObj = NULL;
};


//void* (*__start_routine)(void*)
void * receiveCb(void *pVoid){
    LOGD("开启线程... callback...");

    char *name = (char*) pVoid;
    LOGD("开启线程... name = %s \n" , name);

    return nullptr;
}

/**
 * //todo 注意！！传递的对象不能是env，否则调用env函数会报错
 *
 * env不能跨线程传递，必须绑定当前线程使用。
 *
 * @param env
 * @return
 */
void* receiveObjCb(void* env){
    LOGD("开启线程... callback... \n");

    //todo ##### 错误演示，跨线程传递env并使用。
//    LOGD("开启线程... callback... envp= %p \n",env);    //有地址。
//    JNIEnv* t_env = (JNIEnv *) env;
//    LOGD("cast end... \n");
//    if(t_env == nullptr){
//        LOGE("线程中获取的env对象为空...\n");
//        return nullptr;
//    }
//    //尝试调用env
//    jclass jc = t_env->FindClass("cn/gd/snm/basejnipro/ThreadActivity");
//    //todo  此时报错
//    jfieldID jd12 = t_env->GetStaticFieldID(jc, "staticAge", "I");
//    jint js12 = (jint) t_env->GetStaticIntField(jc, jd12);
//    LOGD("尝试使用传递的env获取类，并获取类的静态成员...js12 = %d \n" , js12);


    //todo #### env在线程中的正确获取方式
    JNIEnv* th_env = nullptr;
    jint result = jvm->AttachCurrentThread(&th_env, nullptr);
    if(result != JNI_OK){
        LOGE("绑定失败！ \n");
        return nullptr;
    }

    return nullptr;
}


/**
 *
 * //todo thiz可以传递，但传递前必须使用NewGloble申明为全局。
 *
 *  通常在源码中，都会申明jni层类或结构体对应java中的实体。子线程绑定出来的env不具备完成的接口功能，如FindClass获取的
 *  就会为空。
 *
 *  另外C语言中的线程的参数只有一个，如果要向线程回调函数传递多个参数，需要自行使用类或者结构体进行封装。
 *
 *
 * @param obj
 * @return
 */
void* receiveObjCb2(void* obj){
    LOGD("开启线程... receiveObjCb2... \n");

    ThreadActivityJni* threadActivityJni = (ThreadActivityJni*) obj;

    JNIEnv* env;

    //todo 申明一个env绑定当前线程。
    jint result = jvm->AttachCurrentThread(&env, nullptr);
    if(result != JNI_OK){
        LOGE("新线程的env绑定失败！ \n");
        return nullptr;
    }

    //不可以使用其他线程的evn，在具体获取对象或执行函数时，会报错。
//    env = threadActivityJni->env;

    //调用actObj对象的方法。
    jclass j_act_class = env->GetObjectClass(threadActivityJni->actObj);
    jmethodID j_act_printf_id = env->GetMethodID(j_act_class,"testPrintf","()V");
    env->CallVoidMethod(threadActivityJni->actObj,j_act_printf_id); //可以正常调用。

    //todo 尝试使用env直接获取class，获取为空。子线程的evn不允许这么操作。
    // 可以考虑放到主线程做，然后变全局或者放参数传递过来。
    jclass jc = env->FindClass("cn/gd/snm/basejnipro/ThreadActivity");
    if(jc == nullptr){
        LOGE("获取的jc为空！ \n");    //会走这里。
        return nullptr;
    }

    jvm->DetachCurrentThread();

    return nullptr;
}


/**
 * JavaVM在jni的进程中全局唯一，绑定当前进程，可以跨线程。
 *
 * JNIEnv绑定的是当前线程，切换线程后失效，不可以当做线程开启参数跨线程传递给其他线程。
 *
 * jobject可以作为参数传递给其他线程，但需要将jobject定义为全局然后再传递。
 *
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_ThreadActivity_createThreadInJni(JNIEnv *env, jobject thiz,
                                                           jstring name) {
    LOGD("createThreadInJni...");
    const char* c_name = env->GetStringUTFChars(name, nullptr);

    //使用pthead开启线程
    pthread_t pid;

    //todo #### 测试传递基础数据类型。
//    pthread_create(&pid, nullptr,receiveCb,(void *)c_name);


    //todo #### 测试传递env，thiz
//    pthread_create(&pid, nullptr,receiveObjCb,(void*) env);

    //todo #### 测试传递thiz
    ThreadActivityJni* threadActivityJni = new ThreadActivityJni;
    threadActivityJni->env = env;
    //跨线程必须要申明全局
    threadActivityJni->actObj = env->NewGlobalRef(thiz);
    pthread_create(&pid, nullptr,receiveObjCb2,threadActivityJni);

    //等待子线程执行。
    pthread_join(pid, nullptr);
    LOGD("open thread end...");

}


//todo ################ java层和jni层映射缓存filed变量 ###########

//与java层对应的jni层的静态变量
static jfieldID j_cache_name1;
static jfieldID j_cache_name2;

/**
 * 测试jfieldID与java层的绑定关系。
 *
 * 结论：java层修改值后，jfieldID对应也会修改，也就是说jfieldID实际上就是个java层对象的索引。
 *
 * 如果java层有个对象反复用到，可以考虑将其jfieldID直接缓存在jni层，方便访问也节约性能。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_ThreadActivity_cacheJni(JNIEnv *env, jobject thiz, jstring cache_name1,
                                                  jstring cache_name2) {
    LOGD("cacheJni... \n");
    //测试java修改变量后，重新获取值，查看fieldId是否存在映射关系。
    jclass jcl = env->GetObjectClass(thiz);
    jstring name1 = (jstring) env->GetStaticObjectField(jcl,j_cache_name1);
    jstring name2 = (jstring) env->GetStaticObjectField(jcl,j_cache_name2);

    char* c_name = (char*) env->GetStringUTFChars(name1, nullptr);
    char* c_name2 = (char*) env->GetStringUTFChars(name2, nullptr);

    LOGD("cacheJni name1=%s, name2=%s \n", c_name,c_name2);
}

/**
 * 将java层的静态对象的id缓存到jni层静态。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_ThreadActivity_initJniCache(JNIEnv *env, jclass clazz,
                                                      jstring cache_name1, jstring cache_name2) {
    LOGD("initJniCache... \n");
    const char* name1 = env->GetStringUTFChars(cache_name1, nullptr);
    const char* name2 = env->GetStringUTFChars(cache_name1, nullptr);
    j_cache_name1 = env->GetStaticFieldID(clazz,name1, "Ljava/lang/String;");
    j_cache_name2 = env->GetStaticFieldID(clazz,name2, "Ljava/lang/String;");
}


//todo ####################  测试java与jni层异常的相互捕获及处理  #########################

/**
 * Jni层的异常特点：
 *  1. 一定会等走完这个函数才会抛出异常。
 *  2. 可以在jni层自行进行捕获消除异常。
 *  3. 也可以对java层抛出异常。
 *  4. 也可以捕获java层抛出来的异常，实际上就是第二点。
 *
 * 注意：Jni层的异常若是属于C层的错误，那么该错误无法捕获。简单来说，通常能捕获的异常都是手动抛出的异常和evn接口的异常。
 *
 *
 * Jni层 env异常及捕获特点：
 *
 *  1. 一个ExceptionClear只能清除一个异常，若在clear前出现了两个异常，程序还是会崩溃。
 *  2. 若单纯出现一个异常，jni函数仍然会走完，但会crash。如果出现一个异常后又出现一个新的异常，则程序直接崩溃。
 *  3. 不能捕获C的异常，只能捕获env也就是java层的异常，这个和C++的异常捕获类似。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_cn_gd_snm_basejnipro_ThreadActivity_exceptionJni(JNIEnv *env, jobject thiz) {
    LOGD("exceptionJni... \n");
    jclass jc_thiz = env->GetObjectClass(thiz);

    //todo #### 测试jni层出错，自行捕获。
//    env->GetStaticFieldID(jc_thiz,"DEMOFI","I");
//    jthrowable jt = env->ExceptionOccurred();
//    if(jt){
//        LOGE("jt is not null, show exception! \n");
//        //清除异常
//        env->ExceptionClear();
//        //若不清除异常，直接返回，还是会报错。
////        return;
//    }

    //todo #### 测试jni层C层出错，异常是否能捕获测试。
    // 结论：不能捕获C的异常，C出现异常后直接报错，程序不会继续向下。
//    int* a;
//    int b = *a + 2; //制造野指针异常。
//    jthrowable jt = env->ExceptionOccurred();   //尝试捕获并清理。
//    if(jt){
//        LOGE("jt is not null, show exception! \n");
//        //清除异常
//        env->ExceptionClear();
//    }

    //todo #### 测试jni层捕获java层接口抛出的异常。
    // 结论：可以捕获，但是当前函数会调用多次？？。
//    jmethodID jid = env->GetMethodID(jc_thiz,"testException","()V");
//    env->CallVoidMethod(thiz,jid);
//    jthrowable jt = env->ExceptionOccurred();
//    if(jt){
//        LOGE("java exception... \n");
//        env->ExceptionClear();
//    }

    //也可以是用check检查是否有异常。
//    if (env->ExceptionCheck()) {
//        env->ExceptionDescribe(); // 输出异常信息
//        env->ExceptionClear();
//    }

    //todo #### 测试jni层向java层抛异常，java层捕获。
    jclass clz = env->FindClass("java/lang/NoSuchFieldException");
    env->ThrowNew(clz, "Exception from jni...");

    LOGD("exceptionJni... end ... \n");
}