package cn.gd.snm.basejnipro

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class ThreadActivity : AppCompatActivity() {

    var actName = "ThreadActivity..."

    companion object{

        val TAG = ThreadActivity::class.simpleName

        init {
            System.loadLibrary("basejnipro")
        }

        @JvmStatic
        var staticAge = 10
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_thread)

        testJniCreateThread()

    }

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