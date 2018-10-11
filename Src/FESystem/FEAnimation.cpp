#include "FEAnimation.h"
#include "FETime.h"

FEAnimation::FEAnimation()
	: _animState(PAUSE), _animTime(0.0f), _maxAnimTime(0.0f) 
{
}
FEAnimation::~FEAnimation()
{
}

float Lerp(float a, float b, float t)
{
	return a + ((b - a) * t);
}

void toEulerAngle(const FEVector4& q, float& roll, float& pitch, float& yaw)
{
	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	roll = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		pitch = copysign(FE_PI / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	yaw = atan2(siny_cosp, cosy_cosp);
}

void FEAnimation::AnimationUpdate()
{
	if (_animState == RUN)
	{
		float weight;
		FEVector3 v, v2;
		if (m_animPos[0].animTime <= _animTime)
		{
			for (int i = 1; i < m_animPos.size(); i++)
			{
				if (m_animPos[i].animTime > _animTime)
				{
					weight = (_animTime - m_animPos[i - 1].animTime) / (m_animPos[i].animTime - m_animPos[i - 1].animTime);
					v.x = Lerp(m_animPos[i - 1].pos.x, m_animPos[i].pos.x, weight);
					v.y = Lerp(m_animPos[i - 1].pos.y, m_animPos[i].pos.y, weight);
					v.z = Lerp(m_animPos[i - 1].pos.z, m_animPos[i].pos.z, weight);
					GetMyObject()->GetTransform()->SetPositionLocal(v);
					break;
				}
			}
		}

		FEVectorA totalQ, nextQ;
		totalQ = DirectX::XMQuaternionRotationAxis(FEMath::FEConvertToAlignData(m_animRot[0].axis), m_animRot[0].angle);
		if (m_animRot[0].animTime <= _animTime)
		{
			for (int i = 1; i < m_animRot.size(); i++)
			{
				if (m_animRot[i].animTime > _animTime)
				{
					weight = (_animTime - m_animRot[i - 1].animTime) / (m_animRot[i].animTime - m_animRot[i - 1].animTime);
					nextQ = DirectX::XMQuaternionMultiply(totalQ, DirectX::XMQuaternionRotationAxis(FEMath::FEConvertToAlignData(m_animRot[i].axis), m_animRot[i].angle));

					totalQ = DirectX::XMQuaternionSlerp(totalQ, nextQ, weight);
					toEulerAngle(totalQ, v.x, v.y, v.z);

					GetMyObject()->GetTransform()->SetRotationRadian(v);
					break;
				}

				totalQ = DirectX::XMQuaternionMultiply(totalQ, DirectX::XMQuaternionRotationAxis(FEMath::FEConvertToAlignData(m_animRot[i].axis), m_animRot[i].angle));
			}
		}
		if (m_animScale[0].animTime <= _animTime)
		{
			for (int i = 1; i < m_animScale.size(); i++)
			{
				if (m_animScale[i].animTime > _animTime)
				{
					weight = (_animTime - m_animScale[i - 1].animTime) / (m_animScale[i].animTime - m_animScale[i - 1].animTime);
					v.x = Lerp(m_animScale[i - 1].scale.x, m_animScale[i].scale.x, weight);
					v.y = Lerp(m_animScale[i - 1].scale.y, m_animScale[i].scale.y, weight);
					v.z = Lerp(m_animScale[i - 1].scale.z, m_animScale[i].scale.z, weight);
					GetMyObject()->GetTransform()->m_vScale = v;
					break;
				}
			}
		}


		_animTime += FETime::GetDeltaTime();
		if (_animTime >= _maxAnimTime)
		{
			_animTime = 0.0f;
			if (!_bLoop)
				_animState = PAUSE;
		}
	}
}

void FEAnimation::Play()
{
	_animState = RUN;
}
void FEAnimation::Pause()
{
	_animState = PAUSE;
}
void FEAnimation::Stop()
{
	_animTime = 0.0f;
	_animState = PAUSE;
}