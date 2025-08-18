#include "rb.h"
#include <string.h>

size_t rb_count(const RingBuffer *rb)
{
    size_t h = rb->head, t = rb->tail, s = rb->size;
    return (h >= t) ? (h - t) : (s - (t - h));
}

size_t rb_free(const RingBuffer *rb)
{
    return (rb->size - 1U) - rb_count(rb);
}

size_t rb_write_overwrite(RingBuffer *rb, const uint8_t *src, size_t n)
{
    if (n == 0U) return 0U;

    RB_ENTER_CRIT();
    while (rb_free(rb) < n) {
        rb->tail = (rb->tail + 1U) % rb->size;   // ??? ??? ??
    }

    size_t first = rb->size - rb->head;
    if (first > n) first = n;

    memcpy(&rb->data[rb->head], src, first);
    memcpy(&rb->data[0],       src + first, n - first);

    rb->head = (rb->head + n) % rb->size;
    RB_EXIT_CRIT();

    return n;
}

size_t rb_read(RingBuffer *rb, uint8_t *dst, size_t n)
{
    if (n == 0U) return 0U;

    RB_ENTER_CRIT();
    size_t avail = rb_count(rb);
    if (n > avail) n = avail;

    size_t first = rb->size - rb->tail;
    if (first > n) first = n;

    memcpy(dst,              &rb->data[rb->tail], first);
    memcpy(dst + first,      &rb->data[0],        n - first);

    rb->tail = (rb->tail + n) % rb->size;
    RB_EXIT_CRIT();

    return n;
}
