#pragma once

#include <vector>
#include <memory>

// A singleton class to manage memory
namespace Memory{
    template <typename T>
    class Manager{
    private:
        std::vector<std::shared_ptr<T>> allocated;
        // Delete copy and move constructors so that we only have a single static instance of memory manager
        Manager(Manager const&)=delete;
        Manager(Manager&&)=delete;
        auto& operator=(Manager const&)=delete;
        auto& operator=(Manager &&)=delete;

    public:
        Manager() = default;
        ~Manager() = default;

        template<typename... Args>
        T * create(Args&&... args){
            auto pointer = std::make_shared<T>(args...);
            allocated.push_back(pointer);
            return allocated.back().get();
        }

        T * manage(T* pointer){
            allocated.push_back(std::shared_ptr<T>(pointer));
            return allocated.back().get();
        }
    };

    // Maybe switch singleton implementation to return a singleton wrapper
    // Avoids having to assign reference to output of getInstance()
    template <typename T>
    class Singleton{
    private:
        Singleton() = default;
        ~Singleton() = default;
        // Delete copy and move constructors so that we only have a single static instance
        Singleton(Singleton const&)=delete;
        Singleton(Singleton&&)=delete;
        auto& operator=(Singleton const&)=delete;
        auto& operator=(Singleton &&)=delete;

    public:
        static T& getInstance(){
            static T instance;
            return instance;
        }
    };

    template <typename T>
    Manager<T>& get_manager() {
        return Singleton<Manager<T>>::getInstance();
    }
} // namespace Memory
