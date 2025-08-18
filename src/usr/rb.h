#ifndef RB_H
#define RB_H

#include <stdint.h>
#include <stddef.h>   // size_t

#ifdef __cplusplus
extern "C" {
#endif

/* ?
   ?) #define RB_ENTER_CRIT()  uint32_t _m=__get_PRIMASK(); __disable_irq()
       #define RB_EXIT_CRIT()   if(!_m) __enable_irq()
*/
#ifndef RB_ENTER_CRIT
#define RB_ENTER_CRIT()  ((void)0)
#define RB_EXIT_CRIT()   ((void)0)
#endif

typedef struct {
    uint8_t *data;     // ??? ???
    size_t   size;     // ? ??(???)
    size_t   head;     // write index
    size_t   tail;     // read index
} RingBuffer;

static inline void rb_init_static(RingBuffer *rb, uint8_t *storage, size_t bytes) {
    rb->data = storage;
    rb->size = bytes;
    rb->head = rb->tail = 0;
}

size_t rb_count(const RingBuffer *rb);
size_t rb_free (const RingBuffer *rb);

size_t rb_write_overwrite(RingBuffer *rb, const uint8_t *src, size_t n);
size_t rb_read(RingBuffer *rb, uint8_t *dst, size_t n);
static inline void rb_clear(RingBuffer *rb) { RB_ENTER_CRIT(); rb->head = rb->tail = 0; RB_EXIT_CRIT(); }

#ifdef __cplusplus
}
#endif
#endif /* RB_H */
