#pragma once
#ifndef COLLECTOR_H
#define COLLECTOR_H
#include <frc/commands/Subsystem.h>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

class Collector : public frc::Subsystem {
 private:
  // It's desirable that everything possible under private except
  // for methods that implement subsystem capabilities
    frc::DoubleSolenoid *collectorCage;
    frc::DoubleSolenoid *collectorBridge;
    frc::DigitalInput *collectorPhotoeye;
    VictorSPX      *collectorRollers;
    bool m_DeployCage;
    bool m_OpenBridge;
 public:
    Collector();
    void InitDefaultCommand() override;
    void CollectorPeriodic(void);
    bool IsPhotoeyeDetected(void);
    void DeployCage(void);
    void RetractCage(void);
    void SetCage(int position);
    bool IsCageDeployed(void);
    void OpenBridge(void);
    void CloseBridge(void);
    void SetBridge(int position);
    bool IsBridgeDeployed(void);
    const static int CLOSE_BRIDGE;
    const static int OPEN_BRIDGE;
    const static int RETRACT_CAGE;
    const static int DEPLOY_CAGE;
    const static int COLLECT_SPEED;
    const static int BRIDGE_SPEED;
    const static int MANUAL_SPIT_SPEED;
    const static int MANUAL_INTAKE_FAST;
    const static int MANUAL_INTAKE_SLOW;
    void CollectorRollers(double power);
    void StopCollectorRollers(void);
    void VictorSPXInit(void);
};
#endif