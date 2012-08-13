#include <OASClient.h>
#include <iostream>
#include <unistd.h>

void processArgsAndInitConnection(int argc, char **argv);

int main(int argc, char **argv)
{
    processArgsAndInitConnection(argc, argv);

    // Create a test sound that is a simple sine wave corresponding to Middle-C (261.63 hz)
    // and 0 phase shift, with a 2 second duration
    oasclient::Sound test = oasclient::Sound(oasclient::Sound::SINE, 261.63, 0, 5);


    if (!test.isValid())
    {
        std::cerr << "--> Unable to create the test sound." << std::endl;
        exit(2);
    }

    test.setGain(0.2);      // Sound is initially at a low gain
    test.play();            // Play the sound
    test.fade(0.8, 3);      // Fade in gradually, over 3 seconds

    sleep(6);

    test.stop();
    test.release();

    oasclient::ClientInterface::shutdown();

    return 0;
}

void processArgsAndInitConnection(int argc, char **argv)
{
    std::string ipAddrStr;
    unsigned short port;

    if (argc == 1)
    {
        // Use localhost address as default
        ipAddrStr = "127.0.0.1";
        port = 31231;
    }
    else if (argc == 3)
    {
        ipAddrStr = argv[1];
        port = (unsigned short) atol(argv[2]);
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " [<ip addr> <port>]" << std::endl;
        exit(3);
    }

    if (!oasclient::ClientInterface::initialize(ipAddrStr, port))
    {
        std::cerr << "--> Unable to create a connection with the server." << std::endl;
        exit(1);
    }
}
