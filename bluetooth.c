/**
LooCI Copyright (C) 2013 KU Leuven.
All rights reserved.

LooCI is an open-source software development kit for developing and maintaining networked embedded applications;
it is distributed under a dual-use software license model:

1. Non-commercial use:
Non-Profits, Academic Institutions, and Private Individuals can redistribute and/or modify LooCI code under the terms of the GNU General Public License version 3, as published by the Free Software Foundation
(http://www.gnu.org/licenses/gpl.html).

2. Commercial use:
In order to apply LooCI in commercial code, a dedicated software license must be negotiated with KU Leuven Research & Development.

Contact information:
  Administrative Contact: Sam Michiels, sam.michiels@cs.kuleuven.be
  Technical Contact:           Danny Hughes, danny.hughes@cs.kuleuven.be
Address:
  iMinds-DistriNet, KU Leuven
  Celestijnenlaan 200A - PB 2402,
  B-3001 Leuven,
  BELGIUM. 
 */
/*
 * Copyright (c) 2010, Katholieke Universiteit Leuven
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of the Katholieke Universiteit Leuven nor the names of
 *       its contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "contiki.h"
#include "looci.h"
#include "bluetooth.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef LOOCI_COMPONENT_DEBUG
#include "debug.h"
#else
#include "nodebug.h"
#endif

#include "uart.h"
#include <util/delay.h>

#define BT_DEVICE 4242

struct state{
	char ssid[64];
	int beacon;
	int rateScan;		// Seconds between scans
	struct etimer et;
};

// Initial state
static struct state initialState = {.ssid = "test", .beacon = 1, .rateScan = 2000};

#define LOOCI_COMPONENT_NAME bluetooth

//COMPONENT_NO_INTERFACES();
COMPONENT_INTERFACES(BT_DEVICE);
//COMPONENT_NO_RECEPTACLES();
COMPONENT_RECEPTACLES(BT_DEVICE);
#define LOOCI_NR_PROPERTIES 2
//format = {propertyId, datatype,offset,size,name}
LOOCI_PROPERTIES({1, DATATYPE_BYTE_ARRAY, offsetof(struct state,  ssid), 64, "ssid"},
			{2, DATATYPE_BYTE, offsetof(struct state,  beacon), 1, "beacon"},
			{3, DATATYPE_BYTE, offsetof(struct state,  rateScan), 1, "rateScan"});
//LOOCI_PROPERTIES();
//LOOCI_COMPONENT("bluetooth",struct state);
LOOCI_COMPONENT_INIT("bluetooth", struct state, &initialState);

#define MYUBBR 103

static char* test = "Initialising bluetooth component..";

static uint8_t init(struct state* compState, void* data){
	char buf[128];
	int i;
	
	PRINTF("%s",test);
	setupUART(MYUBBR);
	
	// Set in master mode
	serialWriteString("\r\n+STWMOD=1\r\n");
	_delay_ms(3000);
	serialWriteString("\r\n+STNA=test\r\n");
	_delay_ms(3000);
	
	return 1;
}

static uint8_t destroy(struct state* compState, void* data){
	return 1;

}

static uint8_t activate(struct state* compState, void* data){
	// Set timer to regularly print out data from shield
	ETIMER_SET(&compState->et, 2000);	// TODO set from state
	return 1;
}

static uint8_t deactivate(struct state* compState, void* data){
	ETIMER_STOP(&compState->et);
	return 1;
}

/*
 * Function called when etimer expires
 * Data contains the timer that expired
 */
static uint8_t time(struct state* compState, struct etimer* data){
	char buf[128];
	
	ETIMER_STOP(&compState->et);
	
	serialWriteString("\r\n+INQ=1\r\n");
	_delay_ms(400);

	do {
		serialReadString(buf, 10000);
		// Send event
		if (buf[0] != 0) {
			PRINT_LN("Discovered device");
			PUBLISH_EVENT(BT_DEVICE, &buf, strlen(buf));
		}
	} while (buf[0] != 0);

	serialWriteString("\r\n+INQ=0\r\n");
	
	ETIMER_RESTART(&compState->et);
	return 1;

}

static uint8_t event(struct state* compState, core_looci_event_t* event){
	//uint8_t test = 1;
	//PUBLISH_EVENT(APPLICATION_EVENT_TYPE,&test,sizeof(test));
	return 1;
}

static uint8_t defaultFunc(struct state* state,struct contiki_call* data){
	return 0;
}

static uint8_t propertySet(struct state* compState,struct contiki_call* data){
	// TODO
	return 1;
}


//FUNCTION DECLARATION
// You can comment out unused functions
COMP_FUNCS_INIT //THIS LINE MUST BE PRESENT
COMP_FUNC_INIT(init)
COMP_FUNC_DESTROY(destroy)
COMP_FUNC_ACTIVATE(activate)
COMP_FUNC_DEACTIVATE(deactivate)
COMP_FUNC_TIMER(time)
COMP_FUNC_EVENT(event)
COMP_FUNC_PROPERTY_IS_SET(propertySet)
COMP_FUNCS_END(defaultFunc)//THIS LINE MUST BE PRESENT
