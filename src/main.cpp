#include "main.h"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_event.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/font/lv_font.h"
#include "liblvgl/lv_conf_internal.h"
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
  lv_color_t color;
  if (arc == motorRPMArc) {
    lv_color_t color =
        lv_palette_main(value > maxValue * 0.90   ? LV_PALETTE_RED
                        : value > maxValue * 0.75 ? LV_PALETTE_DEEP_ORANGE
                        : value > maxValue * 0.5  ? LV_PALETTE_ORANGE
                        : value > maxValue * 0.25 ? LV_PALETTE_YELLOW
                                                  : LV_PALETTE_GREEN);
  }

  else if (arc == motorTempArc) {
    lv_color_t color =
        lv_palette_main(value > maxValue * 0.85   ? LV_PALETTE_RED
                        : value > maxValue * 0.75 ? LV_PALETTE_DEEP_ORANGE
                        : value > maxValue * 0.65 ? LV_PALETTE_ORANGE
                        : value > maxValue * 0.50 ? LV_PALETTE_YELLOW
                        : value > maxValue * 0.35 ? LV_PALETTE_GREEN
                                                  : LV_PALETTE_BLUE);
  }
  lv_obj_set_style_arc_color(arc, color, LV_PART_INDICATOR);
}
string device_Type = "NULL";

int maxRPM = 200;

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

    // MOTOR GEARBOX SLECTION BUTTONS
    /*
    lv_obj_t *GreenMotorGearboxButton = lv_obj_create(lv_scr_act()),
             *RedMotorGearboxButton = lv_obj_create(lv_scr_act()),
             *BlueMotorGearboxButton = lv_obj_create(lv_scr_act());

    lv_obj_align(GreenMotorGearboxButton, LV_ALIGN_TOP_LEFT, 220, 40);
    lv_obj_align(RedMotorGearboxButton, LV_ALIGN_TOP_LEFT, 220, 70);
    lv_obj_align(BlueMotorGearboxButton, LV_ALIGN_TOP_LEFT, 220, 100);

    lv_obj_set_size(GreenMotorGearboxButton, 40, 20);
    lv_obj_set_size(RedMotorGearboxButton, 40, 20);
    lv_obj_set_size(BlueMotorGearboxButton, 40, 20);

    lv_obj_set_style_radius(GreenMotorGearboxButton, 5, 0);
    lv_obj_set_style_radius(RedMotorGearboxButton, 5, 0);
    lv_obj_set_style_radius(BlueMotorGearboxButton, 5, 0);

    lv_obj_add_event_cb(GreenMotorGearboxButton, GreenMotorSelect,
                        LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(RedMotorGearboxButton, RedMotorSelect, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(BlueMotorGearboxButton, BlueMotorSelect,
                        LV_EVENT_CLICKED, NULL);

    lv_obj_set_style_bg_color(GreenMotorGearboxButton,
                              lv_palette_main(LV_PALETTE_GREEN),
                              LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(RedMotorGearboxButton,
                              lv_palette_main(LV_PALETTE_RED),
                              LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(BlueMotorGearboxButton,
                              lv_palette_main(LV_PALETTE_BLUE),
                              LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(GreenMotorGearboxButton,
                              lv_palette_darken(LV_PALETTE_GREEN, 3),
                              LV_STATE_PRESSED);

    lv_obj_set_style_bg_color(RedMotorGearboxButton,
                              lv_palette_darken(LV_PALETTE_RED, 3),
                              LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(BlueMotorGearboxButton,
                              lv_palette_darken(LV_PALETTE_BLUE, 3),
                              LV_STATE_PRESSED);
*/
    motorRPMArc = lv_arc_create(lv_scr_act());
    lv_arc_set_range(motorRPMArc, 0, maxRPM);
    lv_obj_set_size(motorRPMArc, 75, 70);
    lv_obj_align(motorRPMArc, LV_ALIGN_TOP_LEFT, 10, 25);
    lv_arc_set_rotation(motorRPMArc, 180);
    lv_arc_set_bg_angles(motorRPMArc, 0, 180);
    lv_arc_set_mode(motorRPMArc, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(motorRPMArc, LV_OBJ_FLAG_CLICKABLE);

    motorTempArc = lv_arc_create(lv_scr_act());
    lv_arc_set_range(motorTempArc, 300, 350);
    lv_obj_set_size(motorTempArc, 75, 70);
    lv_obj_align(motorTempArc, LV_ALIGN_TOP_LEFT, 10, 75);
    lv_arc_set_rotation(motorTempArc, 180);
    // lv_arc_set_bg_angles(motorTempArc, -25, 205);
    lv_arc_set_bg_angles(motorTempArc, 0, 180);
    lv_arc_set_mode(motorTempArc, LV_ARC_MODE_NORMAL);
    lv_obj_clear_flag(motorTempArc, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_align(SpdUPButton, LV_ALIGN_TOP_LEFT, 320, 40);
    lv_obj_align(SpdRESETButton, LV_ALIGN_TOP_LEFT, 320, 115);
    lv_obj_align(SpdDOWNButton, LV_ALIGN_TOP_LEFT, 320, 160);

    lv_obj_set_size(SpdUPButton, 95, 75);
    lv_obj_set_size(SpdRESETButton, 95, 45);
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
    lv_obj_align(motorLabel, LV_ALIGN_TOP_MID, 0, 25);

    motorRPM = lv_label_create(lv_scr_act());
    lv_label_set_text(motorRPM, "RPM: ");
    lv_obj_align(motorRPM, LV_ALIGN_TOP_LEFT, 25, 35);

    motorTemp = lv_label_create(lv_scr_act());
    lv_label_set_text(motorTemp, "Temp: ");
    lv_obj_align(motorTemp, LV_ALIGN_TOP_LEFT, 25, 95);

    motorPower = lv_label_create(lv_scr_act());
    lv_label_set_text(motorPower, "Power: ");
    lv_obj_align(motorPower, LV_ALIGN_TOP_LEFT, 25, 155);

    motorTor = lv_label_create(lv_scr_act());
    lv_label_set_text(motorTor, "Torque: ");
    lv_obj_align(motorTor, LV_ALIGN_TOP_LEFT, 25, 215);
  } else {
  }
}

// Function to update the motor data
void update_Motor_Data() {
  if (device_Type == "Motor") {
    char buffer[50];
    int rpm = Motor(selectedPort).get_actual_velocity();
    // Get temp in Kelvin
    int temp = (Motor(selectedPort).get_temperature() + 273.15);
    lv_arc_set_value(motorRPMArc, fabs(rpm));
    lv_arc_set_value(motorTempArc, temp);

    update_arc_color(motorRPMArc, fabs(rpm), maxRPM);
    update_arc_color(motorTempArc, temp, 350);
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
static void GreenMotorSelect(lv_event_t *e) {
  maxRPM = 200;
  create_Motor_UI();
}
static void RedMotorSelect(lv_event_t *e) {
  maxRPM = 100;
  create_Motor_UI();
}
static void BlueMotorSelect(lv_event_t *e) {
  maxRPM = 600;
  create_Motor_UI();
} /*
 bool STARTED = false;
 static void Start_Diagnostics_Mode(lv_event_t *e) { STARTED = true; }
 void create_Startup_UI() {
   lv_label_set_text(title, "VEX VITALS");
   lv_obj_set_style_text_font(title, &lv_font_montserrat_30, 0);
   title = lv_label_create(lv_scr_act());
   lv_obj_align(title, LV_ALIGN_TOP_LEFT, 240, 50);

   lv_obj_t *STARTButton;

   lv_obj_align(STARTButton, LV_ALIGN_TOP_LEFT, 240, 180);

   lv_obj_set_size(STARTButton, 200, 50);

   lv_obj_set_style_radius(STARTButton, 20, 0);

   lv_obj_add_event_cb(STARTButton, Start_Diagnostics_Mode, LV_EVENT_RELEASED,
                       NULL);

   lv_obj_set_style_bg_color(STARTButton, lv_palette_main(LV_PALETTE_GREEN),
                             LV_STATE_DEFAULT);

   lv_obj_set_style_bg_color(STARTButton, lv_palette_darken(LV_PALETTE_GREEN,
 4), LV_STATE_PRESSED); while (!STARTED) { delay(20);
   }
 }*/

void initialize() {
  lv_init();
  // create_Startup_UI();
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
