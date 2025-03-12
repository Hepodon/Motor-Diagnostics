#include "main.h"
#include "liblvgl/core/lv_obj.h"
#include "pros/misc.h"

using namespace pros;

Controller conInput(E_CONTROLLER_MASTER);
Motor motors[20] = {Motor(1),  Motor(2),  Motor(3),  Motor(4),  Motor(5),
                    Motor(6),  Motor(7),  Motor(8),  Motor(9),  Motor(10),
                    Motor(11), Motor(12), Motor(13), Motor(14), Motor(15),
                    Motor(16), Motor(17), Motor(18), Motor(19), Motor(20)};

int selectedPort = 1;
lv_obj_t *title, *motorLabel, *motorRPM, *motorTemp, *motorPower, *motorEff;

// Function to create the UI for the selected motor
void create_Motor_UI() {
  lv_obj_clean(lv_scr_act()); // Clear the screen before updating

  title = lv_label_create(lv_scr_act());
  lv_label_set_text(title, "Motor Status");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

  motorLabel = lv_label_create(lv_scr_act());
  std::string motorText = "Port " + std::to_string(selectedPort);
  lv_label_set_text(motorLabel, motorText.c_str());
  lv_obj_align(motorLabel, LV_ALIGN_TOP_MID, 0, 40);

  // Check if motor is connected by verifying temperature reading
  if (motors[selectedPort - 1].get_temperature() == PROS_ERR_F) {
    lv_label_set_text(motorLabel, ("Port " + std::to_string(selectedPort) +
                                   ": No motor connected")
                                      .c_str());
    return;
  }

  motorRPM = lv_label_create(lv_scr_act());
  lv_label_set_text(motorRPM, "RPM: ");
  lv_obj_align(motorRPM, LV_ALIGN_TOP_LEFT, 10, 80);

  motorTemp = lv_label_create(lv_scr_act());
  lv_label_set_text(motorTemp, "Temp: ");
  lv_obj_align(motorTemp, LV_ALIGN_TOP_LEFT, 10, 120);

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
      selectedPort = (selectedPort < 20) ? selectedPort + 1 : 1;
      create_Motor_UI();
    }

    if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) {
      selectedPort = (selectedPort > 1) ? selectedPort - 1 : 20;
      create_Motor_UI();
    }

    pros::delay(100);
  }
}
