#ifndef TOOLS__NOTIFIER_HPP
#define TOOLS__NOTIFIER_HPP

#include <functional>
#include <unordered_map>

template <typename ...ArgTypes>
class Notifier
{
    private:
        std::unordered_map<unsigned int, std::function<void(ArgTypes...)>> _listeners;
        unsigned int _count;

    public:
        Notifier();

        unsigned int addSubscriber(std::function<void(ArgTypes...)> callback);
        void deleteSubscriber(unsigned int id);

        void notify(ArgTypes&&... args);
};

template<typename ...ArgTypes>
Notifier<ArgTypes...>::Notifier() :
    _listeners(),
    _count(0)
{

}

template<typename ...ArgTypes>
unsigned int Notifier<ArgTypes...>::addSubscriber(std::function<void(ArgTypes...)> callback)
{
    _listeners[_count] = callback;
    return _count++;
}

template<typename ...ArgTypes>
void Notifier<ArgTypes...>::deleteSubscriber(unsigned int id)
{
    _listeners.erase(id);
}

template<typename ...ArgTypes>
void Notifier<ArgTypes...>::notify(ArgTypes&&... args)
{
    for (auto it = _listeners.begin(); it != _listeners.end(); it++)
    {
        (*it)(std::forward<ArgTypes>(args)...);
    }
}

#endif //TOOLS__NOTIFIER_HPP