/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2007 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011-2013 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2014      Intel, Inc. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "pmix_config.h"

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include "types.h"
#include "src/util/error.h"
#include "src/util/output.h"
#include "src/buffer_ops/internal.h"

int pmix_bfrop_pack(pmix_buffer_t *buffer,
                  const void *src, int32_t num_vals,
                  pmix_data_type_t type)
{
    int rc;

    /* check for error */
    if (NULL == buffer) {
        return PMIX_ERR_BAD_PARAM;
    }

    /* Pack the number of values */
    if (PMIX_BFROP_BUFFER_FULLY_DESC == buffer->type) {
        if (PMIX_SUCCESS != (rc = pmix_bfrop_store_data_type(buffer, PMIX_INT32))) {
            return rc;
        }
    }
    if (PMIX_SUCCESS != (rc = pmix_bfrop_pack_int32(buffer, &num_vals, 1, PMIX_INT32))) {
        return rc;
    }

    /* Pack the value(s) */
    return pmix_bfrop_pack_buffer(buffer, src, num_vals, type);
}

int pmix_bfrop_pack_buffer(pmix_buffer_t *buffer,
                         const void *src, int32_t num_vals,
                         pmix_data_type_t type)
{
    int rc;
    pmix_bfrop_type_info_t *info;

    PMIX_OUTPUT( ( pmix_bfrop_verbose, "pmix_bfrop_pack_buffer( %p, %p, %lu, %d )\n",
                   (void*)buffer, src, (long unsigned int)num_vals, (int)type ) );

    /* Pack the declared data type */
    if (PMIX_BFROP_BUFFER_FULLY_DESC == buffer->type) {
        if (PMIX_SUCCESS != (rc = pmix_bfrop_store_data_type(buffer, type))) {
            return rc;
        }
    }

    /* Lookup the pack function for this type and call it */

    if (NULL == (info = (pmix_bfrop_type_info_t*)pmix_pointer_array_get_item(&pmix_bfrop_types, type))) {
        return PMIX_ERR_PACK_FAILURE;
    }

    return info->odti_pack_fn(buffer, src, num_vals, type);
}


/* PACK FUNCTIONS FOR GENERIC SYSTEM TYPES */

/*
 * BOOL
 */
int pmix_bfrop_pack_bool(pmix_buffer_t *buffer, const void *src,
                       int32_t num_vals, pmix_data_type_t type)
{
    int ret;

    /* System types need to always be described so we can properly
       unpack them.  If we aren't fully described, then add the
       description for this type... */
    if (PMIX_BFROP_BUFFER_FULLY_DESC != buffer->type) {
        if (PMIX_SUCCESS != (ret = pmix_bfrop_store_data_type(buffer, BFROP_TYPE_BOOL))) {
            return ret;
        }
    }

    /* Turn around and pack the real type */
    return pmix_bfrop_pack_buffer(buffer, src, num_vals, BFROP_TYPE_BOOL);
}

/*
 * INT
 */
int pmix_bfrop_pack_int(pmix_buffer_t *buffer, const void *src,
                      int32_t num_vals, pmix_data_type_t type)
{
    int ret;

    /* System types need to always be described so we can properly
       unpack them.  If we aren't fully described, then add the
       description for this type... */
    if (PMIX_BFROP_BUFFER_FULLY_DESC != buffer->type) {
        if (PMIX_SUCCESS != (ret = pmix_bfrop_store_data_type(buffer, BFROP_TYPE_INT))) {
            return ret;
        }
    }

    /* Turn around and pack the real type */
    return pmix_bfrop_pack_buffer(buffer, src, num_vals, BFROP_TYPE_INT);
}

/*
 * SIZE_T
 */
int pmix_bfrop_pack_sizet(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    int ret;

    /* System types need to always be described so we can properly
       unpack them.  If we aren't fully described, then add the
       description for this type... */
    if (PMIX_BFROP_BUFFER_FULLY_DESC != buffer->type) {
        if (PMIX_SUCCESS != (ret = pmix_bfrop_store_data_type(buffer, BFROP_TYPE_SIZE_T))) {
            return ret;
        }
    }

    return pmix_bfrop_pack_buffer(buffer, src, num_vals, BFROP_TYPE_SIZE_T);
}

/*
 * PID_T
 */
int pmix_bfrop_pack_pid(pmix_buffer_t *buffer, const void *src,
                      int32_t num_vals, pmix_data_type_t type)
{
    int ret;

    /* System types need to always be described so we can properly
       unpack them.  If we aren't fully described, then add the
       description for this type... */
    if (PMIX_BFROP_BUFFER_FULLY_DESC != buffer->type) {
        if (PMIX_SUCCESS != (ret = pmix_bfrop_store_data_type(buffer, BFROP_TYPE_PID_T))) {
            return ret;
        }
    }

    /* Turn around and pack the real type */
    return pmix_bfrop_pack_buffer(buffer, src, num_vals, BFROP_TYPE_PID_T);
}


/* PACK FUNCTIONS FOR NON-GENERIC SYSTEM TYPES */

/*
 * NULL
 */
int pmix_bfrop_pack_null(pmix_buffer_t *buffer, const void *src,
                       int32_t num_vals, pmix_data_type_t type)
{
    char null=0x00;
    char *dst;

    PMIX_OUTPUT( ( pmix_bfrop_verbose, "pmix_bfrop_pack_null * %d\n", num_vals ) );
    /* check to see if buffer needs extending */
    if (NULL == (dst = pmix_bfrop_buffer_extend(buffer, num_vals))) {
        return PMIX_ERR_OUT_OF_RESOURCE;
    }

    /* store the nulls */
    memset(dst, (int)null, num_vals);

    /* update buffer pointers */
    buffer->pack_ptr += num_vals;
    buffer->bytes_used += num_vals;

    return PMIX_SUCCESS;
}

/*
 * BYTE, CHAR, INT8
 */
int pmix_bfrop_pack_byte(pmix_buffer_t *buffer, const void *src,
                       int32_t num_vals, pmix_data_type_t type)
{
    char *dst;

    PMIX_OUTPUT( ( pmix_bfrop_verbose, "pmix_bfrop_pack_byte * %d\n", num_vals ) );
    /* check to see if buffer needs extending */
    if (NULL == (dst = pmix_bfrop_buffer_extend(buffer, num_vals))) {
        return PMIX_ERR_OUT_OF_RESOURCE;
    }

    /* store the data */
    memcpy(dst, src, num_vals);

    /* update buffer pointers */
    buffer->pack_ptr += num_vals;
    buffer->bytes_used += num_vals;

    return PMIX_SUCCESS;
}

/*
 * INT16
 */
int pmix_bfrop_pack_int16(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    int32_t i;
    uint16_t tmp, *srctmp = (uint16_t*) src;
    char *dst;

    PMIX_OUTPUT( ( pmix_bfrop_verbose, "pmix_bfrop_pack_int16 * %d\n", num_vals ) );
    /* check to see if buffer needs extending */
    if (NULL == (dst = pmix_bfrop_buffer_extend(buffer, num_vals*sizeof(tmp)))) {
        return PMIX_ERR_OUT_OF_RESOURCE;
    }

    for (i = 0; i < num_vals; ++i) {
        tmp = htons(srctmp[i]);
        memcpy(dst, &tmp, sizeof(tmp));
        dst += sizeof(tmp);
    }
    buffer->pack_ptr += num_vals * sizeof(tmp);
    buffer->bytes_used += num_vals * sizeof(tmp);

    return PMIX_SUCCESS;
}

/*
 * INT32
 */
int pmix_bfrop_pack_int32(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    int32_t i;
    uint32_t tmp, *srctmp = (uint32_t*) src;
    char *dst;

    PMIX_OUTPUT( ( pmix_bfrop_verbose, "pmix_bfrop_pack_int32 * %d\n", num_vals ) );
    /* check to see if buffer needs extending */
    if (NULL == (dst = pmix_bfrop_buffer_extend(buffer, num_vals*sizeof(tmp)))) {
        return PMIX_ERR_OUT_OF_RESOURCE;
    }

    for (i = 0; i < num_vals; ++i) {
        tmp = htonl(srctmp[i]);
        memcpy(dst, &tmp, sizeof(tmp));
        dst += sizeof(tmp);
    }
    buffer->pack_ptr += num_vals * sizeof(tmp);
    buffer->bytes_used += num_vals * sizeof(tmp);

    return PMIX_SUCCESS;
}

/*
 * INT64
 */
int pmix_bfrop_pack_int64(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    int32_t i;
    uint64_t tmp, *srctmp = (uint64_t*) src;
    char *dst;
    size_t bytes_packed = num_vals * sizeof(tmp);

    PMIX_OUTPUT( ( pmix_bfrop_verbose, "pmix_bfrop_pack_int64 * %d\n", num_vals ) );
    /* check to see if buffer needs extending */
    if (NULL == (dst = pmix_bfrop_buffer_extend(buffer, bytes_packed))) {
        return PMIX_ERR_OUT_OF_RESOURCE;
    }

    for (i = 0; i < num_vals; ++i) {
        tmp = hton64(srctmp[i]);
        memcpy(dst, &tmp, sizeof(tmp));
        dst += sizeof(tmp);
    }
    buffer->pack_ptr += bytes_packed;
    buffer->bytes_used += bytes_packed;

    return PMIX_SUCCESS;
}

/*
 * STRING
 */
int pmix_bfrop_pack_string(pmix_buffer_t *buffer, const void *src,
                         int32_t num_vals, pmix_data_type_t type)
{
    int ret = PMIX_SUCCESS;
    int32_t i, len;
    char **ssrc = (char**) src;

    for (i = 0; i < num_vals; ++i) {
        if (NULL == ssrc[i]) {  /* got zero-length string/NULL pointer - store NULL */
            len = 0;
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_int32(buffer, &len, 1, PMIX_INT32))) {
                return ret;
            }
        } else {
            len = (int32_t)strlen(ssrc[i]) + 1;
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_int32(buffer, &len, 1, PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS != (ret =
                pmix_bfrop_pack_byte(buffer, ssrc[i], len, PMIX_BYTE))) {
                return ret;
            }
        }
    }

    return PMIX_SUCCESS;
}

/* FLOAT */
int pmix_bfrop_pack_float(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    int ret = PMIX_SUCCESS;
    int32_t i;
    float *ssrc = (float*)src;
    char *convert;

    for (i = 0; i < num_vals; ++i) {
        asprintf(&convert, "%f", ssrc[i]);
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_string(buffer, &convert, 1, PMIX_STRING))) {
            free(convert);
            return ret;
        }
        free(convert);
    }

    return PMIX_SUCCESS;
}

/* DOUBLE */
int pmix_bfrop_pack_double(pmix_buffer_t *buffer, const void *src,
                         int32_t num_vals, pmix_data_type_t type)
{
    int ret = PMIX_SUCCESS;
    int32_t i;
    double *ssrc = (double*)src;
    char *convert;

    for (i = 0; i < num_vals; ++i) {
        asprintf(&convert, "%f", ssrc[i]);
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_string(buffer, &convert, 1, PMIX_STRING))) {
            free(convert);
            return ret;
        }
        free(convert);
    }

    return PMIX_SUCCESS;
}

/* TIMEVAL */
int pmix_bfrop_pack_timeval(pmix_buffer_t *buffer, const void *src,
                          int32_t num_vals, pmix_data_type_t type)
{
    int64_t tmp[2];
    int ret = PMIX_SUCCESS;
    int32_t i;
    struct timeval *ssrc = (struct timeval *)src;

    for (i = 0; i < num_vals; ++i) {
        tmp[0] = (int64_t)ssrc[i].tv_sec;
        tmp[1] = (int64_t)ssrc[i].tv_usec;
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_int64(buffer, tmp, 2, PMIX_INT64))) {
            return ret;
        }
    }

    return PMIX_SUCCESS;
}

/* TIME */
int pmix_bfrop_pack_time(pmix_buffer_t *buffer, const void *src,
                       int32_t num_vals, pmix_data_type_t type)
{
    int ret = PMIX_SUCCESS;
    int32_t i;
    time_t *ssrc = (time_t *)src;
    uint64_t ui64;

    /* time_t is a system-dependent size, so cast it
     * to uint64_t as a generic safe size
     */
    for (i = 0; i < num_vals; ++i) {
        ui64 = (uint64_t)ssrc[i];
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_int64(buffer, &ui64, 1, PMIX_UINT64))) {
            return ret;
        }
    }

    return PMIX_SUCCESS;
}


/* PACK FUNCTIONS FOR GENERIC PMIX TYPES */

/*
 * PMIX_DATA_TYPE
 */
int pmix_bfrop_pack_data_type(pmix_buffer_t *buffer, const void *src, int32_t num_vals,
                            pmix_data_type_t type)
{
    int ret;
    
    /* Turn around and pack the real type */
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, src, num_vals, PMIX_DATA_TYPE_T))) {
    }
    
    return ret;
}

/*
 * PMIX_BYTE_OBJECT
 */
int pmix_bfrop_pack_byte_object(pmix_buffer_t *buffer, const void *src, int32_t num,
                             pmix_data_type_t type)
{
    pmix_byte_object_t **sbyteptr;
    int32_t i, n;
    int ret;

    sbyteptr = (pmix_byte_object_t **) src;

    for (i = 0; i < num; ++i) {
        n = sbyteptr[i]->size;
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_int32(buffer, &n, 1, PMIX_INT32))) {
            return ret;
        }
        if (0 < n) {
            if (PMIX_SUCCESS != (ret =
                pmix_bfrop_pack_byte(buffer, sbyteptr[i]->bytes, n, PMIX_BYTE))) {
                return ret;
            }
        }
    }

    return PMIX_SUCCESS;
}

/*
 * PMIX_PSTAT
 */
int pmix_bfrop_pack_pstat(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    pmix_pstats_t **ptr;
    int32_t i;
    int ret;
    char *cptr;
    
    ptr = (pmix_pstats_t **) src;
    
    for (i = 0; i < num_vals; ++i) {
        cptr = ptr[i]->node;
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &cptr, 1, PMIX_STRING))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->rank, 1, PMIX_INT32))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->pid, 1, PMIX_PID))) {
            return ret;
        }
        cptr = ptr[i]->cmd;
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &cptr, 1, PMIX_STRING))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->state[0], 1, PMIX_BYTE))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->time, 1, PMIX_TIMEVAL))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->priority, 1, PMIX_INT32))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->num_threads, 1, PMIX_INT16))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->vsize, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->rss, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->peak_vsize, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->processor, 1, PMIX_INT16))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->sample_time, 1, PMIX_TIMEVAL))) {
            return ret;
        }
    }

    return PMIX_SUCCESS;
}

static int pack_disk_stats(pmix_buffer_t *buffer, pmix_diskstats_t *dk)
{
    uint64_t i64;
    int ret;

    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &dk->disk, 1, PMIX_STRING))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_reads_completed;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_reads_merged;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_sectors_read;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->milliseconds_reading;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_writes_completed;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_writes_merged;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_sectors_written;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->milliseconds_writing;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->num_ios_in_progress;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->milliseconds_io;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)dk->weighted_milliseconds_io;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    return PMIX_SUCCESS;
}

static int pack_net_stats(pmix_buffer_t *buffer, pmix_netstats_t *ns)
{
    uint64_t i64;
    int ret;

    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ns->net_interface, 1, PMIX_STRING))) {
        return ret;
    }
    i64 = (uint64_t)ns->num_bytes_recvd;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)ns->num_packets_recvd;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)ns->num_recv_errs;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)ns->num_bytes_sent;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)ns->num_packets_sent;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    i64 = (uint64_t)ns->num_send_errs;
    if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &i64, 1, PMIX_UINT64))) {
        return ret;
    }
    return PMIX_SUCCESS;
}

/*
 * PMIX_NODE_STAT
 */
int pmix_bfrop_pack_node_stat(pmix_buffer_t *buffer, const void *src,
                            int32_t num_vals, pmix_data_type_t type)
{
    pmix_node_stats_t **ptr;
    int32_t i, j;
    int ret;
    pmix_diskstats_t *ds;
    pmix_netstats_t *ns;

    ptr = (pmix_node_stats_t **) src;
    
    for (i = 0; i < num_vals; ++i) {
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->la, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->la5, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->la15, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->total_mem, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->free_mem, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->buffers, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->cached, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->swap_cached, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->swap_total, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->swap_free, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_float(buffer, &ptr[i]->mapped, 1, PMIX_FLOAT))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->sample_time, 1, PMIX_TIMEVAL))) {
            return ret;
        }
        /* pack the number of disk stat objects on the list */
        j = pmix_list_get_size(&ptr[i]->diskstats);
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &j, 1, PMIX_INT32))) {
            return ret;
        }
        if (0 < j) {
            /* pack them */
            PMIX_LIST_FOREACH(ds, &ptr[i]->diskstats, pmix_diskstats_t) {
                if (PMIX_SUCCESS != (ret = pack_disk_stats(buffer, ds))) {
                    return ret;
                }
            }
        }
        /* pack the number of net stat objects on the list */
        j = pmix_list_get_size(&ptr[i]->netstats);
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &j, 1, PMIX_INT32))) {
            return ret;
        }
        if (0 < j) {
            /* pack them */
            PMIX_LIST_FOREACH(ns, &ptr[i]->netstats, pmix_netstats_t) {
                if (PMIX_SUCCESS != (ret = pack_net_stats(buffer, ns))) {
                    return ret;
                }
            }
        }
    }

    return PMIX_SUCCESS;
}

/*
 * PMIX_VALUE
 */
int pmix_bfrop_pack_value(pmix_buffer_t *buffer, const void *src,
                        int32_t num_vals, pmix_data_type_t type)
{
    pmix_value_t **ptr;
    int32_t i, n;
    int ret;

    ptr = (pmix_value_t **) src;
    
    for (i = 0; i < num_vals; ++i) {
        /* pack the key and type */
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_string(buffer, &ptr[i]->key, 1, PMIX_STRING))) {
            return ret;
        }
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_data_type(buffer, &ptr[i]->type, 1, PMIX_DATA_TYPE))) {
            return ret;
        }
        /* now pack the right field */
        switch (ptr[i]->type) {
        case PMIX_BOOL:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.flag, 1, PMIX_BOOL))) {
                return ret;
            }
            break;
        case PMIX_BYTE:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.byte, 1, PMIX_BYTE))) {
                return ret;
            }
            break;
        case PMIX_STRING:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.string, 1, PMIX_STRING))) {
                return ret;
            }
            break;
        case PMIX_SIZE:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.size, 1, PMIX_SIZE))) {
                return ret;
            }
            break;
        case PMIX_PID:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.pid, 1, PMIX_PID))) {
                return ret;
            }
            break;
        case PMIX_INT:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.integer, 1, PMIX_INT))) {
                return ret;
            }
            break;
        case PMIX_INT8:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.int8, 1, PMIX_INT8))) {
                return ret;
            }
            break;
        case PMIX_INT16:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.int16, 1, PMIX_INT16))) {
                return ret;
            }
            break;
        case PMIX_INT32:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.int32, 1, PMIX_INT32))) {
                return ret;
            }
            break;
        case PMIX_INT64:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.int64, 1, PMIX_INT64))) {
                return ret;
            }
            break;
        case PMIX_UINT:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.uint, 1, PMIX_UINT))) {
                return ret;
            }
            break;
        case PMIX_UINT8:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.uint8, 1, PMIX_UINT8))) {
                return ret;
            }
            break;
        case PMIX_UINT16:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.uint16, 1, PMIX_UINT16))) {
                return ret;
            }
            break;
        case PMIX_UINT32:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.uint32, 1, PMIX_UINT32))) {
                return ret;
            }
            break;
        case PMIX_UINT64:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.uint64, 1, PMIX_UINT64))) {
                return ret;
            }
            break;
        case PMIX_BYTE_OBJECT:
            /* have to pack by hand so we can match unpack without allocation */
            n = ptr[i]->data.bo.size;
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_int32(buffer, &n, 1, PMIX_INT32))) {
                return ret;
            }
            if (0 < n) {
                if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_byte(buffer, ptr[i]->data.bo.bytes, n, PMIX_BYTE))) {
                    return ret;
                }
            }
            break;
        case PMIX_FLOAT:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.fval, 1, PMIX_FLOAT))) {
                return ret;
            }
            break;
        case PMIX_DOUBLE:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.dval, 1, PMIX_DOUBLE))) {
                return ret;
            }
            break;
        case PMIX_TIMEVAL:
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_buffer(buffer, &ptr[i]->data.tv, 1, PMIX_TIMEVAL))) {
                return ret;
            }
            break;
        case PMIX_PTR:
            /* just ignore these values */
            break;
        case PMIX_FLOAT_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.fval_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.fval_array.data,
                                            ptr[i]->data.fval_array.size,
                                            PMIX_FLOAT))) {
                return ret;
            }
            break;
        case PMIX_DOUBLE_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.dval_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.dval_array.data,
                                            ptr[i]->data.dval_array.size,
                                            PMIX_DOUBLE))) {
                return ret;
            }
            break;
        case PMIX_STRING_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.string_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.string_array.data,
                                            ptr[i]->data.string_array.size,
                                            PMIX_STRING))) {
                return ret;
            }
            break;
        case PMIX_BOOL_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.flag_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.flag_array.data,
                                            ptr[i]->data.flag_array.size,
                                            PMIX_BOOL))) {
                return ret;
            }
            break;
        case PMIX_SIZE_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.size_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.size_array.data,
                                            ptr[i]->data.size_array.size,
                                            PMIX_SIZE))) {
                return ret;
            }
            break;
        case PMIX_BYTE_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.byte_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.byte_array.data,
                                            ptr[i]->data.byte_array.size,
                                            PMIX_BYTE))) {
                return ret;
            }
            break;
        case PMIX_INT_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.integer_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.integer_array.data,
                                            ptr[i]->data.integer_array.size,
                                            PMIX_INT))) {
                return ret;
            }
            break;
        case PMIX_INT8_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.int8_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.int8_array.data,
                                            ptr[i]->data.int8_array.size,
                                            PMIX_INT8))) {
                return ret;
            }
            break;
        case PMIX_INT16_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.int16_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.int16_array.data,
                                            ptr[i]->data.int16_array.size,
                                            PMIX_INT16))) {
                return ret;
            }
            break;
        case PMIX_INT32_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.int32_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.int32_array.data,
                                            ptr[i]->data.int32_array.size,
                                            PMIX_INT32))) {
                return ret;
            }
            break;
        case PMIX_INT64_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.int64_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.int64_array.data,
                                            ptr[i]->data.int64_array.size,
                                            PMIX_INT64))) {
                return ret;
            }
            break;
        case PMIX_UINT_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.uint_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.uint_array.data,
                                            ptr[i]->data.uint_array.size,
                                            PMIX_UINT))) {
                return ret;
            }
            break;
        case PMIX_UINT8_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.uint8_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.uint8_array.data,
                                            ptr[i]->data.uint8_array.size,
                                            PMIX_UINT8))) {
                return ret;
            }
            break;
        case PMIX_UINT16_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.uint16_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.uint16_array.data,
                                            ptr[i]->data.uint16_array.size,
                                            PMIX_UINT16))) {
                return ret;
            }
            break;
        case PMIX_UINT32_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.uint32_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.uint32_array.data,
                                            ptr[i]->data.uint32_array.size,
                                            PMIX_UINT32))) {
                return ret;
            }
            break;
        case PMIX_UINT64_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.uint64_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.uint64_array.data,
                                            ptr[i]->data.uint64_array.size,
                                            PMIX_UINT64))) {
                return ret;
            }
            break;
        case PMIX_PID_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.pid_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.pid_array.data,
                                            ptr[i]->data.pid_array.size,
                                            PMIX_PID))) {
                return ret;
            }
            break;
        case PMIX_TIMEVAL_ARRAY:
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            &ptr[i]->data.tv_array.size,
                                            1,
                                            PMIX_INT32))) {
                return ret;
            }
            if (PMIX_SUCCESS !=
                (ret = pmix_bfrop_pack_buffer(buffer,
                                            ptr[i]->data.tv_array.data,
                                            ptr[i]->data.tv_array.size,
                                            PMIX_TIMEVAL))) {
                return ret;
            }
            break;
        default:
            pmix_output(0, "PACK-PMIX-VALUE: UNSUPPORTED TYPE %d", (int)ptr[i]->type);
            return PMIX_ERROR;
        }
    }

    return PMIX_SUCCESS;
}


/*
 * BUFFER CONTENTS
 */
int pmix_bfrop_pack_buffer_contents(pmix_buffer_t *buffer, const void *src,
                                  int32_t num_vals, pmix_data_type_t type)
{
    pmix_buffer_t **ptr;
    int32_t i;
    int ret;

    ptr = (pmix_buffer_t **) src;
    
    for (i = 0; i < num_vals; ++i) {
        /* pack the number of bytes */
        PMIX_OUTPUT((pmix_bfrop_verbose, "pmix_bfrop_pack_buffer_contents: bytes_used %u\n", (unsigned)ptr[i]->bytes_used));
        if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_sizet(buffer, &ptr[i]->bytes_used, 1, PMIX_SIZE))) {
            return ret;
        }
        /* pack the bytes */
        if (0 < ptr[i]->bytes_used) {
            if (PMIX_SUCCESS != (ret = pmix_bfrop_pack_byte(buffer, ptr[i]->base_ptr, ptr[i]->bytes_used, PMIX_BYTE))) {
                return ret;
            }
        } else {
            ptr[i]->base_ptr = NULL;
        }
    }
    return PMIX_SUCCESS;
}


