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

    size_t size()
    {
        return q.size();
    }

    unsigned int max_size()
    {
        return maxItems;
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

private:

    std::mutex mtx, fullMtx;
    std::condition_variable cv, fullCv;
    unsigned int maxItems;
    std::queue<type> q;
};
