#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define FASTLED_INTERNAL // hide all the FastLED SPI and bit bang spam
// #define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define FASTLED_ESP32_I2S
// More headroom for the refill interrupt to be late before the 3-wire strips latch stale data, per FastLED's I2S driver docs
#define FASTLED_ESP32_I2S_NUM_DMA_BUFFERS 4
// #define MONITOR_TASK_STACK_SIZES

#endif
