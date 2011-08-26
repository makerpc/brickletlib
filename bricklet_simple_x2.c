/* brickletlib
 * Copyright (C) 2011 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_simple_x2.c: Functionality for simple sensor bricklets with 2 values
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

#include "bricklet_simple_x2.h"

#ifdef BRICKLET_HAS_SIMPLE_SENSOR_X2

#include <stdint.h>

#include <adc/adc.h>
#include <bricklib/bricklet/bricklet_communication.h>

#include "config.h"

#include "brickletlib/bricklet_debug.h"

extern const SimpleMessageProperty smp[];
extern const SimpleUnitProperty sup[];

void simple_invocation(uint8_t com, uint8_t *data) {
	uint8_t id = ((SimpleStandardMessage*)data)->type - 1;
	if(smp[id].direction == SIMPLE_DIRECTION_SET) {
		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_VALUE: {
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleSetPeriod *ssp = ((SimpleSetPeriod*)data);
				BC->signal_period[smp[id].unit] = ssp->period;

				logbli("set period: %d\n\r", ssp->period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleSetThreshold* sst = (SimpleSetThreshold*)data;
				BC->threshold_option_save[smp[id].unit] = sst->option;
				BC->threshold_min_save1[smp[id].unit] = sst->min1;
				BC->threshold_max_save1[smp[id].unit] = sst->max1;
				BC->threshold_min_save2[smp[id].unit] = sst->min2;
				BC->threshold_max_save2[smp[id].unit] = sst->max2;

				if(sst->option == 'o' ||
				   sst->option == 'i' ||
				   sst->option == 'x') {
					BC->threshold_option[smp[id].unit] = sst->option;
					BC->threshold_min1[smp[id].unit] = sst->min1;
					BC->threshold_max1[smp[id].unit] = sst->max1;
					BC->threshold_min2[smp[id].unit] = sst->min2;
					BC->threshold_max2[smp[id].unit] = sst->max2;
				} else if(sst->option == '<') {
					BC->threshold_option[smp[id].unit] = 'o';
					BC->threshold_min1[smp[id].unit] = sst->min1;
					BC->threshold_max1[smp[id].unit] = 2147483647;
					BC->threshold_min2[smp[id].unit] = sst->min2;
					BC->threshold_max2[smp[id].unit] = 2147483647;
				} else if(sst->option == '>') {
					BC->threshold_option[smp[id].unit] = 'o';
					BC->threshold_min1[smp[id].unit] = -2147483647;
					BC->threshold_max1[smp[id].unit] = sst->min1;
					BC->threshold_min2[smp[id].unit] = -2147483647;
					BC->threshold_max2[smp[id].unit] = sst->min2;
				} else {
					// TODO: Error?
				}

				BC->threshold_period_current[smp[id].unit] = BC->threshold_debounce;

				logbli("set threshold: %c %d %d %d %d (%d)\n\r", sst->option,
				                                                 sst->min1,
				                                                 sst->max1,
				                                                 sst->min2,
				                                                 sst->max2,
				                                                 smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleSetDebounce *ssd = ((SimpleSetDebounce*)data);
				BC->threshold_debounce = ssd->debounce;

				logbli("set debounce: %d\n\r", ssd->debounce);
				break;
			}
		}
	} else if(smp[id].direction == SIMPLE_DIRECTION_GET) {
		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_VALUE: {
				SimpleGetValue* sgv = (SimpleGetValue*)data;
				SimpleGetValueReturn sgvr = {
					sgv->stack_id,
					sgv->type,
					sizeof(SimpleGetValueReturn),
					BC->value1[smp[id].unit],
					BC->value2[smp[id].unit]
				};

				BA->send_blocking_with_timeout(&sgvr,
				                               sizeof(SimpleGetValueReturn),
				                               com);

				logbli("get value: %d %d\n\r", sgvr.value1, sgvr.value2);
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleGetPeriod* sgp = (SimpleGetPeriod*)data;
				SimpleGetPeriodReturn sgpr = {
					sgp->stack_id,
					sgp->type,
					sizeof(SimpleGetPeriodReturn),
					BC->signal_period[smp[id].unit]
				};

				BA->send_blocking_with_timeout(&sgpr,
				                               sizeof(SimpleGetPeriodReturn),
				                               com);

				logbli("get period: %d\n\r", sgpr.period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleGetThreshold* sgt = (SimpleGetThreshold*)data;
				SimpleGetThresholdReturn sgtr = {
					sgt->stack_id,
					sgt->type,
					sizeof(SimpleGetThresholdReturn),
					BC->threshold_option_save[smp[id].unit],
					BC->threshold_min_save1[smp[id].unit],
					BC->threshold_max_save1[smp[id].unit],
					BC->threshold_min_save2[smp[id].unit],
					BC->threshold_max_save2[smp[id].unit]
				};

				BA->send_blocking_with_timeout(&sgtr,
				                               sizeof(SimpleGetThresholdReturn),
				                               com);

				logbli("get threshold: %c %d %d %d %d (%d)\n\r", sgtr.option,
				                                                 sgtr.min1,
				                                                 sgtr.max1,
				                                                 sgtr.min2,
				                                                 sgtr.max2,
				                                                 smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleGetDebounce* sgd = (SimpleGetDebounce*)data;
				SimpleGetDebounceReturn sgdr = {
					sgd->stack_id,
					sgd->type,
					sizeof(SimpleGetDebounceReturn),
					BC->threshold_debounce
				};

				BA->send_blocking_with_timeout(&sgdr,
				                               sizeof(SimpleGetDebounceReturn),
				                               com);

				logbli("get debounce: %d\n\r", sgdr.debounce);
				break;
			}
		}
	}
}

void simple_constructor(void) {
	BC->threshold_debounce = 100;

	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		BC->value1[i] = 0;
		BC->value2[i] = 0;
		BC->last_value1[i] = 0xFFFFFFFF;
		BC->last_value2[i] = 0xFFFFFFFF;
		BC->signal_period[i] = 0;
		BC->signal_period_counter[i] = 0;
		BC->threshold_period_current[i] = 0;
		BC->threshold_min1[i] = 0;
		BC->threshold_max1[i] = 0;
		BC->threshold_min2[i] = 0;
		BC->threshold_max2[i] = 0;
		BC->threshold_option[i] = 'x';

		BC->threshold_min_save1[i] = 0;
		BC->threshold_max_save1[i] = 0;
		BC->threshold_min_save2[i] = 0;
		BC->threshold_max_save2[i] = 0;
		BC->threshold_option_save[i] = 'x';
	}

	BC->tick = 0;

	logbli("simple constructor\n\r");
}

void simple_destructor(void) {
	simple_constructor();
	logbli("simple destructor\n\r");
}

void simple_tick(void) {
	// Get values
    for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
    	if(BC->tick % 2) {
    		BC->value1[i] = BRICKLET_OFFSET_SIMPLE(sup[i].func1)(BC->value1[sup[i].call_value_id]);
    	} else {
    		BC->value2[i] = BRICKLET_OFFSET_SIMPLE(sup[i].func2)(BC->value2[sup[i].call_value_id]);
    	}
    }

    BC->tick++;

    // Handle period signals
    for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
    	if(BC->signal_period[i] != 0 &&
    	   BC->signal_period[i] <= BC->signal_period_counter[i]) {
    		if(BC->last_value1[i] != BC->value1[i] ||
    		   BC->last_value2[i] != BC->value2[i]) {
        		SimpleGetValueReturn sgvr = {
        			BS->stack_id,
        			sup[i].type_period,
        			sizeof(SimpleGetValueReturn),
        			BC->value1[i],
        			BC->value2[i]
        		};

        		BA->send_blocking_with_timeout(&sgvr,
        		                               sizeof(SimpleGetValueReturn),
        		                               *BA->com_current);

        		//logbli("period value: %d %d\n\r", BC->value1[i],
        		//                                 BC->value2[i]);
        		BC->signal_period_counter[i] = 0;
        		BC->last_value1[i] = BC->value1[i];
        		BC->last_value2[i] = BC->value2[i];
    		}
    	} else {
    		BC->signal_period_counter[i]++;
    	}
    }

    // Handle threshold signals
    for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
    	int32_t value1 = BC->value1[i];
    	int32_t value2 = BC->value2[i];

		if(((BC->threshold_option[i] == 'o') &&
		    ((value1 < BC->threshold_min1[i]) ||
		     (value1 > BC->threshold_max1[i])) &&
		    ((value2 < BC->threshold_min2[i]) ||
		     (value2 > BC->threshold_max2[i]))) ||
		   ((BC->threshold_option[i] == 'i') &&
		    ((value1 > BC->threshold_min1[i]) &&
		     (value1 < BC->threshold_max1[i])) &&
		    ((value2 > BC->threshold_min2[i]) &&
		     (value2 < BC->threshold_max2[i])))) {

			if(BC->threshold_period_current[i] == BC->threshold_debounce) {
	    		SimpleGetValueReturn sgvr = {
	    			BS->stack_id,
	    			sup[i].type_reached,
	    			sizeof(SimpleGetValueReturn),
	    			BC->value1[i],
	    			BC->value2[i]
	    		};
	    		BA->send_blocking_with_timeout(&sgvr,
    		                                   sizeof(SimpleGetValueReturn),
    		                                   *BA->com_current);
				BC->threshold_period_current[i] = 0;

				logbli("threshold value: %d %d\n\r", BC->value1[i],
				                                     BC->value2[i]);
			} else {
				BC->threshold_period_current[i]++;
			}
		} else {
			if(BC->threshold_period_current[i] != BC->threshold_debounce) {
				BC->threshold_period_current[i]++;
			}
		}
    }
}
#endif