#include <jni.h>
#include <string>
#include <iostream>


//todo 通常会封装Log日志。
#include "Log.h"

//todo 头文件可以直接导入，但通常会在makefile中申明一个专门放.h文件的目录，
// cpp文件则需要在makefile中申明加入编译。
#include "CPlusDemo.h"


/**
 * extern "C"： 使用C的环境。
 *
 * jstring JNICALL：返回值为jni层的string类型。
 *
 */
extern "C" JNIEXPORT jstring JNICALL
Java_cn_gd_snm_basejnipro_MainActivity_stringFromJNI(
        JNIEnv* env,
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
    jfloat  jf = fl;
    jdouble jd = dou;

    LOGD("输出基础类型，ji=%d , js=%d , jf=%f, jd=%lf \n",ji, js, jf,jd);

    //todo boolean类型在jni层是unsigned char,但是也不能直接输出...
//    LOGD("输出jboolean类型：bo=%s \n",bo);     //输出会报错。
    if(bo == JNI_TRUE){     //但可以使用jni封装的变量进行判断。
        LOGD("jboolean 的值为true... \n");
    }

    //todo 针对非基础类型，需要就借助env。返回值是const char*，但可以使用const_cast转化为普通指针。
    const char* strC = env->GetStringUTFChars(str,NULL);
    char* strC2 = const_cast<char *>(env->GetStringUTFChars(str, NULL));
    strcpy(strC2,"jniModify..");
    LOGD("输出str类型：str=%s ,str2=%s",strC,strC2);

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
    jfieldID jfid1 = env->GetFieldID(jp1,"name","Ljava/lang/String;");
    jstring js1 =(jstring) env->GetObjectField(person,jfid1);
    const char* javaName = env->GetStringUTFChars(js1,NULL);
    LOGD("person filed name=%s \n",javaName);


    LOGD("####################  \n");

    //TODO: 测试获取对象的静态成员 -- java静态
    jclass jp11 = env->GetObjectClass(person);
    jfieldID  jd11 = env->GetStaticFieldID(jp11,"staticNameJava", "Ljava/lang/String;");
    jstring js11 = (jstring)env->GetStaticObjectField(jp11,jd11);
    const char* javaStaticName = env->GetStringUTFChars(js11,NULL);
    LOGD("person static java filed=%s \n",javaStaticName);

    LOGD("####################  \n");


    //TODO：测试获取Kotlin companion成员 -- 与java调用静态一样。
    jclass jp12 = env->GetObjectClass(person);
    jfieldID  jd12 = env->GetStaticFieldID(jp12,"staticNameKt", "Ljava/lang/String;");
    jstring js12 = (jstring)env->GetStaticObjectField(jp12,jd12);
    const char* javaStaticName2 = env->GetStringUTFChars(js12,NULL);
    LOGD("person static kotlin filed=%s \n",javaStaticName2);


    LOGD("####################  \n");


    //TODO: 测试调用对象带参数的public方法
    jclass jp2 = env->GetObjectClass(person);
    jmethodID j_pub_fun = env->GetMethodID(jp2,"pubFun", "(ILjava/lang/String;)I");
    jstring jst = env->NewStringUTF("jni 发送数据到java...");
    jint result = env->CallIntMethod(person, j_pub_fun, 100,jst);
    LOGD("jni层接受pub方法的返回值为 result=%d \n", result);

    LOGD("####################  \n");

    //TODO: 测试调用对象的private方法 -- 可以正常调用私有方法并传递值。
    jclass jp3 = env->GetObjectClass(person);
    jmethodID j_pri_fun = env->GetMethodID(jp3,"priFun","(I)V");
    env->CallVoidMethod(person, j_pri_fun, 100);

    LOGD("####################  \n");


    //TODO: 测试调用对象的java静态方法
    jclass jp4 = env->GetObjectClass(person);
    jmethodID jd4 = env->GetStaticMethodID(jp4,"staticFunAboutJava", "()V");
    env->CallStaticVoidMethod(jp4,jd4);
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

    jfieldID jf9 = env->GetStaticFieldID(jp9,"Companion", "Lcn/gd/snm/basejnipro/Person$Companion;");
    jobject jo9 = env->GetStaticObjectField(jp9, jf9);
    jmethodID jd9 = env->GetMethodID(jp10,"staticFunAboutKt", "()V");
    env->CallVoidMethod(jo9,jd9);


    //TODO: 测试在jni层创建Person对象，并调用Person构造，回调object传递对象。
    jclass jc5 = env->FindClass("cn/gd/snm/basejnipro/Person");
    
    //实例化对象，不会调用构造
    jobject jo = env->AllocObject(jc5);
    jfieldID jf6 = env->GetFieldID(jc5,"name","Ljava/lang/String;");
    jstring js6 = env->NewStringUTF("set jni Str");
    env->SetObjectField(jo,jf6,js6);    //设置下值
    jmethodID jm6 = env->GetMethodID(jc5,"printf", "()V");
    env->CallVoidMethod(jo,jm6);    //调用输出下刚才设置的值

    //TODO: 调用构造 -- 默认构造
    jclass jcc1 = env->FindClass("cn/gd/snm/basejnipro/Person");
    jmethodID jmm1 = env->GetMethodID(jcc1,"<init>","()V");
    jobject joo1 = env->NewObject(jcc1,jmm1);
    jfieldID jff1 = env->GetFieldID(jcc1,"name", "Ljava/lang/String;");
    jstring jss1 = (jstring) env->GetObjectField(joo1,jff1);
    const char* cha = env->GetStringUTFChars(jss1,NULL);
    LOGD("调用默认构造生成对象，name = %s",cha);

    //TODO: 调用构造 -- 其他构造
    jclass jcc2 = env->FindClass("cn/gd/snm/basejnipro/Person");
    jmethodID jmm2 = env->GetMethodID(jcc2,"<init>","(Ljava/lang/String;)V");
    jstring jst22 = env->NewStringUTF("jni name..");
    jobject joo2 = env->NewObject(jcc2,jmm2,jst22);
    jfieldID jff2 = env->GetFieldID(jcc2,"name", "Ljava/lang/String;");
    jstring jss2 = (jstring) env->GetObjectField(joo2,jff2);
    const char* cha2 = env->GetStringUTFChars(jss2,NULL);
    LOGD("调用带参数构造生成对象，name = %s",cha2);

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
    LOGD("输出C文件中的age=%d \n" ,age);
    printf_name();

    //todo 使用C++的方式编写业务逻辑代码并输出。
    CPlusDemo cPlusDemo;
    LOGD("输出C++文件中的age=%d \n" ,cPlusDemo.age);
    cPlusDemo.printf_name();
}


