#ifndef CONFIG_H
#define CONFIG_H

/* Debug configuration */
#define DEBUG 0

/* Pin configuration */

#define PIN_DCF77SIGNAL 2

#define PIN_LEDFLASH 4
#define PIN_LEDDCFSTATUS 5

#define PIN_HOURMETER 9
#define PIN_MINUTEMETER 10
#define PIN_SECONDMETER 11

/* Serial configuration */
#define SERIAL_BAUD 9600

/* DCF77Driver configuration */
#define DCF_INT 0
#define DCF_REFRESH 50
#define DCF_MAXDESYNCTIME 300000

/* LEDDriver configuration */
#define LED_FLASHCOUNT 3
#define LED_FLASHINTERVAL 500

#endif /* CONFIG_H */