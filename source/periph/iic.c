#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "src/sysctl.h"
#include "src/gpio.h"
#include "src/i2c.h"
#include "src/interrupt.h"
#include "src/pin_map.h"

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

void iic_write_m(unsigned char byte)
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
	SDA_H;
	SDA_IN;
	SCL_H;
	while (SDA_READ) {
		if (i-- == 0)
			break;
	}
	SCL_L;
	SDA_OUT;
}

/* unsigned char Read1byte() */
unsigned char iic_read_m()
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
/* 	SCL_L;
 * 	SDA_H;
 * 	SCL_H;
 */
	SCL_L;
	SDA_L;
	SCL_H;
	SCL_L;
	SDA_H;

	return temp;
}

/* unsigned char Read1byte() */
unsigned int ads_read_count(unsigned int count)
{
	unsigned char i=0;

	unsigned int temp=0;
	SDA_In;
	for(i=0;i<count;i++)
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


IIC_t iic_status;
void iic_handler(void)
{
	/* Clear the I2C interrupt. */
	I2CMasterIntClear(I2C0_MASTER_BASE);

	/* Determine what to do based on the current state */
	switch(iic_status.status) {
		case STATE_IDLE: /* The idle state */
			/* There is nothing to be done */
			break;

		case STATE_WRITE_NEXT: /* The state for the middle of a burst write */
			/* Write the next byte to the data register */
			I2CMasterDataPut(I2C0_MASTER_BASE, *iic_status.pdat++);
			iic_status.count--;
			/* Continue the burst write */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
			/* If there is one byte left, set the next state to the final write
			 * state */
			if (iic_status.count == 1) {
				iic_status.status = STATE_WRITE_FINAL;
			}
			break;

		case STATE_WRITE_FINAL: /* The state for the final write of a burst sequence */
			/* Write the final byte to the data register */
			I2CMasterDataPut(I2C0_MASTER_BASE, *iic_status.pdat++);
			iic_status.count--;
			/* Finish the burst write */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
			/* The next state is to wait for the burst write to complete */
			iic_status.status = STATE_SEND_ACK;
			break;

		case STATE_WAIT_ACK: /* Wait for an ACK on the read after a write */
			/* See if there was an error on the previously issued read */
			if (I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE) {
				/* Read the byte received */
				I2CMasterDataGet(I2C0_MASTER_BASE);
				/* There was no error, so the state machine is now idle */
				iic_status.status = STATE_IDLE;
				break;
			}
			/* Fall through to STATE_SEND_ACK */

		case STATE_SEND_ACK: /* Send a read request, looking for the ACK to indicate
							  * that the write is done */
			/* Put the I2C master into receive mode */
			I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x50, true);
			/* Perform a single byte read */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
			/* The next state is the wait for the ack */
			iic_status.status = STATE_WAIT_ACK;
			break;

		case STATE_READ_ONE: /* The state for a single byte read */
			/* Put the I2C master into receive mode */
			I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x50, true);
			/* Perform a single byte read */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
			/* The next state is the wait for final read state */
			iic_status.status = STATE_READ_WAIT;
			break;

		case STATE_READ_FIRST: /* The state for the start of a burst read */
			/* Put the I2C master into receive mode */
			I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x50, true);
			/* Start the burst receive */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
			/* The next state is the middle of the burst read */
			iic_status.status = STATE_READ_NEXT;
			break;

		case STATE_READ_NEXT: /* The state for the middle of a burst read */
			/* Read the received character */
			*iic_status.pdat++ = I2CMasterDataGet(I2C0_MASTER_BASE);
			iic_status.count--;
			/* Continue the burst read */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			/* If there are two characters left to be read, make the next
			 * state be the end of burst read state */
			if (iic_status.count == 2) {
				iic_status.status = STATE_READ_FINAL;
			}
			break;

		case STATE_READ_FINAL: /* The state for the end of a burst read */
			/* Read the received character */
			*iic_status.pdat++ = I2CMasterDataGet(I2C0_MASTER_BASE);
			iic_status.count--;
			/* Finish the burst read */
			I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
			/* The next state is the wait for final read state */
			iic_status.status = STATE_READ_WAIT;
			break;

		case STATE_READ_WAIT: /* This state is for the final read of a single or burst read */
			/* Read the received character */
			*iic_status.pdat++ = I2CMasterDataGet(I2C0_MASTER_BASE);
			iic_status.count--;
			/* The state machine is now idle */
			iic_status.status = STATE_IDLE;
			break;

		default:break;
	}
}

/* iic_sys_init - initialize the system IIC module
 */
void iic_sys_init(void)
{
	/* Enable IIC module */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	/* Enable IIC GPIO */
	SysCtlPeripheralEnable(I2C0SCL_PERIPH);
	SysCtlPeripheralEnable(I2C0SDA_PERIPH);
	/* Enable IIC pin */
	GPIOPinTypeI2C(I2C0SCL_PORT, I2C0SCL_PIN);
	GPIOPinTypeI2C(I2C0SDA_PORT, I2C0SDA_PIN);
	/* Configure IIC speed 100kbps */
	/* I2CMasterInit(I2C1_MASTER_BASE, false); */
	/* Initialize the I2C master */
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
	/* Register IIC interrupt handler */
	I2CIntRegister(I2C0_MASTER_BASE, iic_handler);
	/* I2CIntRegister(I2C0_SLAVE_BASE, iic_handler); */
	/* Enable IIC interrupt */
	IntEnable(INT_I2C0);
	/* Enable IIC module interrupt */
	I2CMasterIntEnable(I2C0_MASTER_BASE);
}		/* -----  end of function iic_sys_init  ----- */


/* iic_read - read data from IIC bus
 */
void iic_read(unsigned long address, unsigned char *dat, unsigned long count)
{
	/* Configure iic status structure */
	iic_status.pdat = dat;
	iic_status.count = count;
	if (count != 1) {
		iic_status.status = STATE_READ_ONE;
	} else {
		iic_status.status = STATE_READ_FIRST;
	}

	/* Set the master address will place on the bus */
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x50 | (address >> 8), false);
	/* Place the address to be written in the data register */
	I2CMasterDataPut(I2C0_MASTER_BASE, address);
	/* Perform a single send, writing the address as the only byte */
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	/* Wait until the I2C interrupt state machine is idle */
	while(iic_status.status != STATE_IDLE) {
	};
}		/* -----  end of function iic_read  ----- */

/* iic_write - write data to IIC bus
 */
void iic_write(unsigned long address, unsigned char *dat, unsigned long count)
{
	/* Configure iic status structure */
	iic_status.pdat = dat;
	iic_status.count = count;
	if (count != 1) {
		iic_status.status = STATE_WRITE_NEXT;
	} else {
		iic_status.status = STATE_WRITE_FINAL;
	}
	/* Set the slave address will place on the bus */
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x50 | (address >> 8), false);
	/* Place the address to be written in the data register */
	I2CMasterDataPut(I2C0_MASTER_BASE, address);
	/* Start the burst cycle, writing the address as the first byte */
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	/* Wait until the I2C interrupt state machine is idle. */
	while(iic_status.status != STATE_IDLE) {
	}
}		/* -----  end of function iic_write  ----- */

