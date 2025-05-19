/** @file ringbuf.c
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
 * 1.0  Sonlq   Sep 20, 2020 	 First release
 *
 *
 * Copyright (C) VinFast R&D, LLC. All rights reserved.
 *
 *</pre>
 *****************************************************************************/

/***************************** Include Files *********************************/
#include "ringbuf.h"
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Variable Definitions *****************************/

/**************************       Functions      *****************************/
ring_queue_t* ring_queue_init(size_t max_ring_queue)
{
    if(max_ring_queue < UINT8_MAX)
    {
        ring_queue_t* q = (ring_queue_t*)malloc(sizeof(ring_queue_t));
        q->u8Cnt = 0;
        q->u8RdCnt = 0;
        q->u8WrCnt = 0;
        q->u8MaxRingQueue = max_ring_queue;
        q->msg = (ring_queue_data_t**)malloc(sizeof(ring_queue_data_t*)* max_ring_queue);
        memset(q->msg, 0, max_ring_queue);
        pthread_mutex_init(&(q->lock), NULL);
        return q;
    }    
    else
    {
        return NULL;
    }    
}

bool ring_queue_push(ring_queue_t* q, void* msg)
{
    bool ret = false;
    pthread_mutex_lock(&(q->lock));
    if (q->u8Cnt < MAX_RING_QUEUE) {
        q->msg[q->u8WrCnt] = msg;
        q->u8Cnt++;
        q->u8WrCnt++;
        if (q->u8WrCnt >= MAX_RING_QUEUE) {
            q->u8WrCnt = 0;
        }
        ret = true;
    }
    
    pthread_mutex_unlock(&(q->lock));
    return ret;
}

ring_queue_data_t* ring_queue_pop(ring_queue_t* q)
{
    ring_queue_data_t* msg = NULL;
    pthread_mutex_lock(&(q->lock));
    if (q->u8Cnt > 0) // found defer messeage
    {
        if (q->msg[q->u8RdCnt] != NULL) {
            msg = q->msg[q->u8RdCnt];
            q->u8Cnt--;
            q->u8RdCnt++;
            if (q->u8RdCnt >= MAX_RING_QUEUE) {
                q->u8RdCnt = 0;
            }
        }
    }
    pthread_mutex_unlock(&(q->lock));
    return msg;
}

ring_queue_data_t* ring_queue_get(ring_queue_t* q)
{
    ring_queue_data_t* msg = NULL;
    pthread_mutex_lock(&(q->lock));
    if (q->u8Cnt > 0) {
        if (q->msg[q->u8RdCnt] != NULL) {
            msg = q->msg[q->u8RdCnt];
        }
    }
    pthread_mutex_unlock(&(q->lock));
    return msg;
}

uint8_t ring_queue_get_used_element(ring_queue_t* q)
{
    uint8_t totalElem = 0;
    pthread_mutex_lock(&(q->lock));
    totalElem = q->u8Cnt;
    pthread_mutex_unlock(&(q->lock));
    return totalElem;
}

void ring_queue_destroy(ring_queue_t* q)
{
    if (q != NULL) {
        if(q->msg != NULL)
        {
            free(q->msg);
        }
        free(q);
    }
}
