#include <util.h>


#define CC1101_SRES			0x30	// Reset chip.
#define CC1101_SFSTXON		0x31	// Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA):
//Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).

#define CC1101_SXOFF		0x32	// Turn off crystal oscillator.
#define CC1101_SCAL			0x33	// Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without
//setting manual calibration mode (MCSM0.FS_AUTOCAL=0)

#define CC1101_SRX		    0x34	// Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1.
#define CC1101_STX			0x35	// In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
//If in RX state and CCA is enabled: Only go to TX if channel is clear.

#define CC1101_SIDLE		0x36	// Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable.
#define CC1101_SWOR			0x38	// Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if
//WORCTRL.RC_PD=0.

#define CC1101_SPWD			0x39	// Enter power down mode when CSn goes high.
#define CC1101_SFRX			0x3A	// Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states.
#define CC1101_SFTX			0x3B	// Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states.
#define CC1101_SWORRST		0x3C	// Reset real time clock to Event1 value.
#define CC1101_SNOP			0x3D	// No operation. May be used to get access to the chip status byte.



#define RADIO_CC1101
#define CC1101_IOCFG2           0x00    //GDO2 OUTPUT PIN CONFIGURATION
#define CC1101_IOCFG1           0x01    //GDO1 OUTPUT PIN CONFIGURATION
#define CC1101_IOCFG0           0x02    //GDO0 OUTPUT PIN CONFIGURATION
#define CC1101_FIFOTHR          0x03    //RX FIFO AND TX FIFO THRESHOLDS
#define CC1101_SYNC1            0x04    //SYNC WORD, HIGH BYTE
#define CC1101_SYNC0            0x05    //SYNC WORD, LOW BYTE
#define CC1101_PKTLEN           0x06    //PACKET LENGTH
#define CC1101_PKTCTRL1         0x07    //PACKET AUTOMATION CONTROL
#define CC1101_PKTCTRL0         0x08    //PACKET AUTOMATION CONTROL
#define CC1101_ADDR             0x09    //DEVICE ADDRESS
#define CC1101_CHANNR           0x0A    //CHANNEL NUMBER
#define CC1101_FSCTRL1          0x0B    //FREQUENCY SYNTHESIZER CONTROL
#define CC1101_FSCTRL0          0x0C    //FREQUENCY SYNTHESIZER CONTROL
#define CC1101_FREQ2            0x0D    //FREQUENCY CONTROL WORD, HIGH BYTE
#define CC1101_FREQ1            0x0E    //FREQUENCY CONTROL WORD, MIDDLE BYTE
#define CC1101_FREQ0            0x0F    //FREQUENCY CONTROL WORD, LOW BYTE
#define CC1101_MDMCFG4          0x10    //MODEM CONFIGURATION
#define CC1101_MDMCFG3          0x11    //MODEM CONFIGURATION
#define CC1101_MDMCFG2          0x12    //MODEM CONFIGURATION
#define CC1101_MDMCFG1          0x13    //MODEM CONFIGURATION
#define CC1101_MDMCFG0          0x14    //MODEM CONFIGURATION
#define CC1101_DEVIATN          0x15    //MODEM DEVIATION SETTING
#define CC1101_MCSM2            0x16    //MAIN RADIO CONTROL STATE MACHINE CONFIGURATION
#define CC1101_MCSM1            0x17    //MAIN RADIO CONTROL STATE MACHINE CONFIGURATION
#define CC1101_MCSM0            0x18    //MAIN RADIO CONTROL STATE MACHINE CONFIGURATION
#define CC1101_FOCCFG           0x19    //FREQUENCY OFFSET COMPENSATION CONFIGURATION
#define CC1101_BSCFG            0x1A    //BIT SYNCHRONIZATION CONFIGURATION
#define CC1101_AGCCTRL2         0x1B    //AGC CONTROL
#define CC1101_AGCCTRL1         0x1C    //AGC CONTROL
#define CC1101_AGCCTRL0         0x1D    //AGC CONTROL
#define CC1101_WOREVT1          0x1E    //HIGH BYTE EVENT0 TIMEOUT
#define CC1101_WOREVT0          0x1F    //LOW BYTE EVENT0 TIMEOUT
#define CC1101_WORCTRL          0x20    //WAKE ON RADIO CONTROL
#define CC1101_FREND1           0x21    //FRONT END RX CONFIGURATION
#define CC1101_FREND0           0x22    //FRONT END TX CONFIGURATION
#define CC1101_FSCAL3           0x23    //FREQUENCY SYNTHESIZER CALIBRATION
#define CC1101_FSCAL2           0x24    //FREQUENCY SYNTHESIZER CALIBRATION
#define CC1101_FSCAL1           0x25    //FREQUENCY SYNTHESIZER CALIBRATION
#define CC1101_FSCAL0           0x26    //FREQUENCY SYNTHESIZER CALIBRATION
#define CC1101_RCCTRL1          0x27    //RC OSCILLATOR CONFIGURATION
#define CC1101_RCCTRL0          0x28    //RC OSCILLATOR CONFIGURATION
#define CC1101_FSTEST           0x29    //FREQUENCY SYNTHESIZER CALIBRATION CONTROL
#define CC1101_PTEST            0x2A    //PRODUCTION TEST
#define CC1101_AGCTEST          0x2B    //AGC TEST
#define CC1101_TEST2            0x2C    //VARIOUS TEST SETTINGS
#define CC1101_TEST1            0x2D    //VARIOUS TEST SETTINGS
#define CC1101_TEST0            0x2E    //VARIOUS TEST SETTINGS
#define CC1101_PARTNUM          0x30    //CHIP ID
#define CC1101_VERSION          0x31    //CHIP ID
#define CC1101_FREQEST          0x32    //FREQUENCY OFFSET ESTIMATE FROM DEMODULATOR
#define CC1101_LQI              0x33    //DEMODULATOR ESTIMATE FOR LINK QUALITY
#define CC1101_RSSI             0x34    //RECEIVED SIGNAL STRENGTH INDICATION
#define CC1101_MARCSTATE        0x35    //MAIN RADIO CONTROL STATE MACHINE STATE
#define CC1101_WORTIME1         0x36    //HIGH BYTE OF WOR TIME
#define CC1101_WORTIME0         0x37    //LOW BYTE OF WOR TIME
#define CC1101_PKTSTATUS        0x38    //CURRENT GDOX STATUS AND PACKET STATUS
#define CC1101_VCO_VC_DAC       0x39    //CURRENT SETTING FROM PLL CALIBRATION MODULE
#define CC1101_TXBYTES          0x3A    //UNDERFLOW AND NUMBER OF BYTES
#define CC1101_RXBYTES          0x3B    //OVERFLOW AND NUMBER OF BYTES
#define CC1101_RCCTRL1_STATUS   0x3C    //LAST RC OSCILLATOR CALIBRATION RESULT
#define CC1101_RCCTRL0_STATUS   0x3D    //LAST RC OSCILLATOR CALIBRATION RESULT


/* Address Config = No address check */
/* Base Frequency = 434.249908 */
/* CRC Autoflush = false */
/* CRC Enable = true */
/* Carrier Frequency = 434.249908 */
/* Channel Number = 0 */
/* Channel Spacing = 199.951172 */
/* Data Format = Normal mode */
/* Data Rate = 1.19948 */
/* Deviation = 5.157471 */
/* Device Address = 0 */
/* Manchester Enable = false */
/* Modulated = true */
/* Modulation Format = GFSK */
/* PA Ramping = false */
/* Packet Length = 255 */
/* Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word */
/* Preamble Count = 4 */
/* RX Filter BW = 58.035714 */
/* Sync Word Qualifier Mode = 30/32 sync word bits detected */
/* TX Power = 0 */
/* Whitening = false */
#define RF_IOCFG2           0x29
#define RF_IOCFG1           0x2E
#define RF_IOCFG0           0x02
#define RF_FIFOTHR          0x4F
#define RF_SYNC1            0xD3
#define RF_SYNC0            0x91
#define RF_PKTLEN           0xFF
#define RF_PKTCTRL1         0x04
#define RF_PKTCTRL0         0x05
#define RF_ADDR             0x00
#define RF_CHANNR           0x00
#define RF_FSCTRL1          0x06
#define RF_FSCTRL0          0x00
#define RF_FREQ2            0x10
#define RF_FREQ1            0xB3
#define RF_FREQ0            0x72
#define RF_MDMCFG4          0xF5
#define RF_MDMCFG3          0x83
#define RF_MDMCFG2          0x13
#define RF_MDMCFG1          0x22
#define RF_MDMCFG0          0xF8
#define RF_DEVIATN          0x15
#define RF_MCSM2            0x07
#define RF_MCSM1            0x32
#define RF_MCSM0            0x18
#define RF_FOCCFG           0x16
#define RF_BSCFG            0x6C
#define RF_AGCCTRL2         0x03
#define RF_AGCCTRL1         0x40
#define RF_AGCCTRL0         0x91
#define RF_WOREVT1          0x87
#define RF_WOREVT0          0x6B
#define RF_WORCTRL          0xFB
#define RF_FREND1           0x56
#define RF_FREND0           0x10
#define RF_FSCAL3           0xE9
#define RF_FSCAL2           0x2A
#define RF_FSCAL1           0x00
#define RF_FSCAL0           0x1F
#define RF_RCCTRL1          0x41
#define RF_RCCTRL0          0x00
#define RF_FSTEST           0x59
#define RF_PTEST            0x7F
#define RF_AGCTEST          0x3F
#define RF_TEST2            0x81
#define RF_TEST1            0x35
#define RF_TEST0            0x09
#define RF_PARTNUM          0x00
#define RF_VERSION          0x04
#define RF_FREQEST          0x00
#define RF_LQI              0x00
#define RF_RSSI             0x00
#define RF_MARCSTATE        0x00
#define RF_WORTIME1         0x00
#define RF_WORTIME0         0x00
#define RF_PKTSTATUS        0x00
#define RF_VCO_VC_DAC       0x00
#define RF_TXBYTES          0x00
#define RF_RXBYTES          0x00
#define RF_RCCTRL1_STATUS   0x00
#define RF_RCCTRL0_STATUS   0x00



void sendreg();
u8_t cc1101_get_status();
void write_cc1101_status_regersters();

