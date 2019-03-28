#pragma once
#ifndef COLLECTOR_H
#define COLLECTOR_H
#include <frc/commands/Subsystem.h>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

class Collector : public frc::Subsystem {
private:
    
    frc::DoubleSolenoid *collectorCage;
    frc::DoubleSolenoid *collectorBridge;
    frc::DigitalInput   *collectorPhotoeye;
    VictorSPX           *collectorRollers;
    bool m_DeployCage;
    bool m_OpenBridge;
    bool m_autocollect;
    int  m_ballcount;
    

public:
    bool m_autoXfer;    //!!! This should be private !!!!!!
    Collector();
    void InitDefaultCommand() override;
    void CollectorPeriodic(void);
    bool IsCollectorPhotoeyeDetected(void);
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
    const static double COLLECT_SPEED;
    const static double BRIDGE_SPEED;
    const static double MANUAL_SPIT_SPEED;
    const static double MANUAL_INTAKE_FAST;
    const static double MANUAL_INTAKE_SLOW;
    void CollectorRollers(double power);
    void StopCollectorRollers(void);
    void VictorSPXInit(void);
};
#endif