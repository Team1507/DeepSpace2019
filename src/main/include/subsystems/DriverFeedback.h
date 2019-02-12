
#pragma once

#include <frc/commands/Subsystem.h>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

//RGB values for LED colors
//                 red, green, blue
#define YELLOW_rgb  0.6, 0.2, 0.0
#define ORANGE_rgb  0.9, 0.1, 0.0
#define GREEN_rgb   0.0, 1.0, 0.0
#define RED_rgb     1.0, 0.0, 0.0
#define BLUE_rgb    0.0, 0.0, 1.0
#define PURPLE_rgb  0.5, 0.0, 0.5

class DriverFeedback : public frc::Subsystem {
 private:
  // It's desirable that everything possible under private except
  // for methods that implement subsystem capabilities
    CANifier* l_canifier;
    CANifier* r_canifier;

    
 public:
    
    DriverFeedback();
    void InitDefaultCommand() override;
    void DriverFeedbackPeriodic(void);
    void UpdateLeftLEDs(float,float,float);
    void UpdateRightLEDs(float,float,float);
    void LeftLEDsOff(void);
    void RightLEDsOff(void);
    void RumbleOn(void);
    void RumbleOff(void);
};
