#ifndef DSAISERIALLISTENER_H
#define DSAISERIALLISTENER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string>
#include <functional>

#define MIN_INPUT_SIZE 138
#define TIME_INTERVAL_TO_READ 5
#define BAUDRATE B9600
#define TTY_READ_BUFFER 512

// Forward declaration of class
class DSAISerialListener;

class DSAISerialListener
{
	public:
	// Constructor
	DSAISerialListener(std::string ttyDevice, int serialSpeed);
	
	// Destructor
	~DSAISerialListener();

	// Init file descriptor
	bool Init();
	
	// Reads from tty device
	std::string Read();
	
	// Remove structures and buffers
	void Cleanup();
	
	private:
	// Creates fd for reading tty device
	void CreateTTYDeviceFileDescriptor();
	
	void ConfigureTTYDevice();
	
	// TTY Device address
	std::string m_strTTYAddress;
	
	// Serial speed
	int m_serialSpeed;
	
	// File descriptor for the tty device
	int m_ttyFileDescriptor;
	
	struct termios tty;
	struct termios tty_old;
};


#endif // DSAISERIALLISTENER_H