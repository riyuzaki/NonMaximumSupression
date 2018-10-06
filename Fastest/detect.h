#ifndef __DETECT_H__
#define __DETECT_H__

#include<iostream>
#include<stdio.h>

#include<vector>
#include<algorithm>
#include<string>
#include <sstream>

#include <fstream>
#include <iterator>
#include <unistd.h>

#include <experimental/filesystem> //C++17

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include "blockingqueue.h"

#define NUM_THINGFINDER_THREADS 4
#define MAX_THINGFINDER_QUEUE_LENGTH 4

using namespace std;
using namespace cv;

class DetectCars
{
    public:
        DetectCars() : eoQueue(MAX_THINGFINDER_QUEUE_LENGTH) {};
        void Init(pthread_attr_t* p_attr);
        void PushInput(string input);
        //void doNonMaxSupression(string filename);

    protected:
        std::mutex findMutex;
        float ComputeIOUScore(vector<float> a, vector<float> b);
        void aggregate(string filename);
        BlockingQueue<string> eoQueue;

        void Work();
        pthread_t workThread[NUM_THINGFINDER_THREADS];

    friend void* detectcars_thread_work(void* sender);
};

#endif // __DETECT_H__
