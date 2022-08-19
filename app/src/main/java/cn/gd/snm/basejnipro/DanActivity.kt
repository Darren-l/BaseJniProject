package cn.gd.snm.basejnipro

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_dan.*

class DanActivity : AppCompatActivity() {

    companion object{
        init {
            System.loadLibrary("basejnipro")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_dan)

        da_tv.text = "test dac..."

        testDynamicRegister()

    }

    //todo 测试动态注册
    private fun testDynamicRegister() {

        var age = 10
        var name = "darren test dynamic fun..."
        testDynamicFun(age,name)
    }

    external fun testDynamicFun(age: Int, name: String)
}