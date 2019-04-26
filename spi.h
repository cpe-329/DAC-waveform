#ifndef __SPI_H__
#define __SPI_H__

#include "msp.h"
#include "my_msp.h"

#define SPI_B0_CLK_PIN  P1_5
#define SPI_B0_MOSI_PIN P1_6
#define SPI_B0_MISO_PIN P1_7

#define SPI_CS_PIN P3_0

#define DAC_CMD       (0x8000)
#define DAC_BUF_VREF  (0x4000)    
#define DAC_GAIN_1X   (0x2000)
#define DAC_OUT_EN    (0x1000)
#define DAC_DATA_MASK (0x0FFF)
 
inline int dac_cmd(const int data);
inline void cs_low();
inline void cs_high();

#endif
