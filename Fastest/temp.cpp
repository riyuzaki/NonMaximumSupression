void* thingfinder_thread_work(void* sender);

void* thingfinder_thread_work(void* sender)
{
    /* Call the sender's thread routine. */
    ((ThingFinder*)sender)->Work();
    return NULL;
}

void ThingFinder::Init(pthread_attr_t* p_attr)
{
    for(uint32_t j = 0; j < NUM_THINGFINDER_THREADS; j++)
    {
        pthread_create(&workThread[j], p_attr, &thingfinder_thread_work, this);
    }
}


void ThingFinder::Work()
{
    while(1)
    {
        input = eoQueue.pop(); //input

        doNonMaxSupression(input);

    }

}

void PushInput(string input)
{
    for(uint32_t i = 0; i < eoQueue.size(); i++)
    {
        if(eoQueue->try_push(input) != 0)
        {
            cout << "eoQueue queue full" << endl;
            eoQueue->push(input);
        }
    }
}

int main(int argc, char* argv[])
{
    pthread_attr_t attr;
    struct sched_param prio;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    prio.sched_priority = 1;
    pthread_attr_setschedparam(&attr, &prio);

    ThingFinder tf;
    tf.Init(&cam, &fw, &attr);

    vector<String> filenames;
    String folder = "input";//argv[1];

    glob(folder, filenames);

    for(int i = 0; i < filenames.size(); i++){
        PushInput(filenames[i]);
    }


    while(1)
    {
        sleep(3);
    }

}
