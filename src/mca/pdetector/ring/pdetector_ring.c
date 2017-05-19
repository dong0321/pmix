/*
 * Copyright (c) 2010      Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2011-2012 Los Alamos National Security, LLC.  All rights
 *                         reserved.
  *
 * Copyright (c) 2017      Intel, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include <src/include/pmix_config.h>
#include <pmix_common.h>

#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif  /* HAVE_UNISTD_H */
#ifdef HAVE_STRING_H
#include <string.h>
#endif  /* HAVE_STRING_H */
#include <stdio.h>
#include <pthread.h>
#include PMIX_EVENT_HEADER

#include "src/util/argv.h"
#include "src/util/error.h"
#include "src/util/output.h"
#include "src/util/show_help.h"
#include "src/include/pmix_globals.h"
#include "src/mca/ptl/ptl.h"

#include "src/mca/pdetector/base/base.h"
#include "pdetector_ring.h"

/* declare the API functions */
static pmix_status_t ring_start();
static pmix_status_t ring_stop();

/* instantiate the module */
pmix_pdetector_base_module_t pmix_pdetector_ring_module = {
    .start = ring_start,
    .stop = ring_stop
};

/*
 * Local functions
 */
static int fd_heartbeat_request(pmix_pdetector_ring_t* ring);
static int fd_heartbeat_send(pmix_pdetector_ring_t* ring);

static int fd_heartbeat_request_cb(struct pmix_peer_t *peer,
                                   pmix_ptl_hdr_t *hdr,
                                   pmix_buffer_t *buf, void *cbdata);

static int fd_heartbeat_recv_cb(struct pmix_peer_t *peer,
                                pmix_ptl_hdr_t *hdr,
                                pmix_buffer_t *buf, void *cbdata);

static double Wtime();
static double pmix_ring_heartbeat_period = 2e-1;
static double pmix_ring_heartbeat_timeout = 5e-1;
static pmix_event_base_t* fd_event_base = NULL;
static void fd_event_cb(int fd, short flags, void* pring);

/* define a local caddy */
typedef struct {
    pmix_object_t super;
    pmix_event_t ev;
    pmix_peer_t *requestor;
    char *id;
} ring_caddy_t;

typedef struct {
    pmix_object_t super;
    pmix_event_t ev;
    pmix_peer_t *peer;
} pmix_pdetector_beat_t;

static pmix_status_t ring_start()
{
    //pmix_ring_trkr_t *ft;
    size_t n;

    PMIX_OUTPUT_VERBOSE((1, pmix_pdetector_base_framework.framework_output,
                         "[%s:%d] starting the detector for heartbeat",
                         pmix_globals.myid.nspace, pmix_globals.myid.rank));

    pmix_ptl.recv(pmix_globals.mypeer, fd_heartbeat_recv_cb, PMIX_PTL_TAG_RING_HEARTBEAT);
    pmix_ptl.recv(pmix_globals.mypeer, fd_heartbeat_recv_cb, PMIX_PTL_TAG_RING_HEARTBEAT_REQUEST);

    /* need to push into our event base to add this to our trackers */
  /*  pmix_event_assign(&ft->cdev, pmix_pdetector_base.evbase, -1,
                      EV_WRITE, add_tracker, ft);
    pmix_event_active(&ft->cdev, EV_WRITE, 1);
*/
    return PMIX_SUCCESS;
}

static pmix_status_t ring_stop(pmix_peer_t *requestor, char *id)
{
 /*   ring_caddy_t *cd;

    cd = PMIX_NEW(ring_caddy_t);
    PMIX_RETAIN(requestor);
    cd->requestor = requestor;
    cd->id = strdup(id);

    /* need to push into our event base to add this to our trackers */
   /* pmix_event_assign(&cd->ev, pmix_pdetector_base.evbase, -1,
                      EV_WRITE, del_tracker, cd);
    pmix_event_active(&cd->ev, EV_WRITE, 1);
*/
    return PMIX_SUCCESS;
}

static int fd_heartbeat_recv_cb(struct pmix_peer_t *peer,
        pmix_ptl_hdr_t *hdr,
        pmix_buffer_t *buf, void *cbdata)
{
    return PMIX_SUCCESS;
}

