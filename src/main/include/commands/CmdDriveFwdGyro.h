

#pragma once

#include <frc/commands/Command.h>

class CmdDriveFwdGyro : public frc::Command {
 public:
    CmdDriveFwdGyro(double power, double heading, double distance, bool stop, double timeout);
    
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End() override;
    void Interrupted() override;


  private:
    double m_power;
    double m_heading;
    double m_distance;
    bool   m_stop;
    double m_timeout;   

};
