#include "delay.h"
#include "definitions.h"      // Brings in CMSIS (CoreDebug/DWT) via Harmony

#ifndef __NOP
  #define __NOP() __asm__ __volatile__("nop")
#endif

/* Internal state */
static uint32_t s_cpu_hz = 150000000u;   // Default placeholder; set in delay_init().
static bool     s_dwt_ok  = false;

/* Enable and sanity-check DWT CYCCNT */
static bool dwt_enable(void)
{
    /* Enable trace subsystem */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

#ifdef DWT_CTRL_NOCYCCNT_Msk
    /* Some MCUs may not implement CYCCNT; bail if reported unsupported */
    if (DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) {
        return false;
    }
#endif

    /* Clear and enable cycle counter */
    DWT->CYCCNT = 0u;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

    /* Self-test: confirm the counter is incrementing */
    uint32_t before = DWT->CYCCNT;
    __NOP(); __NOP(); __NOP(); __NOP();
    uint32_t after  = DWT->CYCCNT;

    return (after != before);
}

void delay_init(uint32_t cpu_hz)
{
#if defined(SystemCoreClock)
    /* If cpu_hz==0, prefer CMSIS SystemCoreClock */
    s_cpu_hz = (cpu_hz != 0u) ? cpu_hz : SystemCoreClock;
#else
    s_cpu_hz = (cpu_hz != 0u) ? cpu_hz : s_cpu_hz;
#endif

    /* Try to enable DWT-based timing */
    s_dwt_ok = dwt_enable();
}

bool delay_is_dwt_available(void)
{
    return s_dwt_ok;
}

void delay_cycles(uint32_t cycles)
{
    if (cycles == 0u) return;

    if (s_dwt_ok) {
        /* 32-bit wrap-safe comparison */
        uint32_t start = DWT->CYCCNT;
        while ((uint32_t)(DWT->CYCCNT - start) < cycles) {
            /* busy wait */
        }
    } 
    else {
        /* Fallback: approximate NOP loop (lower accuracy) */
        while (cycles--) {
            __NOP();
        }
    }
}

void delay_us(uint32_t us)
{
    if (us == 0u) return;

    /* Chunk to avoid large 64-bit multiplies and overflow.
       500,000 us (0.5 s) per chunk is a safe bound. */
    const uint32_t CHUNK_US = 500000u;
    while (us) {
        uint32_t step = (us > CHUNK_US) ? CHUNK_US : us;
        uint64_t cyc64 = ((uint64_t)s_cpu_hz * (uint64_t)step) / 1000000ULL;
        delay_cycles((uint32_t)cyc64);
        us -= step;
    }
}

void delay_ms(uint32_t ms)
{
    /* Implement in terms of microseconds to reuse logic and preserve accuracy */
    while (ms--) {
        delay_us(1000u);
    }
}
