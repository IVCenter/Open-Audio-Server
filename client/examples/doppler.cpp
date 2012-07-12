/**
 * @file doppler.cpp
 * @author Shree Chowkwale
 *
 * This client example for OAS takes a file as a command line argument, and 
 * demonstrates the audio server's rendering of the doppler effect.
 * 
 * The listener's position, velocity, and orientation stay constant, at their
 * default values, for the duration of this example.
 *
 * The sound is initially placed at <-10, 3, 0>, so that it is to the left and
 * slightly above the listener.
 *
 * The sound has an initial velocity of <30, 0, 0>, so that it is moving to the
 * right, relative to the listener.
 *
 * The sound moves along its velocity vector, slows down, reverses direction,
 * and then moves backwards, returning to where it originally was.
 *
 */

#include <OASSound.h>
#include <iostream>
#include <unistd.h>

std::string processArgsAndInitConnection(int argc, char **argv);
void runExample(oasclient::OASSound &sound);

int main(int argc, char **argv)
{
    const std::string &filepath = processArgsAndInitConnection(argc, argv);

    // Create a sound using the file specified by filepath
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

    // This resolution controls the rate at which the demo should run.
    // Increasing the resolution results in a finer "grain" of position and
    // velocity updates, and increases the load on the server and network.
    const int RESOLUTION = 5;

    // Setup the sound with initial values
    velocityX = 30;
    sound.setPosition(-10, 3, 0);
    sound.setVelocity(velocityX, 0, 0);
    sound.setLoop(true);
    sound.play();

    // Move the sound in the sam direction as the velocity
    for (x = -10; x < 10; x += 0.25 / RESOLUTION)
    {
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);
    }

    // Slow the sound down
    // NOTE: For the sake of simplicity, the position is not updated with
    //       complete accuracy, in terms of the changing velocity
    for (; x < 17.5; x += 0.25 / RESOLUTION)
    {
        velocityX -= 1.0 / RESOLUTION;
        sound.setVelocity(velocityX, 0, 0);
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);
    }

    // Move the sound back in the other direciton
    // NOTE: For the sake of simplicity, the position is not updated with
    //       complete accuracy, in terms of the changing velocity
    for (; x > 10; x -= 0.25 / RESOLUTION)
    {
        velocityX -= 1.0 / RESOLUTION;
        sound.setVelocity(velocityX, 0, 0);
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);
    }

    // Once the velocity has reached desired value, keep updating the position
    for (; x > -10; x -= 0.25 / RESOLUTION)
    {
        sound.setPosition(x, 3, 0);
        usleep(150000 / RESOLUTION);
    }

    // End the example
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

