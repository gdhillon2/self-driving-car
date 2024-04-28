#include "MotorTest.h"

void testIndividualMotor(motor_info *motor) {

  Run_Motor(motor);

  sleep(4);

  Set_Direction(motor, BACKWARD);
  Run_Motor(motor);
  sleep(4);

  Stop_Motor(motor);
}

void testMotors(motor_info *motor_a, motor_info *motor_b, motor_info *motor_c,
                motor_info *motor_d) {

  Shift_Left(motor_a, motor_b, motor_c, motor_d);

  Stop_Motor(motor_a);
  Stop_Motor(motor_b);
  Stop_Motor(motor_c);
  Stop_Motor(motor_d);
}

void testIndividualHat(uint8_t motorhat, motor_info *motor_a,
                       motor_info *motor_b) {
  Init_Motorhat(motorhat);

  Run_Motor(motor_a);
  Run_Motor(motor_b);

  sleep(5);

  Stop_Motor(motor_a);
  Stop_Motor(motor_b);
  DEV_ModuleExit();
}

void testBothHats(motor_info *motor_a, motor_info *motor_b) {
  // init and begin running the motors on motorhat 1 (0x40)
  Init_Motorhat(MOTORHAT_1);
  Run_Motor(motor_a);
  Run_Motor(motor_b);

  // init and begin running the motors on motorhat 2 (0x51)
  Init_Motorhat(MOTORHAT_2);
  Run_Motor(motor_a);
  Run_Motor(motor_b);

  sleep(5);

  // stop the motors on motorhat 2 (0x51)
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  // init and stop the motors on motorhat 1 (0x40)
  Init_Motorhat(MOTORHAT_1);
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  // both motors are stopped at this point

  Switch_Direction(motor_a);
  Switch_Direction(motor_b);
  Run_Motor(motor_a);
  Run_Motor(motor_b);

  // motor hat 1 is running in the opposite direction at this point

  Init_Motorhat(MOTORHAT_2);

  Run_Motor(motor_a);
  Run_Motor(motor_b);

  sleep(5);

  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  Init_Motorhat(MOTORHAT_1);
  Stop_Motor(motor_a);
  Stop_Motor(motor_b);

  DEV_ModuleExit();
}
