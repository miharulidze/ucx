/**
 * Copyright (c) NVIDIA CORPORATION & AFFILIATES, 2001-2015. ALL RIGHTS RESERVED.
 *
 * See file LICENSE for terms.
 */


#ifndef UCP_DT_IOV_H_
#define UCP_DT_IOV_H_

#include <ucp/api/ucp.h>
#include <ucp/dt/dt.h>


#define UCP_DT_IS_IOV(_datatype) \
    (((_datatype) & UCP_DATATYPE_CLASS_MASK) == UCP_DATATYPE_IOV)


/**
 * Get the total length of the data in @a iov buffers
 *
 * @param [in]     iov            @ref ucp_dt_iov_t buffer
 * @param [in]     iovcnt         Number of entries in the @a iov buffer
 *
 * @return Total length of data in the @a iov buffers
 */
static inline size_t ucp_dt_iov_length(const ucp_dt_iov_t *iov, size_t iovcnt)
{
    size_t iov_it, total_length = 0;

    for (iov_it = 0; iov_it < iovcnt; ++iov_it) {
        /* cppcheck-suppress nullPointer */
        total_length += iov[iov_it].length;
    }

    return total_length;
}

/**
 * Copy iov data buffers from @a iov to contiguous buffer @a dest with
 * an iov item data @a iov_offset and iov item @a iovcnt_offset
 *
 * @param [in]     dest           Destination contiguous buffer
 *                                (no offset applicable)
 * @param [in]     iov            Source @ref ucp_dt_iov_t buffer
 * @param [in]     length         Total data length to copy in bytes
 * @param [inout]  iov_offset     The offset in bytes to start copying
 *                                from an @a iov item pointed by
 *                                @a iovcnt_offset. The @a iov_offset is not aligned
 *                                by @ref ucp_dt_iov_t items length.
 * @param [inout]  iovcnt_offset  Auxiliary offset to select @a iov item that
 *                                belongs to the @a iov_offset. The point to start
 *                                copying from should be selected as
 *                                iov[iovcnt_offset].buffer + iov_offset
 */
void ucp_dt_iov_gather(ucp_worker_h worker, void *dest, const ucp_dt_iov_t *iov,
                       size_t length, size_t *iov_offset, size_t *iovcnt_offset,
                       ucs_memory_type_t mem_type);


/**
 * Copy contiguous buffer @a src into @ref ucp_dt_iov_t data buffers in @a iov
 * with an iov item data @a iov_offset and iov item @a iovcnt_offset
 *
 * @param [in]     iov            Destination @ref ucp_dt_iov_t buffer
 * @param [in]     iovcnt         Size of the @a iov buffer
 * @param [in]     src            Source contiguous buffer (no offset applicable)
 * @param [in]     length         Total data length to copy in bytes
 * @param [inout]  iov_offset     The offset in bytes to start copying
 *                                to an @a iov item pointed by @a iovcnt_offset.
 *                                The @a iov_offset is not aligned by
 *                                @ref ucp_dt_iov_t items length.
 * @param [inout]  iovcnt_offset  Auxiliary offset to select @a iov item that
 *                                belongs to the @a iov_offset. The point to
 *                                start copying to should be selected as
 *                                iov[iovcnt_offset].buffer + iov_offset
 *
 * @return Size in bytes that was actually copied from @a src to @a iov. It must
 *         be less or equal to @a length.
 */
size_t ucp_dt_iov_scatter(ucp_worker_h worker, const ucp_dt_iov_t *iov,
                          size_t iovcnt, const void *src, size_t length,
                          size_t *iov_offset, size_t *iovcnt_offset,
                          ucs_memory_type_t mem_type);


/**
 * Seek to a logical offset in the iov
 *
 * @param [in]     iov            @ref ucp_dt_iov_t buffer to seek in
 * @param [in]     iovcnt         Number of entries in the @a iov buffer
 * @param [in]     distance       Distance to move, relative to the current
 *                                location
 * @param [inout]  iov_offset     The offset in bytes from the beginning of the
 *                                current iov entry
 * @param [inout]  iovcnt_offset  Current @a iov item index
 */
void ucp_dt_iov_seek(ucp_dt_iov_t *iov, size_t iovcnt, ptrdiff_t distance,
                     size_t *iov_offset, size_t *iovcnt_offset);


/**
 * Count non-empty entries in the @a iov array
 *
 * @param [in]     iov            @ref ucp_dt_iov_t buffer to count
 * @param [in]     iovcnt         Number of entries in the @a iov buffer
 *
 * @return Number of non-empty entries in the @a iov array
 */
size_t ucp_dt_iov_count_nonempty(const ucp_dt_iov_t *iov, size_t iovcnt);


/**
 * Check that all iov entries match the given memory info
 *
 * @param [in]     context        Context for memory detection
 * @param [in]     iov            @ref ucp_dt_iov_t buffer to check
 * @param [in]     iovcnt         Number of entries in the @a iov buffer
 * @param [in]     mem_info       Compare the iov entries to this memory info
 *
 * @return UCS_OK if all iov entries match the given memory info, otherwise
 *         return UCS_ERR_INVALID_PARAM
 */
ucs_status_t ucp_dt_iov_memtype_check(ucp_context_h context,
                                      const ucp_dt_iov_t *iov, size_t iovcnt,
                                      const ucp_memory_info_t *mem_info);


ucs_status_t ucp_dt_iov_memtype_detect(ucp_context_h context,
                                       const ucp_dt_iov_t *iov, size_t iovcnt,
                                       const ucp_request_param_t *param,
                                       uint8_t *sg_count,
                                       ucp_memory_info_t *mem_info);

#endif
