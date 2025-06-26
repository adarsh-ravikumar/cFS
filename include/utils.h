#ifndef UTILS_H
#define UTILS_H

#define KB(x) ((x) * 1024UL)
#define MB(x) ((x) * 1024UL * 1024UL)
#define GB(x) ((x) * 1024UL * 1024UL * 1024UL)

#define SET_BIT(x, n) ((x) |= (1U << (n)))
#define CLEAR_BIT(x, n) ((x) &= ~(1U << (n)))
#define TOGGLE_BIT(x, n) ((x) ^= (1U << (n)))
#define IS_BIT_SET(x, n) (((x) >> (n)) & 1U)

int ask_yes_no(const char *question);

#endif