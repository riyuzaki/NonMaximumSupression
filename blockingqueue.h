#ifndef __BLOCKINGQUEUE_H__
#define __BLOCKINGQUEUE_H__

#include <mutex>
#include <condition_variable>
#include <queue>

template <class type> class BlockingQueue
{
public:

    BlockingQueue(unsigned int max_items)
    {
        maxItems = max_items;
    }

    void push(type t)
    {
        if(q.size() >= maxItems)
        {
            std::unique_lock<std::mutex> lck(fullMtx);
            while(q.size() >= maxItems)
            {
                fullCv.wait(lck);
            }
        }
        {
            std::unique_lock<std::mutex> lck(mtx);
            q.push(t);
            cv.notify_one();
        }
    }

    int try_push(type t)
    {
        if(q.size() >= maxItems)
        {
            return -1;
        }
        else
        {
            std::unique_lock<std::mutex> lck(mtx);
            q.push(t);
            cv.notify_one();
            return 0;
        }
    }

    type pop()
    {
        type t;
        {
            std::unique_lock<std::mutex> lck(mtx);
            while(q.size() == 0)
            {
                cv.wait(lck);
            }

            {
                std::unique_lock<std::mutex> lck(fullMtx);
                t = q.front();
                q.pop();
                fullCv.notify_one();
            }
        }
        return t;
    }

    size_t size()
    {
        return q.size();
    }

    unsigned int max_size()
    {
        return maxItems;
    }

private:

    std::mutex mtx, fullMtx;
    std::condition_variable cv, fullCv;
    unsigned int maxItems;
    std::queue<type> q;
};

#endif // __BLOCKINGQUEUE_H__
