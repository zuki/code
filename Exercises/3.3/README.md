## アクター

Actor:    mState(e), mPosition(V2), mScale(f), mRotation,(f)  mComponents([c])
  ↑
Asteroid:  mCircle(c)

Laser:  mCircle(c), mDeathTimer(f)

Shilp: mLaserCooldonw(f), mCircle(c)

## コンポーネント

Component
　　↑
CircleComponent: mRadius(F)

SpriteComponent

MoveComponent: mMaxSpeed(v), mMaxAcceleration(f),
	           mMass(f), mForceVectorSum(V2), mVelocity(V2),
               mAngularSpeed(f), mBaseForce(V2), mPosition(V2), mAcceleration(V2),

               mMass(f), mSumOfForce(V2), mVelocity(V2),
               mAngularSpeed(f), mForwardSpeed(f)
　　↑
InputComponent: mMaxForce(f), mMaxAngularSpeed(f)

                mMaxForwardSpeed(f), mMaxAngularSpeed(f)


## `InputComponent::Update()`

- forwardKeyで増減するのが速度(float)から力(V2)に
- angularKeyで増減するのは角速度(float)で変わらず

### MovementCompont::AddForceVector(Vector2 forceVector, bool impulse)

```
mBaseForce = (impulse == true) ? forceVector : Zero
mForceVectorSum = forceVector  // += でないと意味が
```
