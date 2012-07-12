#include <OASSound.h>
#include <iostream>
#include <unistd.h>

std::string processArgsAndInitConnection(int argc, char **argv);
void runExample(oasclient::OASSound &sound);

int main(int argc, char **argv)
{
    const std::string &filepath = processArgsAndInitConnection(argc, argv);

    // Create a test sound that is a simple sine wave corresponding to Middle-C (261.63 hz)
    // and 0 phase shift, with a 2 second duration
    oasclient::OASSound sound = oasclient::OASSound(filepath);

    if (!sound.isValid())
    {
        std::cerr << "--> Unable to create the test sound." << std::endl;
        exit(2);
    }

    runExample(sound);

    oasclient::OASClientInterface::shutdown();

    return 0;
}

void runExample(oasclient::OASSound &sound)
{
    if (!sound.isValid())
        return;

    float x, y, z, r, theta, velocityX, gain;
    x = y = z = r = theta = 0;
    const int RESOLUTION = 5;

    velocityX = 30;
    sound.setPosition(-10, 3, 0);
    if (!sound.setVelocity(velocityX, 0, 0))
        std::cerr << "set vel failed" << std::endl;
    else
        std::cerr << "set vel success" << std::endl;
    if (!sound.setLoop(true))
        std::cerr << "set loop success" << std::endl;
    else
        std::cerr << "set vel success" << std::endl;
    if (!sound.play())
        std::cerr << "play failed" << std::endl;
    else
        std::cerr << "play success" << std::endl;

    for (x = -10; x < 10; x += 0.25 / RESOLUTION)
    {
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);
    }

    for (; x < 17.5; x += 0.25 / RESOLUTION)
    {
        velocityX -= 1.0 / RESOLUTION;
        sound.setVelocity(velocityX, 0, 0);
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);

    }

    for (; x > 10; x -= 0.25 / RESOLUTION)
    {
        velocityX -= 1.0 / RESOLUTION;
        sound.setVelocity(velocityX, 0, 0);
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);

    }

    for (; x > -10; x -= 0.25 / RESOLUTION)
    {
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);
    }

    sound.setVelocity(0, 0, 0);
    sound.stop();
}


std::string processArgsAndInitConnection(int argc, char **argv)
{
    std::string ipAddrStr;
    unsigned short port;
    std::string filepath;

    // If only one argument, assume it is a filepath
    if (argc == 2)
    {
        // Use localhost address as default
        ipAddrStr = "127.0.0.1";
        port = 31231;
        filepath = argv[1];
    }
    else if (argc == 4)
    {
        ipAddrStr = argv[1];
        port = (unsigned short) atol(argv[2]);
        filepath = argv[3];
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " [<ip addr> <port>] <filepath>" << std::endl;
        exit(3);
        return "";
    }

    if (!oasclient::OASClientInterface::initialize(ipAddrStr, port))
    {
        std::cerr << "--> Unable to create a connection with the server." << std::endl;
        exit(1);
        return "";
    }

    return filepath;
}
