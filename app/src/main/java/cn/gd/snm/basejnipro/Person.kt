package cn.gd.snm.basejnipro

import android.util.Log

/**
 * 用于测试jni层对象接收、创建、访问对象成员。
 *
 *
 */
class Person() {

    private var TAG = "Darren123"

    companion object{

        //todo: 测试获取、修改、调用kt的“静态”域和方法。
        var staticNameKt = "static,kt,name"

        fun staticFunAboutKt(){
            Log.d("Darren123","staticFunAboutKt...")
        }

        //todo: 测试获取调用java的静态域和方法
        @JvmField
        var staticNameJava = "static,java,name"

        @JvmStatic
        fun staticFunAboutJava(){
            Log.d("Darren123","staticFunAboutJava...")
        }
    }

    var name = "darren"

    fun printf(){
        Log.d("Darren123","sprintf... name=${name}")
    }

    //todo: 测试jni调用构造
    constructor(name: String):this(){
        this.name = name
    }

    //todo 故意私有化，看下jni是否能通过对象获取值。
    private var age = 10

    fun pubFun(arg:Int,arg2:String):Int{
        Log.d(TAG,"pubFun... arg=${arg},arg2=${arg2}")
        return 100;
    }

    //todo 看下能否调用私有方法。
    private fun priFun(arg:Int){
        Log.d(TAG,"priFun...arg=${arg}")
    }

}