#include "DSAISerialListener.h"

DSAISerialListener::DSAISerialListener(const std::string &ttyDevice, int serialSpeed)
	: m_strTTYAddress(ttyDevice),
	  m_serialSpeed(serialSpeed)
{
}

DSAISerialListener::~DSAISerialListener()
{
	Cleanup();
}

bool DSAISerialListener::Init()
{
	CreateTTYDeviceFileDescriptor();
	
	if (m_ttyFileDescriptor < 0)
	{
		return false;
	}
	
	return true;
}

std::string DSAISerialListener::Read()
{
	char read_buf [TTY_READ_BUFFER];
	memset(&read_buf, '\0', sizeof(read_buf));
	int n = read(m_ttyFileDescriptor, &read_buf, sizeof(read_buf));
	
	if(n < 0)
	{
		printf("Error %i from: %s\n", errno, strerror(errno));
	}
	
	return read_buf;
}

void DSAISerialListener::Cleanup()
{
	close(m_ttyFileDescriptor);
}

void DSAISerialListener::CreateTTYDeviceFileDescriptor()
{
	m_ttyFileDescriptor = serialOpen(m_strTTYAddress.c_str(), m_serialSpeed);
	
	if (m_ttyFileDescriptor < 0) {
		printf("Error %i from open: %s\n", errno, strerror(errno));
	}
	
	ConfigureTTYDevice();
}

void DSAISerialListener::ConfigureTTYDevice()
{
	// Save old tty parameters
	tty_old = tty;
	
	memset (&tty, 0, sizeof tty);
	
	if(tcgetattr(m_ttyFileDescriptor, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	
	tty.c_cflag &= ~CSIZE;
	
	/*  Enable parity bit.
	 *	Use &= and tilde if not using parity bit.
	 */
	tty.c_cflag |= PARENB;
	
	/*	Set number of bits fot byte to be 7 bits.
	 *	Othes options: CS5, CS6, CS8.
	 */
	tty.c_cflag |= CS7;
	
	/*	Enabling canonical mode. Input is processed when new line char is received.
	 *	Use &= and tilde to disable.
	 */
	tty.c_lflag |= ICANON;
	
	if (tcsetattr(m_ttyFileDescriptor, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
	
	tcflush(m_ttyFileDescriptor, TCIFLUSH);
}