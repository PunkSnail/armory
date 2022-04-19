#include <atomic>
#include <memory>

using namespace std;

/* NOTE: 线程安全的 CAS(compare and swap) 队列, 更适合用于单生产者模式
 * Thread-safe CAS queue, more suitable for single-producer model */

template <typename T>
class cas_queue_t
{
public:
    explicit cas_queue_t(uint32_t size);
    virtual ~cas_queue_t() {}

    bool enqueue(T &t);
    bool enqueue(T &&t) = delete;

    bool dequeue(T &t);

    bool empty(void);

    uint32_t size();

private:
    inline bool power_of_two(uint32_t num)
    {
        return num != 0 && (num & (num - 1)) == 0;
    }
    inline uint32_t ceil_power_of_two(uint32_t num)
    {
        num |= (num >> 1);
        num |= (num >> 2);
        num |= (num >> 4);
        num |= (num >> 8);
        num |= (num >> 16);
        return num - (num >> 1);
    }
    inline uint32_t roundup_power_of_two(uint32_t num)
    {
        return ceil_power_of_two((num - 1) << 1);
    }
    inline uint32_t index_of_queue(uint32_t index)
    {
        return index & (this->_size - 1);
    }
private:
    uint32_t _size;
    std::atomic<uint32_t> _length;

    std::atomic<uint32_t> _read_idx;
    std::atomic<uint32_t> _write_idx;
    /* 上一个完成入队的元素的索引, 如果它不等于 _write_idx, 则表明有写请求
     * 未完成
     * the index of the last completed equeue element, if it's not equal
     * to _write_idx, then there is an uncompleted write request */
    std::atomic<uint32_t> _last_write_idx;

    unique_ptr<T[]> _queue;
};

template <typename T>
cas_queue_t<T>::cas_queue_t(uint32_t size) : _length(0),
    _read_idx(0), _write_idx(0), _last_write_idx(0)
{
    if (1 == size) {
        this->_size = 2;
    }
    else {
        this->_size = power_of_two(size) ?
            size : roundup_power_of_two(size);
    }
    this->_queue = move(unique_ptr<T[]>(new T[this->_size]));
}

template <typename T>
bool cas_queue_t<T>::enqueue(T &data)
{
    uint32_t curr_read_idx;
    uint32_t curr_write_idx;
    do
    {
        curr_read_idx = this->_read_idx;
        curr_write_idx = this->_write_idx;

        if (index_of_queue(curr_write_idx + 1) ==
            index_of_queue(curr_read_idx))
        {
            return false; // queue is full
        }

    } while (!atomic_compare_exchange_weak
             (&this->_write_idx, &curr_write_idx, (curr_write_idx + 1)));

    this->_queue[index_of_queue(curr_write_idx)] = data;

    while (!atomic_compare_exchange_weak
           (&this->_last_write_idx, &curr_write_idx, curr_write_idx + 1))
    {
        sched_yield(); // relinquish the CPU, take a breath
    }
    atomic_fetch_add(&this->_length, uint32_t(1));

    return true;
}

template <typename T>
bool cas_queue_t<T>::dequeue(T &data)
{
    uint32_t curr_read_idx;
    uint32_t last_write_idx;
    do
    {
        curr_read_idx = this->_read_idx;
        last_write_idx = this->_last_write_idx;

        if (index_of_queue(last_write_idx) ==
            index_of_queue(curr_read_idx))
        {
            // the queue is empty
            break;
        }
        // retrieve the data from the queue
        data = this->_queue[index_of_queue(curr_read_idx)];

        if (atomic_compare_exchange_weak(&this->_read_idx, &curr_read_idx,
                                         curr_read_idx + 1))
        {
            atomic_fetch_sub(&this->_length, uint32_t(1));
            return true;
        }
    } while (1);

    return false;
}

template <typename T>
bool cas_queue_t<T>::empty()
{
    return 0 == this->_length;
}

template <typename T>
uint32_t cas_queue_t<T>::size()
{
    return this->_length;
}
