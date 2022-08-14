//
// Created by Darren on 2020/2/10.
//

/**
 *
 * DarrenAdd20200210:打印封装类。
 *
 */

#ifndef DARPLAYER_LOG_H
#define DARPLAYER_LOG_H

#include <android/log.h>

#define TAG "Darren123JniTag"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#endif //DARPLAYER_LOG_H
