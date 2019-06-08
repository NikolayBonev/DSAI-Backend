#include "DSAISerialListener.h"

DSAISerialListener::DSAISerialListener(std::string ttyDevice, int serialSpeed)
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
	
	return read_buf;
}

void DSAISerialListener::Cleanup()
{
	close(m_ttyFileDescriptor);
}

void DSAISerialListener::CreateTTYDeviceFileDescriptor()
{
	m_ttyFileDescriptor = open( m_strTTYAddress, O_RDWR | O_NDELAY );
	
	if (m_ttyFileDescriptor < 0) {
		printf("Error %i from open: %s\n", errno, strerror(errno));
	}
}

void DSAISerialListener::ConfigureTTYDevice()
{
	// Save old tty parameters
	tty_old = tty;
	
	memset (&tty, 0, sizeof tty);
	
	if(tcgetattr(m_ttyFileDescriptor, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	
	/*  Disable parity bit.
	 *	Use |= and remove tilde if using parity bit.
	 */
	tty.c_cflag &= ~PARENB;
	
	/*	Use only one stop bit.
	 *	Use |= and remove tilde if using two stop bits.
	 */
	tty.c_cflag &= ~CSTOPB;
	
	/*	Set number of bits fot byte to be 8 bits.
	 *	Othes options: CS5, CS6, CS7.
	 */
	tty.c_cflag |= CS8;
	
	/*	Disable RTS/CTS hardware flow controll
	 *	Use |= and remove tilde to enable.
	 */
	tty.c_cflag &= ~CRTSCTS;

	/* 
	 *	Turn on READ & ignore ctrl lines (CLOCAL = 1)
	 */
	tty.c_cflag |= CREAD | CLOCAL;
	
	/*	Enabling canonical mode. Input is processed when new line char is received.
	 *	Use &= and tilde to disable.
	 */
	tty.c_lflag = ICANON;
	
	tty.c_iflag = IGNPAR | ICRNL;
	
	tty.c_oflag = 0;	

	/*	Start read when there are minimum 138 symbols
	 *	or 5 decisecond passed.
	 */
	tty.c_cc[VMIN] = MIN_INPUT_SIZE;
	tty.c_cc[VTIME] = TIME_INTERVAL_TO_READ;
	
	// Set Band Rate
	cfsetospeed (&tty, BAUDRATE);
	cfsetispeed (&tty, BAUDRATE);
	
	if (tcsetattr(m_ttyFileDescriptor, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
	
	tcflush(m_ttyFileDescriptor, TCIFLUSH);
}