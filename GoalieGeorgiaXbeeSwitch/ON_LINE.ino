void getInBounds() {
  stopMotors();
  int pointDirection; //which way the robot should point after spinning: it should spin to the clostest angle parallel to the field
  IMU_GetReadings();
  pointDirection = (abs(IMU_calcError(g_goal)) <  abs(IMU_calcError(g_goal + 180))) ? g_goal : (g_goal + 180);
  IMU_spinToDirection(pointDirection);
  for (int i = 0; i < 10000; i++) {
    updateDistances();
    int x = 0;
    int y = 0;
    boolean isClear = true;
    boolean r = rightClear();
    boolean l = leftClear();
    boolean f = frontClear();
    boolean b = backClear();
    if (!r) x--, isClear = false;
    if (!l) x++, isClear = false;
    if (!f) y--, isClear = false;
    if (!b) y++, isClear = false;
    if (x == 0 && y == 0) { //all sides are clear, or two sensors on the same axis are both blocked
      Serial6.println("x == 0, y == 0");
      IMU_spinToDirection(pointDirection);
      if (backDist > 120) { //if there is space behind you move
        Serial6.println("moving back in bounds backwards");
        driveToHeadingIMU(pointDirection, 180, 100);
      }
      else if (frontDist > 120) {
        Serial6.println("moving back in bounds forwards");
        driveToHeadingIMU(pointDirection, 0, 100);
      }
    } else {
      Serial6.println("doing xyToAngle");
      int ang = xyToAngle(x, y);
      driveToHeadingIMU(pointDirection, ang, 120);
    }
    if (isClear) { //IN BOUNDS
      Serial6.println("in bounds");
      stopMotors();
      clearCameraBuffer();
      lastTimeSawBall = millis();
      spinningToBall = false;
      ballAngle = 10000; //before this ballAngle = whatever it was before it hit the line. So, sets it to 10000 assuming it does not see ball so it does not move at random angle
      break;
    }
  }
  currentState = DOESNT_SEE_BALL; //sets state to doesn't see ball
}


