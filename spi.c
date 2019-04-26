 #include "spi.h"


inline int dac_cmd(const int data){
    return DAC_CMD | 
           DAC_BUF_VREF |
           DAC_GAIN_1X  |
           DAC_OUT_EN   |
           (DAC_DATA_MASK &
            data);
}

inline void cs_low(){
    P3->OUT &= ~SPI_CS_PIN;
}
inline void cs_high(){
    P3->OUT |= SPI_CS_PIN;
}
