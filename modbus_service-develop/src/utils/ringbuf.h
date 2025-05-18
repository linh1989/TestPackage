#ifndef _RINGBUF_H
#define _RINGBUF_H
/** @file ringbuf.h
 *  @brief
 *
 *  @author Sonlq
 *  @note No Note at the moment
 *  @bug No known bugs.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  	  Date        Changes
 * ----- -------- ---------- -----------------------------------------------
 * 1.0  Sonlq   Sep 16, 2020 	 First release
 *
 *
 * Copyright (C) VinFast R&D, LLC. All rights reserved.
 *
 *</pre>
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <stddef.h>
/************************** Constant Definitions *****************************/
#define MAX_RING_QUEUE 20
/**************************** Type Definitions *******************************/
typedef struct ring_queue_data {
    void* data;
    size_t dataLen;
} ring_queue_data_t;

typedef struct ring_queue {
    // ring_queue_data_t* msg[MAX_RING_QUEUE];
    ring_queue_data_t** msg;
    uint8_t u8MaxRingQueue;
    uint8_t u8RdCnt;
    uint8_t u8WrCnt;
    uint8_t u8Cnt;    
    pthread_mutex_t lock;
} ring_queue_t;
/******************************   Variables  *********************************/
/******************************  Interfaces   ********************************/
ring_queue_t* ring_queue_init(size_t max_ring_queue);
bool ring_queue_push(ring_queue_t *q, void *msg);
ring_queue_data_t *ring_queue_pop(ring_queue_t *q);
ring_queue_data_t *ring_queue_get(ring_queue_t *q);
uint8_t ring_queue_get_used_element(ring_queue_t *q);
void ring_queue_destroy(ring_queue_t *q);

/****************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _RINGBUF_H */
