#include "vsensor.h"

extern unsigned char red[8230];
extern unsigned char blue[8267];
extern unsigned char green[8265];

static const struct v4l2_subdev_core_ops vsensor_core_ops = {
    .s_power = NULL,
    .log_status = v4l2_ctrl_subdev_log_status,
    .subscribe_event = v4l2_ctrl_subdev_subscribe_event,
    .unsubscribe_event = v4l2_event_subdev_unsubscribe,
};

static const struct v4l2_subdev_video_ops vsensor_video_ops = {
    .g_frame_interval = vsensor_g_frame_interval,
    .s_frame_interval = vsensor_s_frame_interval,
    .s_stream = vsensor_s_stream,
};

static const struct v4l2_subdev_pad_ops vsensor_pad_ops = {
    .get_fmt = vsensor_get_fmt,
    .set_fmt = vsensor_set_fmt,
    .enum_frame_size = vsensor_enum_frame_size,
    .enum_frame_interval = vsensor_enum_frame_interval,
};

static const struct v4l2_subdev_ops vsensor_subdev_ops = {
    .core = &vsensor_core_ops,
    .video = &vsensor_video_ops,
    .pad = &vsensor_pad_ops,
};


static int vsensor_v4l_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    virtual_sensor_dev *vsensor_dev;
    int ret;

    vsensor_dev = devm_kzalloc(dev, sizeof(*vsensor_dev), GFP_KERNEL);
    if (!vsensor_dev)
        return -ENOMEM;

    vsensor_dev->current_fr = VSENSOR_30_FPS;

    v4l2_subdev_init(&vsensor_dev->sd, &vsensor_subdev_ops);
    vsensor_dev->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE |
                             V4L2_SUBDEV_FL_HAS_EVENTS;
    vsensor_dev->pad.flags = MEDIA_PAD_FL_SOURCE;
    vsensor_dev->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;
    ret = media_entity_pads_init(&vsensor_dev->sd.entity, 1, &vsensor_dev->pad);
    if (ret)
        return ret;

    mutex_init(&vsensor_dev->lock);

    v4l2_async_register_subdev(&vsensor_dev->sd);

    platform_set_drvdata(pdev, vsensor_dev);
    printk("Virtual Sensor driver probed done!\n");

    return 0;
}

static int vsensor_v4l_remove(struct platform_device *pdev)
{
    virtual_sensor_dev *vsensor_dev = platform_get_drvdata(pdev);

    v4l2_async_unregister_subdev(&vsensor_dev->sd);
    mutex_destroy(&vsensor_dev->lock);
    media_entity_cleanup(&vsensor_dev->sd.entity);

    printk("Virtual Sensor driver remove done!\n");
    return 0;
}

static const struct of_device_id vsensor_of_match[] = {
    {.compatible = "virtual,sensor"},
    {},
};

MODULE_DEVICE_TABLE(of, vsensor_of_match);

static struct platform_driver vsensor_driver = {
    .probe = vsensor_v4l_probe,
    .remove = vsensor_v4l_remove,
    .driver = {
        .name = "virtual,sensor",
        .owner = THIS_MODULE,
        .of_match_table = vsensor_of_match,
    }};

static int __init vsensor_init(void)
{
    return platform_driver_register(&vsensor_driver);
}

static void __exit vsensor_exit(void)
{
    platform_driver_unregister(&vsensor_driver);
}

module_init(vsensor_init);
module_exit(vsensor_exit);

MODULE_DESCRIPTION("Virtual Sensor Driver");
MODULE_AUTHOR("Jiamuyu");
MODULE_LICENSE("GPL");
MODULE_ALIAS("Virtual-Sensor");
