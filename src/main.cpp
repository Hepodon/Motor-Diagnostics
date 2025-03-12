#include "main.h"
#include "liblvgl/core/lv_obj.h"
#include "pros/misc.h"

using namespace pros;
using namespace std;

Controller conInput(E_CONTROLLER_MASTER);
Motor P1Motor(1);
Motor P2Motor(2);
Motor P3Motor(3);
Motor P4Motor(4);
Motor P5Motor(5);
Motor P6Motor(6);
Motor P7Motor(7);
Motor P8Motor(8);
Motor P9Motor(9);
Motor P10Motor(10);
Motor P11Motor(11);
Motor P12Motor(12);
Motor P13Motor(13);
Motor P14Motor(14);
Motor P15Motor(15);
Motor P16Motor(16);
Motor P17Motor(17);
Motor P18Motor(18);
Motor P19Motor(19);
Motor P20Motor(20);

lv_obj_t *P1Motor_Label, *P2Motor_Label, *P3Motor_Label, *P4Motor_Label,
    *P5Motor_Label, *P6Motor_Label, *P7Motor_Label, *P8Motor_Label,
    *P9Motor_Label, *P10Motor_Label, *P11Motor_Label, *P12Motor_Label,
    *P13Motor_Label, *P14Motor_Label, *P15Motor_Label, *P16Motor_Label,
    *P17Motor_Label, *P18Motor_Label, *P19Motor_Label, *P20Motor_Label;
lv_obj_t *P1Motor_RPM, *P2Motor_RPM, *P3Motor_RPM, *P4Motor_RPM, *P5Motor_RPM,
    *P6Motor_RPM, *P7Motor_RPM, *P8Motor_RPM, *P9Motor_RPM, *P10Motor_RPM,
    *P11Motor_RPM, *P12Motor_RPM, *P13Motor_RPM, *P14Motor_RPM, *P15Motor_RPM,
    *P16Motor_RPM, *P17Motor_RPM, *P18Motor_RPM, *P19Motor_RPM, *P20Motor_RPM;
lv_obj_t *P1Motor_Temp, *P2Motor_Temp, *P3Motor_Temp, *P4Motor_Temp,
    *P5Motor_Temp, *P6Motor_Temp, *P7Motor_Temp, *P8Motor_Temp, *P9Motor_Temp,
    *P10Motor_Temp, *P11Motor_Temp, *P12Motor_Temp, *P13Motor_Temp,
    *P14Motor_Temp, *P15Motor_Temp, *P16Motor_Temp, *P17Motor_Temp,
    *P18Motor_Temp, *P19Motor_Temp, *P20Motor_Temp;

void create_Motor_UI(int Port) {
  lv_obj_t *title = lv_label_create(lv_scr_act());
  lv_label_set_text(title, "Motor Status");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
  if (Port == 1) {
    P1Motor_Label = lv_label_create(lv_scr_act());
    lv_label_set_text(P1Motor_Label, "Port 1");
    lv_obj_align(P1Motor_Label, LV_ALIGN_TOP_LEFT, 10, 50);
    P1Motor_RPM = lv_label_create(lv_scr_act());
    lv_label_set_text(P1Motor_RPM, "RPM: ");
    lv_obj_align(P1Motor_RPM, LV_ALIGN_TOP_LEFT, 10, 100);
    P1Motor_Temp = lv_label_create(lv_scr_act());
    lv_label_set_text(P1Motor_Temp, "Temp: ");
    lv_obj_align(P1Motor_Temp, LV_ALIGN_TOP_LEFT, 10, 150);
  }
  if (Port == 2) {
    P2Motor_Label = lv_label_create(lv_scr_act());
    lv_label_set_text(P2Motor_Label, "Port 2");
    lv_obj_align(P2Motor_Label, LV_ALIGN_TOP_LEFT, 10, 50);
    P2Motor_RPM = lv_label_create(lv_scr_act());
    lv_label_set_text(P2Motor_RPM, "RPM: ");
    lv_obj_align(P2Motor_RPM, LV_ALIGN_TOP_LEFT, 10, 100);
    P2Motor_Temp = lv_label_create(lv_scr_act());
    lv_label_set_text(P2Motor_Temp, "Temp: ");
    lv_obj_align(P2Motor_Temp, LV_ALIGN_TOP_LEFT, 10, 150);
  }
  if (Port == 3) {
	P3Motor_Label = lv_label_create(lv_scr_act());
	lv_label_set_text(P3Motor_Label, "Port 3");
	lv_obj_align(P3Motor_Label, LV_ALIGN_TOP_LEFT, 10, 50);
	P3Motor_RPM = lv_label_create(lv_scr_act());
	lv_label_set_text(P3Motor_RPM, "RPM: ");
	lv_obj_align(P3Motor_RPM, LV_ALIGN_TOP_LEFT, 10, 100);
	P3Motor_Temp = lv_label_create(lv_scr_act());
	lv_label_set_text(P3Motor_Temp, "Temp: ");
	lv_obj_align(P3Motor_Temp, LV_ALIGN_TOP_LEFT, 10, 150);
  }
  if (Port == 4) {
	P4Motor_Label = lv_label_create(lv_scr_act());
	lv_label_set_text(P4Motor_Label, "Port 4");
	lv_obj_align(P4Motor_Label, LV_ALIGN_TOP_LEFT, 10, 50);
	P4Motor_RPM = lv_label_create(lv_scr_act());
	lv_label_set_text(P4Motor_RPM, "RPM: ");
	lv_obj_align(P4Motor_RPM, LV_ALIGN_TOP_LEFT, 10, 100);
	P4Motor_Temp = lv_label_create(lv_scr_act());
	lv_label_set_text(P4Motor_Temp, "Temp: ");
	lv_obj_align(P4Motor_Temp, LV_ALIGN_TOP_LEFT, 10, 150);
  }
  if (Port == 5) {
	P5Motor_Label = lv_label_create(lv_scr_act());
	lv_label_set_text(P5Motor_Label, "Port 5");
	lv_obj_align(P5Motor_Label, LV_ALIGN_TOP_LEFT, 10, 50);
	P5Motor_RPM = lv_label_create(lv_scr_act());
	lv_label_set_text(P5Motor_RPM, "RPM: ");
	lv_obj_align(P5Motor_RPM, LV_ALIGN_TOP_LEFT, 10, 100);
	P5Motor_Temp = lv_label_create(lv_scr_act());
	lv_label_set_text(P5Motor_Temp, "Temp: ");
	lv_obj_align(P5Motor_Temp, LV_ALIGN_TOP_LEFT, 10, 150);
  }
}

void initialize() { lv_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  while (true) {
    int selectedPort = 1;
    create_Motor_UI(selectedPort);
    lv_task_handler();
    if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)) {
      selectedPort++;
      if (selectedPort > 20) {
        selectedPort = 20;
      }
    }
    if (conInput.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) {
      selectedPort--;
      if (selectedPort < 1) {
        selectedPort = 1;
      }
    }
    delay(10);
  }
}