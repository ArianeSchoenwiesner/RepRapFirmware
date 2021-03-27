/*
 * InputShaper.h
 *
 *  Created on: 20 Feb 2021
 *      Author: David
 */

#ifndef SRC_MOVEMENT_INPUTSHAPER_H_
#define SRC_MOVEMENT_INPUTSHAPER_H_

#include <RepRapFirmware.h>
#include <General/NamedEnum.h>
#include <ObjectModel/ObjectModel.h>

NamedEnum(InputShaperType, uint8_t,
	none,
	ZVD,
	ZVDD,
	EI2,
	DAA
);

class DDA;
class MoveSegment;

struct InputShaperPlan
{
	uint32_t accelSegments : 4,				// number of acceleration segments
			 decelSegments : 4;				// number of deceleration segments

	InputShaperPlan() : accelSegments(1), decelSegments(1) { }
};

class InputShaper INHERIT_OBJECT_MODEL
{
public:
	InputShaper() noexcept;

	uint16_t GetHalfPeriodClocks() const noexcept { return halfPeriod; }			// return the half period in step clocks
	float GetFullPeriod() const noexcept;											// return the full period in seconds
	float GetFrequency() const noexcept;
	float GetFloatDamping() const noexcept;
	float GetMinimumAcceleration() const noexcept { return minimumAcceleration; }
	InputShaperType GetType() const noexcept { return type; }
	InputShaperPlan PlanShaping(DDA& dda) const noexcept;
	MoveSegment *GetAccelerationSegments(InputShaperPlan plan, const DDA& dda, float distanceLimit, MoveSegment *nextSegment) const noexcept;
	MoveSegment *GetDecelerationSegments(InputShaperPlan plan, const DDA& dda, float distanceLimit, float decelStartDistance, float decelStartClocks) const noexcept;

	GCodeResult Configure(GCodeBuffer& gb, const StringRef& reply) THROWS(GCodeException);	// process M593

protected:
	DECLARE_OBJECT_MODEL

private:
	static constexpr float DefaultFrequency = 40.0;
	static constexpr float DefaultDamping = 0.2;
	static constexpr float DefaultMinimumAcceleration = 10.0;

	uint16_t halfPeriod;							// half the period of ringing that we don't want to excite, in step clocks
	uint16_t damping;								// damping factor of the ringing as a 16-bit fractional number
	float minimumAcceleration;						// the minimum value that we reduce acceleration to
	InputShaperType type;
};

#endif /* SRC_MOVEMENT_INPUTSHAPER_H_ */
