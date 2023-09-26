#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "comm/v_type.h"
#include "comm/config.h"
#include "armor/armor.h"
//#include "vision/os/os_base.h"

using namespace cv;

class ProducerConsumer 
{
private:
    std::queue<vint> data_queue_;
    std::mutex mtx_;
    std::condition_variable cond_var_;
    const unsigned vint MAX_SIZE = 5; // queue max len is 5. 
public:
    void producer() {
        int count = 0;
        while(true) {
            std::unique_lock<std::mutex> lock(mtx_);
            cond_var_.wait(lock, [this] {return data_queue_.size() < MAX_SIZE; });
            data_queue_.push(count++);
            lock.unlock();
            cond_var_.notify_all(); //notify consumer.
            std::this_thread::sleep_for(std::chrono::microseconds(1)); // 1ms.
        }
    }

    void consumer() {
        while(true) {
            std::unique_lock<std::mutex> lock(mtx_);
            cond_var_.wait(lock, [this] {return !data_queue_.empty(); });
            int val = data_queue_.front();
            data_queue_.pop();
            lock.unlock();
            cond_var_.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}


int main(int argc,  char** argv){
    // use Producer / Consumer Model
    ProducerConsumer pc;
    std::thread producerThread(&ProducerConsumer::producer, &pc);
    std::thread consumerThread(&ProducerConsumer::consumer, &pc);
    recognition();
    return 0;
}
