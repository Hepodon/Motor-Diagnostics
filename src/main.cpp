#include "main.h"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_event.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/widgets/lv_label.h"
#include "pros/device.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

using namespace pros;
using namespace std;

Controller conInput(E_CONTROLLER_MASTER);

int speed = 0;
int selectedPort = 1;
lv_obj_t *title, *motorLabel, *motorRPM, *motorTemp, *motorPower, *motorTor;

static void MotorSpeedUp(lv_event_t *e) { speed++; }
static void MotorSpeedDown(lv_event_t *e) { speed--; }
static void MotorSpeedReset(lv_event_t *e) { speed = 0; }

lv_obj_t *motorRPMArc, *motorTempArc;

void update_arc_color(lv_obj_t *arc, int value, int maxValue) {
  lv_color_t color =
      lv_palette_main(value > maxValue * 0.75   ? LV_PALETTE_RED
                      : value > maxValue * 0.5  ? LV_PALETTE_ORANGE
                      : value > maxValue * 0.25 ? LV_PALETTE_YELLOW
                                                : LV_PALETTE_GREEN);
  lv_obj_set_style_arc_color(arc, color, LV_PART_INDICATOR);
}
string device_Type = "NULL";

// Function to create Port Selection and detect motor ports
void detect_Port_Type(int portNum) {
  if (Device::get_plugged_type(portNum) == DeviceType::motor) {
    device_Type = "Motor";
  } else if (Device::get_plugged_type(portNum) == DeviceType::imu) {
    device_Type = "Imu";
  }
}

// Function to create the UI for the selected motor
void create_Motor_UI() {
  lv_obj_clean(lv_scr_act()); // Clear the screen before updating

  lv_obj_t *LeftPortButton = lv_obj_create(lv_scr_act()),
           *RightPortButton = lv_obj_create(lv_scr_act());
  lv_obj_t *label;
  lv_obj_align(LeftPortButton, LV_ALIGN_TOP_LEFT, 200, 180);
  lv_obj_align(RightPortButton, LV_ALIGN_TOP_LEFT, 260, 180);
  lv_obj_set_size(LeftPortButton, 40, 55);
  lv_obj_set_size(RightPortButton, 40, 55);
  lv_obj_set_style_radius(LeftPortButton, 10, 0);
  lv_obj_set_style_radius(RightPortButton, 10, 0);
  lv_obj_add_event_cb(LeftPortButton, PortSelectLeft, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(RightPortButton, PortSelectRight, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(LeftPortButton,
                            lv_palette_main(LV_PALETTE_LIGHT_GREEN),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(RightPortButton,
                            lv_palette_main(LV_PALETTE_LIGHT_GREEN),
                            LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(LeftPortButton,
                            lv_palette_darken(LV_PALETTE_LIGHT_GREEN, 5),
                            LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(RightPortButton,
                            lv_palette_darken(LV_PALETTE_LIGHT_GREEN, 5),
                            LV_STATE_PRESSED);

  detect_Port_Type(selectedPort);

  if (device_Type == "Motor") {
    lv_obj_t *SpdUPButton = lv_obj_create(lv_scr_act()),
             *SpdDOWNButton = lv_obj_create(lv_scr_act()),
             *SpdRESETButton = lv_obj_create(lv_scr_act());
    motorRPMArc = lv_arc_create(lv_scr_act());
    lv_arc_set_range(motorRPMArc, 0, 200);
    lv_obj_set_size(motorRPMArc, 75, 70);
    lv_obj_align(motorRPMArc, LV_ALIGN_TOP_LEFT, 10, 25);
    lv_arc_set_rotation(motorRPMArc, 180);
    lv_arc_set_bg_angles(motorRPMArc, 0, 180);
    lv_arc_set_mode(motorRPMArc, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(motorRPMArc, LV_OBJ_FLAG_CLICKABLE);

    motorTempArc = lv_arc_create(lv_scr_act());
    lv_arc_set_range(motorTempArc, 0, 50);
    lv_obj_set_size(motorTempArc, 75, 70);
    lv_obj_align(motorTempArc, LV_ALIGN_TOP_RIGHT, 10, 75);
    lv_arc_set_rotation(motorTempArc, 180);
    lv_arc_set_bg_angles(motorTempArc, 0, 180);
    lv_arc_set_mode(motorTempArc, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(motorTempArc, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_align(SpdUPButton, LV_ALIGN_TOP_LEFT, 320, 40);
    lv_obj_align(SpdRESETButton, LV_ALIGN_TOP_LEFT, 320, 115);
    lv_obj_align(SpdDOWNButton, LV_ALIGN_TOP_LEFT, 320, 160);

    lv_obj_set_size(SpdUPButton, 95, 75);
    lv_obj_set_size(SpdRESETButton, 95, 40);
    lv_obj_set_size(SpdDOWNButton, 95, 75);

    lv_obj_set_style_radius(SpdUPButton, 15, 0);
    lv_obj_set_style_radius(SpdRESETButton, 15, 0);
    lv_obj_set_style_radius(SpdDOWNButton, 15, 0);

    lv_obj_add_event_cb(SpdRESETButton, MotorSpeedReset, LV_EVENT_PRESSING,
                        NULL);
    lv_obj_add_event_cb(SpdUPButton, MotorSpeedUp, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(SpdDOWNButton, MotorSpeedDown, LV_EVENT_PRESSING, NULL);

    lv_obj_set_style_bg_color(
        SpdUPButton, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(SpdDOWNButton, lv_palette_main(LV_PALETTE_BLUE),
                              LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(
        SpdUPButton, lv_palette_darken(LV_PALETTE_BLUE, 3), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(
        SpdDOWNButton, lv_palette_darken(LV_PALETTE_BLUE, 3), LV_STATE_PRESSED);

    lv_obj_set_style_bg_color(SpdRESETButton, lv_palette_main(LV_PALETTE_RED),
                              LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(
        SpdRESETButton, lv_palette_main(LV_PALETTE_YELLOW), LV_STATE_PRESSED);

    label = lv_label_create(SpdUPButton);
    lv_label_set_text(label, "UP");
    lv_obj_center(label);
    label = lv_label_create(SpdDOWNButton);
    lv_label_set_text(label, "DOWN");
    lv_obj_center(label);
    label = lv_label_create(SpdRESETButton);
    lv_label_set_text(label, "STOP");
    lv_obj_center(label);

    title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "Motor Status");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    motorLabel = lv_label_create(lv_scr_act());
    string motorText = "Port " + to_string(selectedPort);
    lv_label_set_text(motorLabel, motorText.c_str());
    lv_obj_align(motorLabel, LV_ALIGN_TOP_MID, 0, 40);

    motorRPM = lv_label_create(lv_scr_act());
    lv_label_set_text(motorRPM, "RPM: ");
    lv_obj_align(motorRPM, LV_ALIGN_TOP_LEFT, 10, 80);

    motorTemp = lv_label_create(lv_scr_act());
    lv_label_set_text(motorTemp, "Temp: ");
    lv_obj_align(motorTemp, LV_ALIGN_TOP_LEFT, 10, 120);

    motorPower = lv_label_create(lv_scr_act());
    lv_label_set_text(motorPower, "Power: ");
    lv_obj_align(motorPower, LV_ALIGN_TOP_LEFT, 10, 160);

    motorTor = lv_label_create(lv_scr_act());
    lv_label_set_text(motorTor, "Torque: ");
    lv_obj_align(motorTor, LV_ALIGN_TOP_LEFT, 10, 200);
  } else {
  }
}

// Function to update the motor data
void update_Motor_Data() {
  if (device_Type == "Motor") {
    char buffer[50];
    int rpm = Motor(selectedPort).get_actual_velocity();
    int temp = (Motor(selectedPort).get_temperature() + 273);
    lv_arc_set_value(motorRPMArc, fabs(rpm));
    lv_arc_set_value(motorTempArc, temp-290);

    update_arc_color(motorRPMArc, fabs(rpm), 200);
    update_arc_color(motorTempArc, (temp - 290), 50);
    sprintf(buffer, "RPM: %.1d", rpm);
    lv_label_set_text(motorRPM, buffer);

    sprintf(buffer, "Temp: %.1dK", temp);
    lv_label_set_text(motorTemp, buffer);

    sprintf(buffer, "Power: %.1fW", Motor(selectedPort).get_power());
    lv_label_set_text(motorPower, buffer);

    sprintf(buffer, "Torque: %.1f%%", Motor(selectedPort).get_torque());
    lv_label_set_text(motorTor, buffer);
  }
}
static void PortSelectLeft(lv_event_t *e) {
  speed = 0;
  Motor(selectedPort).brake();
  selectedPort = (selectedPort > 1) ? selectedPort - 1 : 20;
  create_Motor_UI();
}
static void PortSelectRight(lv_event_t *e) {
  speed = 0;
  Motor(selectedPort).brake();
  selectedPort = (selectedPort < 20) ? selectedPort + 1 : 1;
  create_Motor_UI();
}

void initialize() {
  // create_Startup_UI();
  lv_init();
  create_Motor_UI();
}

void opcontrol() {
  while (true) {
    update_Motor_Data();

    // if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)) {
    //   speed = 0;
    //   Motor(selectedPort).brake();

    //   selectedPort = (selectedPort < 20) ? selectedPort + 1 : 1;
    //   create_Motor_UI();
    // }

    // if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) {
    //   speed = 0;
    //   Motor(selectedPort).brake();
    //   selectedPort = (selectedPort > 1) ? selectedPort - 1 : 20;
    //   create_Motor_UI();
    // }
    Motor(selectedPort).move(speed);

    delay(50);
  }
}
