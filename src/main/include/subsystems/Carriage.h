#pragma once
#ifndef CARRIAGE_H
#define CARRIAGE_H

#include <frc/commands/Subsystem.h>
#include "frc/WPILib.h"

class Carriage : public frc::Subsystem {
 private:
    frc::DoubleSolenoid *carriageHatchEject;
    frc::DoubleSolenoid *carriageHatchLatch;
    frc::DoubleSolenoid *carriageTilt;
    frc::DigitalInput   *carriagePhotoeye;
    frc::Spark          *carriageRollers;
    bool m_HatchEject;
    bool m_OpenLatch;
    bool m_TiltUp;
public:
    Carriage();
    void InitDefaultCommand() override;
    void Periodic(void);
    bool IsPhotoeyeDetected(void);
    void OpenLatch(void);
    void CloseLatch(void);
    void SetLatch(int position);
    bool IsHatchLatched(void);
    const static int LATCH_OPEN;
    const static int LATCH_CLOSE;
    void TiltUp(void);
    void TiltDown(void);
    void SetTilt(int position);
    bool IsCarriageTiltedUp(void);
    const static int TILT_UP;
    const static int TILT_DOWN;
    void CarriageRollers(double power);
    void StopCarriageRollers(void);
    const static int BRIDGE_SPEED;
    const static double GETTING_BALL_SPEED;
    const static double SHOOTING_BALL_SPEED;

};
#endif