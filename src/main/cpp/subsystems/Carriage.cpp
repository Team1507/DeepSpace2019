#include "subsystems/Carriage.h"
#include "Robot.h"

//const int Carriage::HATCH_RETRACT = 0;
//const int Carriage::HATCH_EJECT = 1;
const int Carriage::LATCH_CLOSE = 0;
const int Carriage::LATCH_OPEN = 1;
const int Carriage::TILT_DOWN = 0;
const int Carriage::TILT_UP = 1;
const int Carriage::BRIDGE_SPEED = 0.2;

Carriage::Carriage() : Subsystem("Carriage") {
  carriageHatchLatch = new frc::DoubleSolenoid(2,4,5);
  carriageTilt       = new frc::DoubleSolenoid(2,2,3);
  carriagePhotoeye   = new frc::DigitalInput(5);
  carriageRollers    = new frc::Spark(1);
}

void Carriage::InitDefaultCommand() {}

//********PhotoEye***********
bool Carriage::IsPhotoeyeDetected(void)
{
	return carriagePhotoeye->Get();
}

//********HATCH LATCH***********
void Carriage::OpenLatch(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kForward);
	m_OpenLatch = true;
}
void Carriage::CloseLatch(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kReverse);
	m_OpenLatch = false;
}
void Carriage::SetLatch(int position)
{
	if(position == LATCH_OPEN )  OpenLatch();
	if(position == LATCH_CLOSE)  CloseLatch();
}
bool Carriage::IsHatchLatched(void){
	return m_OpenLatch;
}

//********TILT***********
void Carriage::TiltUp(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kForward);
	m_TiltUp = true;
}
void Carriage::TiltDown(void)
{
	carriageHatchLatch->Set(DoubleSolenoid::kReverse);
	m_TiltUp = false;
}
void Carriage::SetTilt(int position)
{
	if(position == TILT_UP )   TiltUp();
	if(position == TILT_DOWN)  TiltDown();
}
bool Carriage::IsCarriageTiltedUp(void){
	return m_TiltUp;
}

//********ROLLERS***********
void Carriage::CarriageRollers(double power)
{
	carriageRollers->Set(power);
}
void Carriage::StopCarriageRollers(void)
{
	carriageRollers->Set(0.0);
}