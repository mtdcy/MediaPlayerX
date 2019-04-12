/******************************************************************************
 * Copyright (c) 2016, Chen Fang <mtdcy.chen@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/


// File:    MediaFrame.h
// Author:  mtdcy.chen
// Changes: 
//          1. 20160701     initial version
//

#ifndef _MEDIA_FRAME_H
#define _MEDIA_FRAME_H

#include <MediaFramework/MediaTime.h>
#include <MediaFramework/MediaDefs.h>

__BEGIN_DECLS

#define MEDIA_FRAME_NB_PLANES   (8)

__END_DECLS 

#ifdef __cplusplus
__BEGIN_NAMESPACE_MPX

/**
 * media frame structure for decompressed audio and video frames
 * the properties inside this structure have to make sure this
 * frame can be renderred properly without additional informations.
 */
struct API_EXPORT MediaFrame : public SharedObject {
    MediaTime               pts;        ///< display time in us
    MediaTime               duration;   ///< duration of this frame
    /**
     * plane data struct.
     * for planar frame, multi planes must exist. the backend memory may be
     * or may not be continueslly.
     * for packed frame, only one plane exists.
     */
    struct {
        uint8_t *           data;       ///< plane data
        size_t              size;       ///< data size in bytes
    } planes[MEDIA_FRAME_NB_PLANES];    ///< for packed frame, only one plane exists

    union {
        int32_t             format;     ///< sample format, @see ePixelFormat, @see eSampleFormat
        AudioFormat         a;
        ImageFormat         v;
    };
    void                    *opaque;    ///< opaque

    MediaFrame();
    FORCE_INLINE virtual ~MediaFrame() { }
};

/**
 * create a video frame backend by Buffer
 */
API_EXPORT sp<MediaFrame>  MediaFrameCreate(ePixelFormat format, int32_t width, int32_t height);
API_EXPORT sp<MediaFrame>   MediaFrameCreate(const ImageFormat& );

/**
 * create a audio frame backend by Buffer
 */
API_EXPORT sp<MediaFrame>  MediaFrameCreate(const AudioFormat& );

__END_NAMESPACE_MPX
#endif

#ifdef __cplusplus
typedef struct mpx::MediaFrame *     MediaFrameRef;
#else
typedef void *  MediaFrameRef;
#endif

__BEGIN_DECLS

MediaFrameRef   AudioFrameCreate(const AudioFormat *);
MediaFrameRef   ImageFrameCreate(const ImageFormat *);

uint8_t *       MediaFrameGetPlaneData(MediaFrameRef, size_t);
size_t          MediaFrameGetPlaneSize(MediaFrameRef, size_t);

AudioFormat *   MediaFrameGetAudioFormat(MediaFrameRef);
ImageFormat *   MediaFrameGetImageFormat(MediaFrameRef);

void *          MediaFrameGetOpaque(MediaFrameRef);

__END_DECLS

#endif // _MEDIA_FRAME_H 