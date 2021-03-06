
/*
 * Peripheral devices
 */

#include <unistd.h>

/* FDC registers */
enum FLPYDSK_IO
{
	STATUS_REGISTER_A		= 0x3F0, // read-only
	STATUS_REGISTER_B		= 0x3F1, // read-only
	FLPYDSK_DOR          		= 0x3F2,
	TAPE_DRIVE_REGISTER		= 0x3F3,
	FLPYDSK_MSR			= 0x3F4, // read-only
	DATARATE_SELECT_REGISTER	= 0x3F4, // write-only
	DATA_FIFO			= 0x3F5,
	DIGITAL_INPUT_REGISTER		= 0x3F7, // read-only
	FLPYDSK_CTRL			= 0x3F7  // write-only
};

enum FLPYDSK_CMD
{
	FDC_CMD_READ_TRACK	=	2, // generates IRQ6
	FDC_CMD_SPECIFY		=	3, // * set drive parameters
	FDC_CMD_CHECK_STAT	=	4,
	FDC_CMD_WRITE_SECT	=	5, // * write to the disk
	FDC_CMD_READ_SECT	=	6, // * read from the disk
	FDC_CMD_CALIBRATE	=	7, // * seek to cylinder 0
	FDC_CMD_CHECK_INT	=	8, // * ack IRQ6, get status of last command
	FDC_CMD_WRITE_DEL_S	=	9,
	FDC_CMD_READ_ID_S	=	10, // generates IRQ6
	FDC_CMD_READ_DEL_S	=	0xc,
	FDC_CMD_FORMAT_TRACK	=	0xd,
	FDC_CMD_DUMPREG 	=	14,
	FDC_CMD_SEEK		=	0xf,
	VERSION 		=	16, // * used during initialization, once
	SCAN_EQUAL 		=	17,
	PERPENDICULAR_MODE 	=	18, // * used during initialization, once, maybe
	CONFIGURE 		=	19, // * set controller parameters
	LOCK			=	20, // * protect controller params from a reset
	VERIFY			=	22,
	SCAN_LOW_OR_EQUAL 	=	25,
	SCAN_HIGH_OR_EQUAL	=	29
};

enum FLPYDSK_MSR_MASK
{
	FLPYDSK_MSR_MASK_DRIVE1_POS_MODE	=	1,	//00000001
	FLPYDSK_MSR_MASK_DRIVE2_POS_MODE	=	2,	//00000010
	FLPYDSK_MSR_MASK_DRIVE3_POS_MODE	=	4,	//00000100
	FLPYDSK_MSR_MASK_DRIVE4_POS_MODE	=	8,	//00001000
	FLPYDSK_MSR_MASK_BUSY			=	16,	//00010000
	FLPYDSK_MSR_MASK_DMA			=	32,	//00100000
	FLPYDSK_MSR_MASK_DATAIO			=	64, 	//01000000
	FLPYDSK_MSR_MASK_DATAREG		=	128	//10000000
};

enum FLPYDSK_CMD_EXT
{
	FDC_CMD_EXT_SKIP	=	0x20,	//00100000
	FDC_CMD_EXT_DENSITY	=	0x40,	//01000000
	FDC_CMD_EXT_MULTITRACK	=	0x80	//10000000
};

enum FLPYDSK_GAP3_LENGTH
{
	FLPYDSK_GAP3_LENGTH_STD		= 42,
	FLPYDSK_GAP3_LENGTH_5_14	= 32,
	FLPYDSK_GAP3_LENGTH_3_5		= 27
};

enum FLPYDSK_SECTOR_DTL
{ 
	FLPYDSK_SECTOR_DTL_128	=	0,
	FLPYDSK_SECTOR_DTL_256	=	1,
	FLPYDSK_SECTOR_DTL_512	=	2,
	FLPYDSK_SECTOR_DTL_1024	=	4
};

/* for floopy */
void reset_floppy(int device);
void floppy_init();

/* for hard disk */
void hd_init();

