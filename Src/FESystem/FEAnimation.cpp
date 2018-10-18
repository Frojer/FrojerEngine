#include "FEAnimation.h"
#include "FETime.h"
#include <FEMath.h>

FEAnimation::FEAnimation()
	: _animState(PAUSE), _animTime(0.0f), m_maxAnimTime(0.0f), m_bLoop(true)
{
}
FEAnimation::~FEAnimation()
{
}

float Lerp(float a, float b, float t)
{
	return a + ((b - a) * t);
}

void FEAnimation::AnimationUpdate()
{
	if (_animState == RUN)
	{
		float weight;
		FEVector3 v, v2;
		if (m_animPos.size() != 0)
		{
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
		}

		FEVector4 totalQ, nextQ;
		if (m_animRot.size() != 0)
		{
			totalQ = FEMath::FEQuaternionRotationAxis(FEMath::FEConvertToAlignData(m_animRot[0].axis), m_animRot[0].angle);
			if (m_animRot[0].animTime <= _animTime)
			{
				for (int i = 1; i < m_animRot.size(); i++)
				{
					if (m_animRot[i].animTime > _animTime)
					{
						weight = (_animTime - m_animRot[i - 1].animTime) / (m_animRot[i].animTime - m_animRot[i - 1].animTime);
						nextQ = FEMath::FEQuaternionMultiply(totalQ, DirectX::XMQuaternionRotationAxis(FEMath::FEConvertToAlignData(m_animRot[i].axis), m_animRot[i].angle));

						totalQ = FEMath::FEQuaternionSlerp(totalQ, nextQ, weight);
						toEulerAngle(totalQ, v.x, v.y, v.z);

						GetMyObject()->GetTransform()->SetRotationRadian(v);
						break;
					}

					totalQ = FEMath::FEQuaternionMultiply(totalQ, DirectX::XMQuaternionRotationAxis(FEMath::FEConvertToAlignData(m_animRot[i].axis), m_animRot[i].angle));
				}
			}
		}

		if (m_animScale.size() != 0)
		{
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
		}


		_animTime += FETime::GetDeltaTime();
		if (_animTime >= m_maxAnimTime)
		{
			_animTime -= m_maxAnimTime;
			if (!m_bLoop)
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