/*
 * Copyright (c) 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include "Rsocket.h"
#include "jni_util.h"
#include "jvm.h"
#include "nio.h"
#include "sun_nio_ch_RdmaPollArrayWrapper.h"


JNIEXPORT jint JNICALL
Java_sun_nio_ch_RdmaPollArrayWrapper_poll0(JNIEnv *env,
        jclass clazz, jlong address, jint numfds, jlong timeout) {
    struct pollfd *a;
    int res;

    a = (struct pollfd *) jlong_to_ptr(address);
    res = rs_poll(a, numfds, timeout);
    if (res < 0) {
        if (errno == EINTR) {
            return IOS_INTERRUPTED;
        } else {
            JNU_ThrowIOExceptionWithLastError(env, "poll failed");
            return IOS_THROWN;
        }
    }
    return (jint) res;
}


JNIEXPORT void JNICALL
Java_sun_nio_ch_RdmaPollArrayWrapper_interrupt(JNIEnv *env, jobject this, jint fd)
{
    int fakebuf[1];
    fakebuf[0] = 1;
    if (write(fd, fakebuf, 1) < 0) {
         JNU_ThrowIOExceptionWithLastError(env,
                                          "Write to interrupt fd failed");
    }
}