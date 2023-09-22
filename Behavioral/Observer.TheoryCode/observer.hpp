#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include <iostream>
#include <set>
#include <string>
#include <memory>

//////////////////////////////////////////////////////////////////////////////////////
template <typename TSource, typename... TEventArgs>
class Observer
{
public:
    virtual void update(TSource&, TEventArgs... args) = 0;
    virtual ~Observer() = default;
};

//////////////////////////////////////////////////////////////////////////////////////
template <typename TSource, typename... TEventArgs>
struct Observable
{
    void subscribe(std::weak_ptr<Observer<TSource, TEventArgs...>> observer)
    {        
        observers_.insert(observer);
    }
    
    void unsubscribe(std::weak_ptr<Observer<TSource, TEventArgs...>> observer) { observers_.erase(observer); }

protected:
    void notify(TSource& source, TEventArgs... args)
    {
        for (auto&& observer : observers_)
        {
            std::shared_ptr living_observer = observer.lock();
            if (living_observer)
            {
                living_observer->update(static_cast<TSource&>(*this), std::move(args...));
            }
        }
    }

private:
    using ObserverWeakPtr = std::weak_ptr<Observer<TSource, TEventArgs...>>;
    std::set<ObserverWeakPtr, std::owner_less<ObserverWeakPtr>> observers_;
};

#endif /*OBSERVER_HPP_*/
