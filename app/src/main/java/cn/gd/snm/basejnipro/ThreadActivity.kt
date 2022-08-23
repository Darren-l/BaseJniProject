package cn.gd.snm.basejnipro

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_thread.*
import java.lang.Exception

/**
 * 1. 测试jni层开辟线程传递参数注意事项。
 * 2. 测试jni层于java层的对象映射静态缓存。
 * 3. 测试jni层于java层异常的相互捕获。
 *
 */
class ThreadActivity : AppCompatActivity() {

    var actName = "ThreadActivity..."


    companion object{

        val TAG = ThreadActivity::class.simpleName

        init {
            System.loadLibrary("basejnipro")
        }

        @JvmStatic
        external fun initJniCache(cacheName1: String, cacheName2: String)

        @JvmStatic
        var staticAge = 10

    //todo 源码中常用的静态缓存方式，同时在java层和jni层定义static变量，然后进行映射。当java层变量改变时，
    // 实时调用接口同步更新jni层static缓存。
        @JvmStatic
        var cacheName1 = "cache1..."
        @JvmStatic
        var cacheName2 = "cache2..."
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_thread)

        //todo 测试jni创建线程及参数传递
//        testJniCreateThread()

        //todo 测试java层与jni层的静态缓存映射
//        testCacheJni()

        //todo 测试java层与jni层的异常捕获
        testException()

    }

    private fun testException() {

//        exceptionJni()

        //测试捕获jni层的异常，可以正常捕获。
        try {
            exceptionJni()
        }catch (e:Exception){
            e.printStackTrace()
        }

    }

    //用于测试jni调用java方法出现异常时的捕获。
    fun callJavaExceptrion(){
        throw Exception("123123")
    }

    external fun exceptionJni()


    private fun testCacheJni() {
        initJniCache("cacheName1","cacheName3")

        th_tv.setOnClickListener{
            ThreadActivity.cacheName1 = "modify cache name 1..."
            ThreadActivity.cacheName2 = "modify cache name 2..."

            cacheJni(ThreadActivity.cacheName1,ThreadActivity.cacheName2)
        }
    }

    external fun cacheJni(cacheName1: String, cacheName2: String)


    private fun testJniCreateThread() {
        var name = "darrenCreateThread"
        createThreadInJni(name)

    }



    /**
     * 测试jni层调用java层方法。
     *
     */
    fun testPrintf(){
        Log.d(TAG,"testPrintf....")
    }

    external fun createThreadInJni(name: String)
}