#ifndef ADAPTER_HPP_
#define ADAPTER_HPP_

#include <iostream>

// "Target"
class ISwitch
{
public:
    virtual void on() = 0;
    virtual void off() = 0;
    virtual ~ISwitch() = default;
};

// "Adaptee"
class LedLight
{
public:
    virtual void set_rgb(int r, int g, int b)
    {
        std::cout << "RGB(" << r << ", " << g << ", " << b << ")\n";
    }

    virtual ~LedLight() = default;
};

class AmoledLight
{
public:
    void set_rgb(int r, int g, int b)
    {
        std::cout << "Super shiny RGB(" << r << ", " << g << ", " << b << ")\n";
    }
};

// "Adapter"
class LedSwitchCA : public ISwitch, private LedLight
{
public:
    void on() override
    {
        set_rgb(255, 255, 255);
    }

    void off() override
    {
        set_rgb(0, 0, 0);
    }
};

// "Adapter"
class LedSwitchOB : public ISwitch
{
private:
    LedLight& led_;

public:
    LedSwitchOB(LedLight& adaptee)
        : led_(adaptee)
    {
    }

    void on() override
    {
        led_.set_rgb(255, 255, 255);
    }

    void off() override
    {
        led_.set_rgb(0, 0, 0);
    }
};

#endif /*ADAPTER_HPP_*/
