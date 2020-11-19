//
// Created by kenny on 11/2/2020.
//
#include "callback.h"

#include <iostream>
#include <thread>
#include <variant>
#include <queue>
#include <chrono>
#include <optional>
#include <mutex>
#include <condition_variable>

#include "base_class.h"

namespace callback {
//#define USE_CONTEXT

    class DataA : public base_class::BaseClass {
    public:
        void fun() const override {
            std::cout << "    DataA" << std::endl;
        }
    };

    struct DataB {
        void fun() {
            std::cout << "    DataB" << std::endl;
        }
    };

    using Data = std::variant<DataA, DataB>;

    template<class T>
    void visit(T &&data) {
        std::visit([](auto &&data) {
            using D = std::decay_t<decltype(data)>;
            if constexpr (std::is_same_v<D, DataA>)
                data.fun();
            else if constexpr (std::is_same_v<D, DataB>)
                data.fun();
            else {
            }
        }, data);
    }

    template<class T>
    class Queue {
    public:
        Queue() = default;

        void push(T &&data) {
            std::cout << "pushing: " << std::endl;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                visit(data);
                queue_.emplace(std::move(data)); //DataA(DataA &&);
            }
            cv_.notify_all();
        }

        auto pop() -> std::optional<T> {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [&]() { return !queue_.empty() || depose_; });
            if (depose_) {
                return {};
            }
            auto data = std::move(queue_.front()); //DataA(DataA &&); ~DataA();
            queue_.pop(); //~DataA();
            lock.unlock();
            std::cout << "popping: " << std::endl;
            visit(data);
            return data; //DataA(DataA &&)
        }

        void depose() {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (depose_) return;
                depose_ = true;
            }
            cv_.notify_all();
        }

    private:
        std::queue<T> queue_ = {};
        std::mutex mutex_ = {};
        std::condition_variable cv_ = {};
        bool depose_ = false;
    };

    class Processor {
    public:
        Processor(Queue<Data> &queue) : queue_(queue) {}

        void run() {
            std::thread([&]() {
                while (true) {
                    if (auto data = queue_.pop()) {
                        std::cout << "processing: " << std::endl;
                        visit(*data);
                        // ~DataA();
                    } else {
                        break;
                    }
                }
            }).swap(thread_);
        }

        ~Processor() {
            queue_.depose();
            if (thread_.joinable()) {
                thread_.join();
            }
        }

    private:
        std::thread thread_;
        Queue<Data> &queue_;
    };

    Queue<Data> queue;

#ifdef USE_CONTEXT

    void (*g_fun)(void *, void *) = nullptr;

    void *g_context = nullptr;

    void regist_fun(void(*fun)(void *, void *), void *context) {
        g_fun = fun;
        g_context = context;
    }

    void call(void *data) {
        g_fun(data, g_context);
    }

    void callback(void *data, void *context) {
        auto queue = static_cast<Queue <Data> *>(context);
        queue->push(*static_cast<Data *>(data));
    }

#else

    void (*g_fun)(void *) = nullptr;

    void regist_fun(void(*fun)(void *)) {
        g_fun = fun;
    }

    void call(void *data) {
        g_fun(data);
    }

    void callback(void *data) {
        queue.push(Data{*static_cast<Data *>(data)});//DataA(DataA const&); ~DataA();
    }

#endif

    void test() {

#ifdef USE_CONTEXT
        regist_fun(callback, static_cast<void *>(&queue));
#else
        regist_fun(callback);
#endif
        auto processor = Processor{queue};
        processor.run();

        for (auto i = 1; i > 0; --i) {
            auto a = Data{DataA{}}; //DataA(); DataA(DataA &&); ~DataA(); ~DataA()
            call(static_cast<void *>(&a));

            auto b = std::make_unique<Data>(std::in_place_type<DataB>, DataB{});
            call(static_cast<void *>(b.get()));
            //DataA()
            //DataA(DataA &&)
            //~DataA()
            //DataA(DataA const&)
            //pushing:
            //DataA
            //        DataA(DataA &&)
            //~DataA()
            //~DataA()
            //DataA(DataA &&)
            //~DataA()
            //popping:
            //DataA
            //        DataA(DataA &&)
            //~DataA()
            //processing:
            //DataA
            //~DataA()

        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
}