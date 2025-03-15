#include "main.h"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_event.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/widgets/lv_label.h"
#include "pros/misc.h"

using namespace pros;
using namespace std;

Controller conInput(E_CONTROLLER_MASTER);
Motor motors[20] = {Motor(1),  Motor(2),  Motor(3),  Motor(4),  Motor(5),
                    Motor(6),  Motor(7),  Motor(8),  Motor(9),  Motor(10),
                    Motor(11), Motor(12), Motor(13), Motor(14), Motor(15),
                    Motor(16), Motor(17), Motor(18), Motor(19), Motor(20)};

int selectedPort = 1;
lv_obj_t *title, *motorLabel, *motorRPM, *motorTemp, *motorPower, *motorEff;
int speed = 0;
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
// Function to create the UI for the selected motor
void create_Motor_UI() {

  lv_obj_clean(lv_scr_act()); // Clear the screen before updating

  // Check if motor is connected by verifying temperature reading
  if (motors[selectedPort - 1].get_temperature() == PROS_ERR_F) {
    lv_label_set_text(
        motorLabel,
        ("Port " + to_string(selectedPort) + ": No motor connected").c_str());
    return;
  }
  motorRPMArc = lv_arc_create(lv_scr_act());
  lv_arc_set_range(motorRPMArc, 0, 200);
  lv_obj_set_size(motorRPMArc, 120, 120);
  lv_obj_align(motorRPMArc, LV_ALIGN_LEFT_MID, 40, 0);
  lv_arc_set_rotation(motorRPMArc, 270);
  lv_arc_set_bg_angles(motorRPMArc, 180, 360);
  lv_arc_set_mode(motorRPMArc, LV_ARC_MODE_NORMAL);
  lv_obj_clear_flag(motorRPMArc, LV_OBJ_FLAG_CLICKABLE);

  motorTempArc = lv_arc_create(lv_scr_act());
  lv_arc_set_range(motorTempArc, 0, 100);
  lv_obj_set_size(motorTempArc, 120, 120);
  lv_obj_align(motorTempArc, LV_ALIGN_RIGHT_MID, -40, 0);
  lv_arc_set_rotation(motorTempArc, 270);
  lv_arc_set_bg_angles(motorTempArc, 180, 360);
  lv_arc_set_mode(motorTempArc, LV_ARC_MODE_NORMAL);
  lv_obj_clear_flag(motorTempArc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_t *label;
  lv_obj_t *SpdUPButton = lv_obj_create(lv_scr_act()),
           *SpdDOWNButton = lv_obj_create(lv_scr_act());
  lv_obj_t *SpdRESETButton = lv_obj_create(lv_scr_act());

  lv_obj_align(SpdUPButton, LV_ALIGN_TOP_LEFT, 320, 40);
  lv_obj_align(SpdRESETButton, LV_ALIGN_TOP_LEFT, 320, 120);
  lv_obj_align(SpdDOWNButton, LV_ALIGN_TOP_LEFT, 320, 160);

  lv_obj_set_size(SpdUPButton, 95, 75);
  lv_obj_set_size(SpdRESETButton, 95, 35);
  lv_obj_set_size(SpdDOWNButton, 95, 75);

  lv_obj_set_style_radius(SpdUPButton, 15, 0);
  lv_obj_set_style_radius(SpdRESETButton, 15, 0);
  lv_obj_set_style_radius(SpdDOWNButton, 15, 0);

  lv_obj_add_event_cb(SpdRESETButton, MotorSpeedReset, LV_EVENT_PRESSING, NULL);
  lv_obj_add_event_cb(SpdUPButton, MotorSpeedUp, LV_EVENT_PRESSING, NULL);
  lv_obj_add_event_cb(SpdDOWNButton, MotorSpeedDown, LV_EVENT_PRESSING, NULL);

  lv_obj_set_style_bg_color(SpdUPButton, lv_palette_main(LV_PALETTE_LIGHT_BLUE),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(SpdDOWNButton, lv_palette_main(LV_PALETTE_BLUE),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(SpdUPButton, lv_palette_darken(LV_PALETTE_BLUE, 3),
                            LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(
      SpdDOWNButton, lv_palette_darken(LV_PALETTE_BLUE, 3), LV_STATE_PRESSED);
  lv_obj_set_style_bg_color(SpdRESETButton, lv_palette_main(LV_PALETTE_RED),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(SpdRESETButton, lv_palette_main(LV_PALETTE_YELLOW),
                            LV_STATE_PRESSED);

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
  /*
    motorRPM = lv_label_create(lv_scr_act());
    lv_label_set_text(motorRPM, "RPM: ");
    lv_obj_align(motorRPM, LV_ALIGN_TOP_LEFT, 10, 80);

    motorTemp = lv_label_create(lv_scr_act());
    lv_label_set_text(motorTemp, "Temp: ");
    lv_obj_align(motorTemp, LV_ALIGN_TOP_LEFT, 10, 120);
  */
  motorPower = lv_label_create(lv_scr_act());
  lv_label_set_text(motorPower, "Power: ");
  lv_obj_align(motorPower, LV_ALIGN_TOP_LEFT, 10, 160);

  motorEff = lv_label_create(lv_scr_act());
  lv_label_set_text(motorEff, "Efficiency: ");
  lv_obj_align(motorEff, LV_ALIGN_TOP_LEFT, 10, 200);
}

// Function to update the motor data
void update_Motor_Data() {
  if (motors[selectedPort - 1].get_temperature() == PROS_ERR_F)
    return;

  char buffer[50];
  int rpm = motors[selectedPort - 1].get_actual_velocity();
  int temp = motors[selectedPort - 1].get_temperature();
  lv_arc_set_value(motorRPMArc, rpm);
  lv_arc_set_value(motorTempArc, temp);

  update_arc_color(motorRPMArc, rpm, 200);
  update_arc_color(motorTempArc, temp, 100);
  sprintf(buffer, "RPM: %.1f", motors[selectedPort - 1].get_actual_velocity());
  lv_label_set_text(motorRPM, buffer);

  sprintf(buffer, "Temp: %.1fC", motors[selectedPort - 1].get_temperature());
  lv_label_set_text(motorTemp, buffer);

  sprintf(buffer, "Power: %.1fW", motors[selectedPort - 1].get_power());
  lv_label_set_text(motorPower, buffer);

  sprintf(buffer, "Efficiency: %.1f%%",
          motors[selectedPort - 1].get_efficiency());
  lv_label_set_text(motorEff, buffer);
}

void initialize() {
  lv_init();
  create_Motor_UI();
}

void opcontrol() {
  while (true) {
    update_Motor_Data();
    lv_task_handler();

    if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)) {
      speed = 0;
      motors[selectedPort].brake();
      selectedPort = (selectedPort < 20) ? selectedPort + 1 : 1;
      create_Motor_UI();
    }

    if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) {
      speed = 0;
      motors[selectedPort].brake();
      selectedPort = (selectedPort > 1) ? selectedPort - 1 : 20;
      create_Motor_UI();
    }
    motors[selectedPort].move(speed);

    pros::delay(50);
  }
}
