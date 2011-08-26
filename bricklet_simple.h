/* brickletlib
 * Copyright (C) 2010 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_simple.h: Functionality for simple sensor bricklets
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef BRICKLET_SIMPLE_H
#define BRICKLET_SIMPLE_H

#ifdef BRICKLET_HAS_SIMPLE_SENSOR
#include <stdint.h>

#include "config.h"

#define SIMPLE_TRANSFER_VALUE 0
#define SIMPLE_TRANSFER_PERIOD 1
#define SIMPLE_TRANSFER_THRESHOLD 2
#define SIMPLE_TRANSFER_DEBOUNCE 3

#define SIMPLE_DIRECTION_GET 0
#define SIMPLE_DIRECTION_SET 1

#define SIMPLE_SIGNEDNESS_UINT 0
#define SIMPLE_SIGNEDNESS_INT  1

typedef struct {
	uint8_t unit;
	uint8_t transfer;
	uint8_t direction;
} SimpleMessageProperty;

typedef struct {
	value_handler_func_t func;
	uint8_t signedness;
	uint8_t type_period;
	uint8_t type_reached;
	uint8_t call_value_id;
} SimpleUnitProperty;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint16_t value;
} __attribute__((__packed__)) SimpleStandardMessage;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) SimpleGetValue;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	int16_t value;
} __attribute__((__packed__)) SimpleGetValueReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t debounce;
} __attribute__((__packed__)) SimpleSetDebounce;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) SimpleGetDebounce;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t debounce;
} __attribute__((__packed__)) SimpleGetDebounceReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t period;
} __attribute__((__packed__)) SimpleSetPeriod;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) SimpleGetPeriod;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t period;
} __attribute__((__packed__)) SimpleGetPeriodReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char option;
	int16_t min;
	int16_t max;
} __attribute__((__packed__)) SimpleSetThreshold;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) SimpleGetThreshold;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char option;
	int16_t min;
	int16_t max;
} __attribute__((__packed__)) SimpleGetThresholdReturn;

void simple_invocation(uint8_t com, uint8_t *data);
void simple_constructor(void);
void simple_destructor(void);
void simple_tick(void);
#endif

#endif