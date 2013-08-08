#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "src/sysctl.h"
#include "src/gpio.h"
#include "src/i2c.h"
#include "src/interrupt.h"

#include "periph/iic.h"

unsigned int Initdata[4]={0};

void start()
{       
	SDA_H;
	SCL_H;
	SDA_L;
	SCL_L;
}
void stop()
{       
	SDA_L;
	SCL_H;
	SDA_H;
	SCL_L;
}

void Send1byte(unsigned int byte)
{
	unsigned char cByte = byte;
	unsigned char i=0;

	for(i=0;i<8;i++)
	{
		if(cByte&0x80)
			SDA_H;
		else
			SDA_L;
		cByte<<=1;
		SCL_H;
		SCL_L;
	}
	/* SDA_IN; */
	SDA_H;
	SCL_H;
	SCL_L;
	SDA_OUT;
}

/* unsigned char Read1byte() */
unsigned char ADSGetByte()
{
	unsigned char i=0;

	unsigned int temp=0;
	SDA_In;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		SCL_L;
		SCL_H;
		if(SDA_READ)
		{
			temp|=0x01;
		}

	}
	SDA_Out;
	SCL_L;
	SDA_L;
	SCL_H;
	SCL_L;
	SDA_H;
	return temp;
}


void startB()
{       
	SDA_B_H;
	SCL_B_H;
	SDA_B_L;
	SCL_B_L;
}
void stopB()
{       
	SDA_B_L;
	SCL_B_H;
	SDA_B_H;
	SCL_B_L;
}
void Send1byteB(unsigned int byte)
{
	unsigned char cByte = byte;
	unsigned char i=0;

	for(i=0;i<8;i++)
	{
		if(cByte&0x80)
			SDA_B_H;
		else
			SDA_B_L;
		cByte<<=1;
		SCL_B_H;
		SCL_B_L;
	}
	/* SDA_B_IN; */
	SDA_B_H;
	SCL_B_H;
	SCL_B_L;
	SDA_B_OUT;
}
/* unsigned char Read1byte() */
unsigned char ADSGetByteB()
{
	unsigned char i=0;

	unsigned int temp=0;
	SDA_B_In;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		SCL_B_L;
		SCL_B_H;
		if(SDA_B_READ)
		{
			temp|=0x01;
		}

	}
	SDA_B_Out;
	SCL_B_L;
	SDA_B_L;
	SCL_B_H;
	SCL_B_L;
	SDA_B_H;
	return temp;
}



/* init_iic() - initialize IIC module
*/
void SYS_init_iic(void)
{
	/* enable IIC GPIO */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	/* enable IIC module */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	/* enable IIC pin */
	GPIOPinTypeI2C(GPIO_PORTA_BASE, I2C_SCL|I2C_SDA);
	/* configure IIC speed 100kbps */
	I2CMasterInit(I2C1_MASTER_BASE, false);
	/* enable master interrupt */
	IntMasterEnable();
	/* enable IIC interrupt */
	IntEnable(INT_I2C1);
	/* enable IIC module interrupt */
	I2CMasterIntEnable(I2C1_MASTER_BASE);
}

#if 1

/*
 *
 * The states in the interrupt handler state machine.
 */
#define STATE_IDLE         0
#define STATE_WRITE_NEXT   1
#define STATE_WRITE_FINAL  2
#define STATE_WAIT_ACK     3
#define STATE_SEND_ACK     4
#define STATE_READ_ONE     5
#define STATE_READ_FIRST   6
#define STATE_READ_NEXT    7
#define STATE_READ_FINAL   8
#define STATE_READ_WAIT    9

/*
 * The variables that track the data to be transmitted or received.
 */
static unsigned char *g_pucData = 0;
static unsigned long g_ulCount = 0;

static volatile unsigned long g_ulState = STATE_IDLE;

//*****************************************************************************
//
// The I2C interrupt handler.
//
//*****************************************************************************
void
I2CIntHandler(void)
{
    //
    // Clear the I2C interrupt.
    //
    I2CMasterIntClear(I2C1_MASTER_BASE);

    //
    // Determine what to do based on the current state.
    //
    switch(g_ulState)
    {
        //
        // The idle state.
        //
        case STATE_IDLE:
        {
            //
            // There is nothing to be done.
            //
            break;
        }

        //
        // The state for the middle of a burst write.
        //
        case STATE_WRITE_NEXT:
        {
            //
            // Write the next byte to the data register.
            //
            I2CMasterDataPut(I2C1_MASTER_BASE, *g_pucData++);
            g_ulCount--;

            //
            // Continue the burst write.
            //
            I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            //
            // If there is one byte left, set the next state to the final write
            // state.
            //
            if(g_ulCount == 1)
            {
                g_ulState = STATE_WRITE_FINAL;
            }

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the final write of a burst sequence.
        //
        case STATE_WRITE_FINAL:
        {
            //
            // Write the final byte to the data register.
            //
            I2CMasterDataPut(I2C1_MASTER_BASE, *g_pucData++);
            g_ulCount--;

            //
            // Finish the burst write.
            //
            I2CMasterControl(I2C1_MASTER_BASE,
                             I2C_MASTER_CMD_BURST_SEND_FINISH);

            //
            // The next state is to wait for the burst write to complete.
            //
            g_ulState = STATE_SEND_ACK;

            //
            // This state is done.
            //
            break;
        }

        //
        // Wait for an ACK on the read after a write.
        //
        case STATE_WAIT_ACK:
        {
            //
            // See if there was an error on the previously issued read.
            //
            if(I2CMasterErr(I2C1_MASTER_BASE) == I2C_MASTER_ERR_NONE)
            {
                //
                // Read the byte received.
                //
                I2CMasterDataGet(I2C1_MASTER_BASE);

                //
                // There was no error, so the state machine is now idle.
                //
                g_ulState = STATE_IDLE;

                //
                // This state is done.
                //
                break;
            }

            //
            // Fall through to STATE_SEND_ACK.
            //
        }

        //
        // Send a read request, looking for the ACK to indicate that the write
        // is done.
        //
        case STATE_SEND_ACK:
        {
            //
            // Put the I2C master into receive mode.
            //
            I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x50, true);

            //
            // Perform a single byte read.
            //
            I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

            //
            // The next state is the wait for the ack.
            //
            g_ulState = STATE_WAIT_ACK;

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for a single byte read.
        //
        case STATE_READ_ONE:
        {
            //
            // Put the I2C master into receive mode.
            //
            I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x50, true);

            //
            // Perform a single byte read.
            //
            I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

            //
            // The next state is the wait for final read state.
            //
            g_ulState = STATE_READ_WAIT;

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the start of a burst read.
        //
        case STATE_READ_FIRST:
        {
            //
            // Put the I2C master into receive mode.
            //
            I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x50, true);

            //
            // Start the burst receive.
            //
            I2CMasterControl(I2C1_MASTER_BASE,
                             I2C_MASTER_CMD_BURST_RECEIVE_START);

            //
            // The next state is the middle of the burst read.
            //
            g_ulState = STATE_READ_NEXT;

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the middle of a burst read.
        //
        case STATE_READ_NEXT:
        {
            //
            // Read the received character.
            //
            *g_pucData++ = I2CMasterDataGet(I2C1_MASTER_BASE);
            g_ulCount--;

            //
            // Continue the burst read.
            //
            I2CMasterControl(I2C1_MASTER_BASE,
                             I2C_MASTER_CMD_BURST_RECEIVE_CONT);

            //
            // If there are two characters left to be read, make the next
            // state be the end of burst read state.
            //
            if(g_ulCount == 2)
            {
                g_ulState = STATE_READ_FINAL;
            }

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the end of a burst read.
        //
        case STATE_READ_FINAL:
        {
            //
            // Read the received character.
            //
            *g_pucData++ = I2CMasterDataGet(I2C1_MASTER_BASE);
            g_ulCount--;

            //
            // Finish the burst read.
            //
            I2CMasterControl(I2C1_MASTER_BASE,
                             I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            //
            // The next state is the wait for final read state.
            //
            g_ulState = STATE_READ_WAIT;

            //
            // This state is done.
            //
            break;
        }

        //
        // This state is for the final read of a single or burst read.
        //
        case STATE_READ_WAIT:
        {
            //
            // Read the received character.
            //
            *g_pucData++  = I2CMasterDataGet(I2C1_MASTER_BASE);
            g_ulCount--;

            //
            // The state machine is now idle.
            //
            g_ulState = STATE_IDLE;

            //
            // This state is done.
            //
            break;
        }
    }
}


/*
 *
 * Write to the Atmel device.
*/

void IIC_write(unsigned char *pucData, unsigned long ulOffset,
           unsigned long ulCount)
{
    //
    // Save the data buffer to be written.
    //
    g_pucData = pucData;
    g_ulCount = ulCount;

    //
    // Set the next state of the interrupt state machine based on the number of
    // bytes to write.
    //
    if(ulCount != 1)
    {
        g_ulState = STATE_WRITE_NEXT;
    }
    else
    {
        g_ulState = STATE_WRITE_FINAL;
    }

    //
    // Set the slave address and setup for a transmit operation.
    //
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x50 | (ulOffset >> 8), false);

    //
    // Place the address to be written in the data register.
    //
    I2CMasterDataPut(I2C1_MASTER_BASE, ulOffset);

    //
    // Start the burst cycle, writing the address as the first byte.
    //
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //
    // Wait until the I2C interrupt state machine is idle.
    //
    /* while(g_ulState != STATE_IDLE) */
    {
    }
}

/*
 *
 * Read from the Atmel device.
*/
void IIC_read(unsigned char *pucData, unsigned long ulOffset,
          unsigned long ulCount)
{
    //
    // Save the data buffer to be read.
    //
    g_pucData = pucData;
    g_ulCount = ulCount;

    //
    // Set the next state of the interrupt state machine based on the number of
    // bytes to read.
    //
    if(ulCount == 1)
    {
        g_ulState = STATE_READ_ONE;
    }
    else
    {
        g_ulState = STATE_READ_FIRST;
    }

    //
    // Start with a dummy write to get the address set in the EEPROM.
    //
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x50 | (ulOffset >> 8), false);

    //
    // Place the address to be written in the data register.
    //
    I2CMasterDataPut(I2C1_MASTER_BASE, ulOffset);

    //
    // Perform a single send, writing the address as the only byte.
    //
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    //
    // Wait until the I2C interrupt state machine is idle.
    //
    /* while(g_ulState != STATE_IDLE) */
    {
    }
}
#endif
