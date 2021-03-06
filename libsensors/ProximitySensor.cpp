/*
 * Copyright (C) 2012 The Android Open-Source Project
 * Copyright (C) 2014 Rudolf Tammekivi <rtammekivi@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define PROX_DEBUG 0

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "ProximitySensor.h"

/*****************************************************************************/

ProximitySensor::ProximitySensor()
    : SensorBase(NULL, APDS9900_PROX_NAME),
      mInputReader(4), mEnabled(0), mHasPendingEvent(false)
{
    ALOGD_IF(PROX_DEBUG, "ProximitySensor: Initializing...");

    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_P;
    mPendingEvent.type = SENSOR_TYPE_PROXIMITY;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));
}

ProximitySensor::~ProximitySensor() {
}

int ProximitySensor::setEnable(int32_t handle, int enabled)
{
    ALOGD_IF(PROX_DEBUG, "ProximitySensor: enable %d %d", handle, enabled);

    if (mEnabled == enabled)
        return 0;

    char buf[2];
    snprintf(buf, sizeof(buf), "%d", !!enabled);
    int ret = write_sys_attribute(APDS9900_SYSFS_PATH "enable_ps_sensor", buf, sizeof(buf));
    if (ret)
        return ret;

    mEnabled = enabled;
    return 0;
}

int ProximitySensor::setDelay(int32_t handle, int64_t ns)
{
    ALOGD_IF(PROX_DEBUG, "ProximitySensor: setDelay %d %lld", handle, ns);

    if (!mEnabled)
        return 0;

    return 0;
}

bool ProximitySensor::hasPendingEvents() const
{
    return mHasPendingEvent;
}

int ProximitySensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent)
    {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event))
    {
        int type = event->type;
        if (type == EV_ABS)
        {
            if (event->code == EVENT_TYPE_PROXIMITY)
                mPendingEvent.distance = event->value;
        }
        else if (type == EV_SYN)
        {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled)
            {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("ProximitySensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}
