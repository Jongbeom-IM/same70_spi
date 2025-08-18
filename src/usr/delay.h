#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize delay utilities based on DWT cycle counter.
 * @param cpu_hz CPU clock in Hz. If 0, use SystemCoreClock (if available).
 *
 * Enables the Cortex-M7 DWT CYCCNT. After this call, delay_* APIs provide
 * precise busy-wait delays.
 */
void delay_init(uint32_t cpu_hz);

/** @brief Returns true if DWT CYCCNT is available and enabled. */
bool delay_is_dwt_available(void);

/** @brief Busy-wait for a given number of CPU cycles. */
void delay_cycles(uint32_t cycles);

/** @brief Busy-wait for a given number of microseconds. */
void delay_us(uint32_t us);

/** @brief Busy-wait for a given number of milliseconds. */
void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* DELAY_H */
