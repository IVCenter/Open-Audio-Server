/**
 * @file orientation.cpp
 * @author Shree Chowkwale
 *
 * This client example for OAS takes a file as a command line argument, and
 * demonstrates how the audio server handles positional audio as well as orientation.
 */

#include <OASClient.h>
#include <iostream>
#include <unistd.h>

std::string processArgsAndInitConnection(int argc, char **argv);
void runExample(oasclient::Sound &sound);
void reset(oasclient::Sound &sound);
void part1(oasclient::Sound &sound);
void part2(oasclient::Sound &sound);
void part3(oasclient::Sound &sound);

#define PI 3.14159265

int main(int argc, char **argv)
{
    const std::string &filepath = processArgsAndInitConnection(argc, argv);

    // Create a sound using the file specified by filepath
    oasclient::Sound sound = oasclient::Sound(filepath);

    if (!sound.isValid())
    {
        std::cerr << "--> Unable to create the sound specified by '" << filepath
                << "'" << std::endl;
        exit(2);
    }

    runExample(sound);

    if (!oasclient::ClientInterface::shutdown())
    {
        std::cerr << "--> Shutdown of the connection with OAS failed!" << std::endl;
        exit(5);
    }

    return 0;
}

void runExample(oasclient::Sound &sound)
{
    if (!sound.isValid())
        return;

    std::cerr << "This example demonstrates some of the properties of directional audio." << std::endl;

    part1(sound);
    /*
    usleep(3000000);
    part2(sound);
    usleep(3000000);
    part3(sound);
    */
    std::cerr << std::endl << "Example finished." << std::endl;
}

void reset(oasclient::Sound &sound)
{
    // Set the listener's orientation so that it is looking down the positive Y axis,
    // and the "up" direction is in the positive Z axis. This means that the listener is
    // placed on the X-Y plane, with "up" being positive Z.
    oasclient::Listener::getInstance().setOrientation(0, 1, 0,   0, 0, 1);

    // Place the sound 3 units directly in front of the listener
    sound.setPosition(0, 0, 0);

    // No direction associated with the sound
    sound.setDirection(0, 0, 0);
}

void part1(oasclient::Sound &sound)
{
    reset(sound);

    std::cerr << "Beginning the first part of the orientation example." << std::endl;
    std::cerr << "The sound will remain stationary, and will be omnidirectional" << std::endl;
    std::cerr << "    (i.e. it will emit the sound equally in all directions)." << std::endl;
    std::cerr << "The listener's position will remain constant, but the orientation will not." << std::endl;
    std::cerr << "The listener's orientation will be rotated counter-clockwise in the X-Y plane." << std::endl;
    std::cerr << "This will mean that the sound will seem to move clockwise around the listener!" << std::endl;

    oasclient::Listener &listener = oasclient::Listener::getInstance();

    float angle = PI / 2;

    const float endAngle = angle + (2 * PI);
    const float partRunTime = 20;               // Run for approximately this duration, in seconds
    const float iterationSleepTime = 10000;     // Sleep for this amount each loop, in microseconds

    // Based on the above two constants, this is how much the angle needs to be incremented each iteration
    const float angleIncrement = (2 * PI) / (partRunTime * (1000000 / iterationSleepTime));

    sound.play();
    sound.setLoop(true);

    while (angle < endAngle)
    {
        listener.setOrientation(cos(angle), sin(angle), 0,  0, 0, 1);
        usleep(10000);
        angle += angleIncrement;
    }

    sound.stop();
}

void part2(oasclient::Sound &sound)
{
    reset(sound);
}

void part3(oasclient::Sound &sound)
{
    reset(sound);
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

    if (!oasclient::ClientInterface::initialize(ipAddrStr, port))
    {
        std::cerr << "--> Unable to create a connection with the server." << std::endl;
        exit(1);
        return "";
    }

    return filepath;
}

