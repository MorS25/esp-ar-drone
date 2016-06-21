// AR Drone states
#define emergency_mask   0x80000000
#define watch_mask       0x40000000
#define adc_mask         0x20000000
#define ctrl_mask        0x10000000
#define acq_mask         0x08000000
#define videoThread_mask 0x04000000
#define navdata_mask     0x02000000
#define AT_mask          0x01000000
#define PIC_mask         0x00800000
#define cutout_mask      0x00400000
#define ultra_mask       0x00200000
#define wind_mask        0x00100000
#define angl_mask        0x00080000
#define magneto_mask     0x00040000
#define timer_mask       0x00020000
#define userEmer_mask    0x00010000
#define batLow_mask      0x00008000
#define comLost_mask     0x00002000
#define motor_mask       0x00001000
#define navBoot_mask     0x00000800
#define navDemo_mask     0x00000400
#define USB_mask         0x00000200
#define travel_mask      0x00000100
#define cam_mask         0x00000080
#define conAck_mask      0x00000040
#define user_mask        0x00000020
#define alti_mask        0x00000010
#define control_mask     0x00000008
#define vision_mask      0x00000004
#define video_mask       0x00000002
#define fly_mask         0x00000001

// ESP status masks
#define ESP_CONNECTED   0x1
#define ESP_FLYING      0x2
#define ESP_LOWBAT      0x4
#define ESP_EMERGENCY   0x8

// PGSensorHub definitions
#define BUTTON_SELECT   "S"
#define BUTTON_START    "P"
#define BUTTON_PROGRAM  "M"
#define BUTTON_ENTER    "X"
#define BUTTON_CENTER   "C"
#define BUTTON_NONE     "-"

#define POSTURE_RELAXED "R"
#define POSTURE_POINT   "P"
#define POSTURE_FIST    "F"
#define POSTURE_THUMB   "T"
#define POSTURE_BIRD    "B"
#define POSTURE_PEACE   "Y"
#define POSTURE_GUN     "G"

#define MODE_SETUP      0
#define MODE_NORMAL     1
