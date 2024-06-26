#ifndef __V_SENSOR_H__
#define __V_SENSOR_H__

#include <linux/device.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-subdev.h>


enum vsensor_frame_rate {
    VSENSOR_15_FPS = 0,
    VSENSOR_30_FPS,
    VSENSOR_60_FPS,
    VSENSOR_NUM_FRAMERATES,
};


typedef struct virtual_sensor_dev{
    struct v4l2_subdev sd;
    struct media_pad pad;
    struct mutex lock;
    enum vsensor_frame_rate current_fr;
}virtual_sensor_dev;


#endif /* __V_SENSOR_H__ */
