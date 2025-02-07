#pragma once

#include "types.h"

extern uint8_t in8(uint16_t port);

extern void out8(uint16_t port, uint8_t val);
extern void out16(uint16_t port, uint16_t val);

void resize_cursor(uint8_t start, uint8_t end);
void set_cursor_coord(uint8_t row, uint8_t col);
