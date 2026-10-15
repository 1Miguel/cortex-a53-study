#include <stdint.h>

typedef void (*isr_handler_t)(void);

struct _isr_entry {
  uint16_t id;
  uint8_t prio;
  isr_handler_t handler;
};

struct _isr_entry *start;

static inline __irq_init(void) {

}
