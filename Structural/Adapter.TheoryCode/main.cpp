#include "adapter.hpp"
#include <iostream>

using namespace std;

class Client
{
public:
    void do_operation(ISwitch& s)
    {
        s.on();
        s.off();
    }
};

int main()
{
    Client client;

    cout << "-- do_operation on ClassAdapter" << endl;
    LedSwitchCA led_switch;
    client.do_operation(led_switch);

    cout << endl;

    cout << "-- do_operation on ObjectAdapter" << endl;
    LedLight led;
    LedSwitchOB adapter(led);
    client.do_operation(adapter);
}
