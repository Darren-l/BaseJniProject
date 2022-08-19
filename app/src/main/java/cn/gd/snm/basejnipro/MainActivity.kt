package cn.gd.snm.basejnipro

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import cn.gd.snm.basejnipro.databinding.ActivityMainBinding

/**
 * DarrenTest：测试java与jni层调用交互。
 *
 */
class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    val TAG = "Darren123"

    companion object {
        //todo kt在com中的init声明加载库，这个区域相当于静态区。
        init {
            System.loadLibrary("basejnipro")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.sampleText.text = stringFromJNI()

        //todo 测试基础类型传值
//        testJavaToJni()
        //todo 测试对象的传值
//        testJavaToJniObject()

        //todo 测试jni层其它
//        testJniOther()

        //todo 测试获取ffmpag so库版本号
        testGetFFmpegVerson()

    }

    private fun testGetFFmpegVerson() {
        printfFFmpegVersonFromJni()
    }

    external fun printfFFmpegVersonFromJni()


    //todo 测试Jni回调java并传参
    fun testCallBack(age:Int, name:String, person: Person){
        Log.d(TAG,"testCallBack... age=${age}, name=${name}")
        Log.d(TAG,"Person name=${person.name}")
    }

    /**
     * darrenNote: 测试调用jni接口，传递对象数据。
     *
     */
    private fun testJavaToJniObject() {
        var person = Person()
        person.name = "darrenModify..."
        sendObjectDataToJni(person)

    }

    var mainName = "defaultMain"
    var mainPer = Person("detaultMainPerson")
    /**
     * darrenNote：测试java调用jni接口，并传递数据。
     *
     */
    private fun testJavaToJni(){
        //todo 数据传递。
        Log.d(TAG,"testJavaToJni...")

        var i = 10
        var sh : Short = 1
        var fl : Float = 1.3f
        var dou:Double = 2.223
        var bo = true
        var str = "darrenTest"

        sendBaseDataToJni(i,sh,fl,dou,bo,str)
    }


    external fun testJniOther()
    external fun sendObjectDataToJni(person: Person)
    external fun sendBaseDataToJni(i: Int, sh: Short, fl: Float,
                                   dou: Double, bo: Boolean, str: String)

    //todo java写法：public native String stringFromJNI();
    // 实际上kotlin的写法更像是C的声明。
    external fun stringFromJNI(): String
}