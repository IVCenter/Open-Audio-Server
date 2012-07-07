#include <OASSound.h>
#include <iostream>

int main()
{
    if (!oasclient::OASClientInterface::initialize("127.0.0.1", 31231))
    {
        std::cerr << "--> Unable to create a connection with the server." << std::endl;
        exit(1);
    }
    
    oasclient::OASSound test = oasclient::OASSound(oasclient::OASSound::SINE, 261.63, 0, 2);

    if (!test.isValid())
    {
        std::cerr << "--> Unable to create the test sound." << std::endl;
        exit(2);
    }

    test.play();

    return 0;
}
