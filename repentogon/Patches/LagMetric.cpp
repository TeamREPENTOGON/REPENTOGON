#include "LagMetric.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "../REPENTOGONOptions.h"
#include <chrono>
namespace LagMetric{
    int _lagMetric=16000;	//millisecond precision with resolution of 1000 for gradual transition
    int _lastLagMetric=16000;
	std::chrono::system_clock::time_point _lastFrameTime = std::chrono::system_clock::now();
	int _lastMillis=1600;
	bool _init = false;

	inline bool LagMetric::is_circle_laser(Entity_Laser* in) {
		unsigned int _subtype = in->_subtype;
		if (((_subtype != 1) && (_subtype != 2)) && (_subtype != 3)) {
			return false;
		};
		return true;
	};
	inline bool LagMetric::is_tris_laser(Entity_Laser* in) {
		const unsigned int tris_flag = (1 << (60 - 32));
		if(in->GetParent() && in->GetParent()->_type == 2 && ((((Entity_Tear*)(in->GetParent()))->_tearFlags.lh & tris_flag) == tris_flag)) {
			return true;
		};
		return false;
	};
	void LagMetric::ApplyTris(Entity_Laser* in) {
		unsigned int num_samples = 1;
		const unsigned int FLAGS_WORMS_LL = 1 << 10 | 1 << 26 | 1 << 30;
		const unsigned int FLAGS_WORMS_LH = 1 << (46 - 32);	//ouroborous
		const unsigned int FLAGS_WORMS_HL = 1 << (71 - 64);	//brain worm
		BitSet128& laser_flags = in->_tearFlags;
		laser_flags.ll &= (~(1 << 2));	//remove homing
		bool has_worm = ((laser_flags.ll & (FLAGS_WORMS_LL)) != 0) || ((laser_flags.lh & (FLAGS_WORMS_LH)) != 0) || ((laser_flags.hl & (FLAGS_WORMS_HL)) != 0);
		bool has_reflection = (laser_flags.ll & (1 << 8)) != 0;
		if (has_reflection) {
			num_samples = 10;
		};
		if (has_worm) {
			num_samples = 16;
		};
		in->_homingLaser._sampleNum = num_samples;
	};

	void LagMetric::ApplyFastLasers(Entity_Laser* in) {
		unsigned int num_samples = 50;
		unsigned int _lagFPS = (unsigned int)(1000 * 1000 / _lagMetric);
		//		num_samples -= 1;
		num_samples = (unsigned int)(num_samples * _lagFPS / (60 * 3));
		num_samples += 1;
		num_samples = 2 * ((num_samples + 2 - 1) / 2);	//ceil of div by 2 multiplied by 2 to get a number rounded up to be even
		if (is_circle_laser(in)) {
			num_samples = (std::max)(num_samples, 6u);	//ensure that circles can't become diamonds
		};
		in->_homingLaser._sampleNum = num_samples;
	};
};
using namespace LagMetric;
namespace chrono=std::chrono;
HOOK_METHOD(Game,Render,(void)->void){
	if (!repentogonOptions.fastLasers) {	//todo: add a separate toggle for performance metrics
		return super();
	};
//	return super();
	chrono::time_point frametime = chrono::system_clock::now();
	auto diff = frametime - _lastFrameTime;
	int millis = (int)chrono::duration_cast<chrono::milliseconds>(diff).count()*1000;
	int millis_diff = millis - _lastMillis;
	if (!_init) {
		_lastFrameTime = frametime;
		_lastLagMetric = 1600;
		millis = 1600;
		millis_diff = 0;
		_init = true;
	};
	_lagMetric = (std::max)( (int)(_lastLagMetric * 0.995), millis+millis_diff*2);
	super();
	_lastMillis = millis;
	_lastFrameTime = frametime;
	_lastLagMetric = _lagMetric;
//	ZHL::Log("current lag metric is %u ms\n",_lagMetric);
};


HOOK_METHOD(Game, Exit, (bool ShouldSave)->void) {
	super(ShouldSave);
	_init = false;
};
HOOK_METHOD(Entity_Laser, ClearLaserSamples, ()->void) {
	if (is_tris_laser(this)) {
		ApplyTris(this);
		return super();
	};
	if (!this->_sampleLaser || (this->_homingLaser._sampleNum < 10) || (this->_variant == 2 && !*(this->GetOneHit()) && !is_circle_laser(this))) {
		return super();
	};
	//if (repentogonOptions.fastLasers) {
	//	ApplyFastLasers(this);
	//	return super();
	//};
	return super();
};
HOOK_METHOD(Entity_Laser, Update, (void)->void) {
	if(is_tris_laser(this)){
		ApplyTris(this);
		return super();
	};
	if (!this->_sampleLaser || (this->_homingLaser._sampleNum < 10) || (this->_variant==2 && !*(this->GetOneHit()) && !is_circle_laser(this)) ) {
		return super();
	};
	//if (repentogonOptions.fastLasers) {
	//	ApplyFastLasers(this);
	//	return super();
	//};
	return super();
};