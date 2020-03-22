/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>

#include <stdlib.h>
#include <dali/public-api/dali-core.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <dali/integration-api/render-task-list-integ.h>
#include <dali/devel-api/events/gesture-devel.h>
#include <dali/devel-api/events/rotation-gesture.h>
#include <dali/devel-api/events/rotation-gesture-detector.h>
#include <dali-test-suite-utils.h>
#include <test-touch-utils.h>

using namespace Dali;

void utc_dali_rotation_gesture_detector_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_rotation_gesture_detector_cleanup(void)
{
  test_return_value = TET_PASS;
}

///////////////////////////////////////////////////////////////////////////////
namespace
{

// Stores data that is populated in the callback and will be read by the TET cases
struct SignalData
{
  SignalData()
  : functorCalled(false),
    voidFunctorCalled(false),
    receivedGesture(Gesture::Started)
  {}

  void Reset()
  {
    functorCalled = false;
    voidFunctorCalled = false;

    receivedGesture.state = Gesture::Started;
    receivedGesture.rotation = 0.0f;
    receivedGesture.screenCenterPoint = Vector2(0.0f, 0.0f);
    receivedGesture.localCenterPoint = Vector2(0.0f, 0.0f);

    rotatedActor.Reset();
  }

  bool functorCalled;
  bool voidFunctorCalled;
  RotationGesture receivedGesture;
  Actor rotatedActor;
};

// Functor that sets the data when called
struct GestureReceivedFunctor
{
  GestureReceivedFunctor(SignalData& data) : signalData(data) { }

  void operator()(Actor actor, const RotationGesture& rotation)
  {
    signalData.functorCalled = true;
    signalData.receivedGesture = rotation;
    signalData.rotatedActor = actor;
  }

  void operator()()
  {
    signalData.voidFunctorCalled = true;
  }

  SignalData& signalData;
};

// Functor that removes the gestured actor from stage
struct UnstageActorFunctor : public GestureReceivedFunctor
{
  UnstageActorFunctor( SignalData& data, Gesture::State& stateToUnstage )
  : GestureReceivedFunctor( data ),
    stateToUnstage( stateToUnstage )
  {
  }

  void operator()( Actor actor, const RotationGesture& rotation )
  {
    GestureReceivedFunctor::operator()( actor, rotation );

    if ( rotation.state == stateToUnstage )
    {
      Stage::GetCurrent().Remove( actor );
    }
  }

  Gesture::State& stateToUnstage;
};

// Functor for receiving a touch event
struct TouchEventFunctor
{
  bool operator()(Actor actor, const TouchEvent& touch)
  {
    return false;
  }
};

} // anon namespace

///////////////////////////////////////////////////////////////////////////////

int UtcDaliRotationGestureDetectorConstructor(void)
{
  TestApplication application;

  RotationGestureDetector detector;
  DALI_TEST_CHECK(!detector);
  END_TEST;
}

int UtcDaliRotationGestureDetectorCopyConstructorP(void)
{
  TestApplication application;

  RotationGestureDetector detector = RotationGestureDetector::New();;

  RotationGestureDetector copy( detector );
  DALI_TEST_CHECK( detector );
  END_TEST;
}

int UtcDaliRotationGestureDetectorAssignmentOperatorP(void)
{
  TestApplication application;

  RotationGestureDetector detector = RotationGestureDetector::New();;

  RotationGestureDetector assign;
  assign = detector;
  DALI_TEST_CHECK( detector );

  DALI_TEST_CHECK( detector == assign );
  END_TEST;
}

int UtcDaliRotationGestureDetectorNew(void)
{
  TestApplication application;

  RotationGestureDetector detector = RotationGestureDetector::New();

  DALI_TEST_CHECK(detector);

  // Attach an actor and emit a touch event on the actor to ensure complete line coverage
  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  detector.Attach(actor);

  Integration::TouchEvent touchEvent(1);
  Integration::Point point;
  point.SetDeviceId( 1 );
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 20.0f, 20.0f ) );
  touchEvent.AddPoint(point);
  application.ProcessEvent(touchEvent);

  Integration::Point point2;
  point.SetDeviceId( 1 );
  point.SetState( PointState::DOWN );
  point.SetScreenPosition( Vector2( 20.0f, 20.0f ) );
  touchEvent.AddPoint(point2);
  application.ProcessEvent(touchEvent);
  END_TEST;
}

int UtcDaliRotationGestureDetectorDownCast(void)
{
  TestApplication application;
  tet_infoline("Testing Dali::RotationGestureDetector::DownCast()");

  RotationGestureDetector detector = RotationGestureDetector::New();

  BaseHandle object(detector);

  RotationGestureDetector detector2 = RotationGestureDetector::DownCast(object);
  DALI_TEST_CHECK(detector2);

  RotationGestureDetector detector3 = DownCast< RotationGestureDetector >(object);
  DALI_TEST_CHECK(detector3);

  BaseHandle unInitializedObject;
  RotationGestureDetector detector4 = RotationGestureDetector::DownCast(unInitializedObject);
  DALI_TEST_CHECK(!detector4);

  RotationGestureDetector detector5 = DownCast< RotationGestureDetector >(unInitializedObject);
  DALI_TEST_CHECK(!detector5);

  GestureDetector detector6 = RotationGestureDetector::New();
  RotationGestureDetector detector7 = RotationGestureDetector::DownCast(detector6);
  DALI_TEST_CHECK(detector7);
  END_TEST;
}

// Negative test case for a method
int UtcDaliRotationGestureSignalReceptionNegative(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Do a rotation outside actor's area
  TestStartRotation( application,  Vector2( 112.0f, 62.0f ), Vector2( 112.0f, 162.0f ),
                                   Vector2( 112.0f, 100.0f ), Vector2( 112.0f, 124.0f ), 100 );

  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);

  // Continue rotation into actor's area - we should still not receive the signal
  data.Reset();
  TestContinueRotation( application, Vector2( 112.0f, 100.0f ), Vector2( 112.0f, 124.0f ),
                                     Vector2( 5.0f, 5.0f ), Vector2( 35.0f, 35.0f ), 200 );

  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);

  // Stop rotating - we should still not receive the signal
  data.Reset();
  TestEndRotation( application,  Vector2( 6.0f, 6.0f ), Vector2( 18.0f, 18.0f ),
                                 Vector2( 10.0f, 8.0f ), Vector2( 14.0f, 16.0f ), 300 );

  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionDownMotionLeave(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Start pan within the actor's area
  TestStartRotation( application,  Vector2( 5.0f, 5.0f ), Vector2( 20.0f, 20.0f ),
                                   Vector2( 5.0f, 5.0f ), Vector2( 20.0f, 30.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Started, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(0.244f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(12.5f, 17.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Continue the pan within the actor's area - we should still receive the signal
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 17.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 400 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Continuing, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.785398f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(21.0f, 20.0f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Pan Gesture leaves actor's area - we should still receive the signal
  data.Reset();
  TestContinueRotation( application, Vector2( 17.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                     Vector2( 300.0f, 10.0f ), Vector2( 340.0f, 10.0f ), 1000 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Continuing, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.785398f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(320.0f, 10.0f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Gesture ends - we would receive a finished state
  data.Reset();
  TestEndRotation( application,  Vector2( 300.0f, 10.0f ), Vector2( 340.0f, 10.0f ),
                                 Vector2( 305.0f, 10.0f ), Vector2( 315.0f, 10.0f ), 1500);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Finished, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.785398f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(310.0f, 10.0f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionDownMotionUp(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Start rotation within the actor's area
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 31.0f, 29.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Started, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(0.404892f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(20.5f, 24.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Continue the rotation within the actor's area - we should still receive the signal
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 29.0f, 15.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Continuing, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.343024f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(22.0f, 17.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Gesture ends within actor's area - we would receive a finished state
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 29.0f, 15.0f ), 1000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Finished, data.receivedGesture.state, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.463648f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(24.0f, 17.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionDetach(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Start rotation within the actor's area
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Started, data.receivedGesture.state, TEST_LOCATION);


  // Continue the rotation within the actor's area - we should still receive the signal
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Continuing, data.receivedGesture.state, TEST_LOCATION);

  // Gesture ends within actor's area
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Finished, data.receivedGesture.state, TEST_LOCATION);

  // Detach actor
  detector.DetachAll();

  // Ensure we are no longer signalled
  data.Reset();
  TestGenerateRotation(  application );
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionDetachWhileRotationing(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Start rotation within the actor's area
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Started, data.receivedGesture.state, TEST_LOCATION);

  // Continue the rotation within the actor's area - we should still receive the signal
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(Gesture::Continuing, data.receivedGesture.state, TEST_LOCATION);

  // Detach actor during the rotation, we should not receive the next event
  detector.DetachAll();

  // Gesture ends within actor's area
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionActorDestroyedWhileRotationing(void)
{
  TestApplication application;

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.DetectedSignal().Connect(&application, functor);

  // Attach a temporary actor to stop detector being removed from RotationGestureProcessor when main actor
  // is destroyed.
  Actor tempActor = Actor::New();
  tempActor.SetSize(100.0f, 100.0f);
  tempActor.SetAnchorPoint(AnchorPoint::BOTTOM_RIGHT);
  Stage::GetCurrent().Add(tempActor);
  detector.Attach(tempActor);

  // Actor lifetime is scoped
  {
    Actor actor = Actor::New();
    actor.SetSize(100.0f, 100.0f);
    actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
    Stage::GetCurrent().Add(actor);

    // Render and notify
    application.SendNotification();
    application.Render();

    detector.Attach(actor);

    // Start rotation within the actor's area
    TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                     Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
    DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
    DALI_TEST_EQUALS(Gesture::Started, data.receivedGesture.state, TEST_LOCATION);

    // Continue the rotation within the actor's area - we should still receive the signal
    data.Reset();
    TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                       Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
    DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
    DALI_TEST_EQUALS(Gesture::Continuing, data.receivedGesture.state, TEST_LOCATION);

    // Remove the actor from stage and reset the data
    Stage::GetCurrent().Remove(actor);

    // Render and notify
    application.SendNotification();
    application.Render();
  }

  // Actor should now have been destroyed

  // Gesture ends within the area where the actor used to be
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionRotatedActor(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetOrientation(Dali::Degree(90.0f), Vector3::ZAXIS);
  Stage::GetCurrent().Add(actor);

  // Render and notify a couple of times
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Do an entire rotation, only check finished value
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 27.0f, 15.0f ), 1000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.558599f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(23.0f, 17.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Rotate actor again and render and notify
  actor.SetOrientation(Dali::Degree(180.0f), Vector3::ZAXIS);
  application.SendNotification();
  application.Render();

  // Do an entire rotation, only check finished value
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 2100 );
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 27.0f, 15.0f ), 3000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.558599f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(23.0f, 17.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Rotate actor again and render and notify
  actor.SetOrientation(Dali::Degree(270.0f), Vector3::ZAXIS);
  application.SendNotification();
  application.Render();

  // Do an entire rotation, only check finished value
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 4100 );
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 27.0f, 15.0f ), 5000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(-0.558599f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(23.0f, 17.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionChildHit(void)
{
  TestApplication application;

  Actor parent = Actor::New();
  parent.SetSize(100.0f, 100.0f);
  parent.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(parent);

  // Set child to completely cover parent.
  // Change rotation of child to be different from parent so that we can check if our local coordinate
  // conversion of the parent actor is correct.
  Actor child = Actor::New();
  child.SetSize(100.0f, 100.0f);
  child.SetAnchorPoint(AnchorPoint::CENTER);
  child.SetParentOrigin(ParentOrigin::CENTER);
  child.SetOrientation(Dali::Degree(90.0f), Vector3::ZAXIS);
  parent.Add(child);

  TouchEventFunctor touchFunctor;
  child.TouchedSignal().Connect(&application, touchFunctor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(parent);
  detector.DetectedSignal().Connect(&application, functor);

  // Do an entire pan, only check finished value - hits child area but parent should still receive it
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 29.0f, 25.0f ), 1000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, parent == data.rotatedActor, TEST_LOCATION);
  DALI_TEST_EQUALS(0.463648f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(24.0f, 22.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);

  // Attach child and generate same touch points to yield same results
  // (Also proves that you can detach and then re-attach another actor)
  detector.Attach(child);
  detector.Detach(parent);

  // Do an entire pan, only check finished value
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 2100 );
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 29.0f, 35.0f ), 3000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, child == data.rotatedActor, TEST_LOCATION);
  DALI_TEST_EQUALS(0.982794f, data.receivedGesture.rotation.radian, 0.01f, TEST_LOCATION);
  DALI_TEST_EQUALS(Vector2(24.0f, 27.5f), data.receivedGesture.screenCenterPoint, 0.01f, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionAttachDetachMany(void)
{
  TestApplication application;

  Actor first = Actor::New();
  first.SetSize(100.0f, 100.0f);
  first.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(first);

  Actor second = Actor::New();
  second.SetSize(100.0f, 100.0f);
  second.SetX(100.0f);
  second.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(second);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(first);
  detector.Attach(second);
  detector.DetectedSignal().Connect(&application, functor);

  // Start rotation within second actor's area
  TestStartRotation( application,  Vector2( 102.0f, 20.0f ), Vector2( 138.0f, 20.0f ),
                                   Vector2( 110.0f, 20.0f ), Vector2( 130.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, second == data.rotatedActor, TEST_LOCATION);

  // Rotation moves into first actor's area - second actor should receive the rotation
  data.Reset();
  TestContinueRotation( application, Vector2( 110.0f, 20.0f ), Vector2( 130.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, second == data.rotatedActor, TEST_LOCATION);

  // Detach the second actor during the rotation, we should not receive the next event
  detector.Detach(second);

  // Gesture ends within actor's area
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 119.0f, 20.0f ), Vector2( 121.0f, 20.0f ), 3000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionActorBecomesUntouchable(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  SignalData data;
  GestureReceivedFunctor functor(data);

  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect(&application, functor);

  // Start rotation in actor's area
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);

  // Pan continues within actor's area
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);

  // Actor become invisible - actor should not receive the next rotation
  actor.SetVisible(false);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Gesture ends within actor's area
  data.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 3000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionMultipleDetectorsOnActor(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  Actor actor2 = Actor::New();
  actor2.SetSize(100.0f, 100.0f);
  actor2.SetAnchorPoint(AnchorPoint::BOTTOM_RIGHT);
  Stage::GetCurrent().Add(actor2);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Attach actor to one detector
  SignalData firstData;
  GestureReceivedFunctor firstFunctor(firstData);
  RotationGestureDetector firstDetector = RotationGestureDetector::New();
  firstDetector.Attach(actor);
  firstDetector.DetectedSignal().Connect(&application, firstFunctor);

  // Attach actor to another detector
  SignalData secondData;
  GestureReceivedFunctor secondFunctor(secondData);
  RotationGestureDetector secondDetector = RotationGestureDetector::New();
  secondDetector.Attach(actor);
  secondDetector.DetectedSignal().Connect(&application, secondFunctor);

  // Add second actor to second detector, when we remove the actor, this will make sure that this
  // gesture detector is not removed from the GestureDetectorProcessor.  In this scenario, the
  // functor should still not be called (which is what we're also testing).
  secondDetector.Attach(actor2);

  // Rotation in actor's area - both detector's functors should be called
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, firstData.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, secondData.functorCalled, TEST_LOCATION);

  // Rotation continues in actor's area - both detector's functors should be called
  firstData.Reset();
  secondData.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, firstData.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, secondData.functorCalled, TEST_LOCATION);

  // Detach actor from firstDetector and emit rotation on actor, only secondDetector's functor should be called.
  firstDetector.Detach(actor);
  firstData.Reset();
  secondData.Reset();
  TestEndRotation( application,  Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(false, firstData.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, secondData.functorCalled, TEST_LOCATION);

  // New rotation on actor, only secondDetector has actor attached
  firstData.Reset();
  secondData.Reset();
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 1500 );
  DALI_TEST_EQUALS(false, firstData.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(true, secondData.functorCalled, TEST_LOCATION);

  // Detach actor from secondDetector
  secondDetector.Detach(actor);
  firstData.Reset();
  secondData.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 2000 );
  DALI_TEST_EQUALS(false, firstData.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(false, secondData.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureSignalReceptionEnsureCorrectSignalling(void)
{
  TestApplication application;

  Actor actor1 = Actor::New();
  actor1.SetSize(100.0f, 100.0f);
  actor1.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor1);
  SignalData data1;
  GestureReceivedFunctor functor1(data1);
  RotationGestureDetector detector1 = RotationGestureDetector::New();
  detector1.Attach(actor1);
  detector1.DetectedSignal().Connect(&application, functor1);

  Actor actor2 = Actor::New();
  actor2.SetSize(100.0f, 100.0f);
  actor2.SetAnchorPoint(AnchorPoint::BOTTOM_RIGHT);
  actor2.SetParentOrigin(ParentOrigin::BOTTOM_RIGHT);
  Stage::GetCurrent().Add(actor2);
  SignalData data2;
  GestureReceivedFunctor functor2(data2);
  RotationGestureDetector detector2 = RotationGestureDetector::New();
  detector2.Attach(actor2);
  detector2.DetectedSignal().Connect(&application, functor2);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Start pan in actor1's area, only data1 should be set
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data1.functorCalled, TEST_LOCATION);
  DALI_TEST_EQUALS(false, data2.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureActorUnstaged(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  // State to remove actor in.
  Gesture::State stateToUnstage( Gesture::Started );

  // Attach actor to detector
  SignalData data;
  UnstageActorFunctor functor( data, stateToUnstage );
  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect( &application, functor );

  // Emit signals
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  data.Reset();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Re-add actor to stage
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Change state to Gesture::Continuing to remove
  stateToUnstage = Gesture::Continuing;

  // Emit signals
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  data.Reset();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Re-add actor to stage
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Change state to Gesture::Continuing to remove
  stateToUnstage = Gesture::Finished;

  // Emit signals
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  tet_result( TET_PASS ); // If we get here then we have handled actor stage removal gracefully.
  END_TEST;
}

int UtcDaliRotationGestureActorStagedAndDestroyed(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Create and add a second actor so that GestureDetector destruction does not come into play.
  Actor dummyActor( Actor::New() );
  dummyActor.SetSize( 100.0f, 100.0f );
  dummyActor.SetPosition( 100.0f, 100.0f );
  dummyActor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(dummyActor);

  // Render and notify
  application.SendNotification();
  application.Render();

  // State to remove actor in.
  Gesture::State stateToUnstage( Gesture::Started );

  // Attach actor to detector
  SignalData data;
  UnstageActorFunctor functor( data, stateToUnstage );
  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.Attach(dummyActor);
  detector.DetectedSignal().Connect( &application, functor );

  // Here we are testing a Started actor which is removed in the Started callback, but then added back
  // before we get a continuing state.  As we were removed from the stage, even if we're at the same
  // position, we should still not be signalled.

  // Emit signals
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Re add to the stage, we should not be signalled
  Stage::GetCurrent().Add(actor);

  // Render and notify
  application.SendNotification();
  application.Render();

  // Continue signal emission
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 500 );
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  data.Reset();

  // Here we delete an actor in started, we should not receive any subsequent signalling.

  // Emit signals
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 1500 );
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Delete actor as well
  actor.Reset();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Continue signal emission
  TestContinueRotation( application, Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                     Vector2( 15.0f, 20.0f ), Vector2( 25.0f, 20.0f ), 2000 );
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  data.Reset();
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 3000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  END_TEST;
}

int UtcDaliRotationGestureLayerConsumesTouch(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetSize(100.0f, 100.0f);
  actor.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add(actor);

  // Add a detector
  SignalData data;
  GestureReceivedFunctor functor(data);
  RotationGestureDetector detector = RotationGestureDetector::New();
  detector.Attach(actor);
  detector.DetectedSignal().Connect( &application, functor );

  // Add a layer to overlap the actor
  Layer layer = Layer::New();
  layer.SetSize(100.0f, 100.0f);
  layer.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  Stage::GetCurrent().Add( layer );
  layer.RaiseToTop();

  // Render and notify
  application.SendNotification();
  application.Render();

  // Emit signals, should receive
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 100 );
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 1000);
  DALI_TEST_EQUALS(true, data.functorCalled, TEST_LOCATION);
  data.Reset();

  // Set layer to consume all touch
  layer.SetTouchConsumed( true );

  // Render and notify
  application.SendNotification();
  application.Render();

  // Emit the same signals again, should not receive
  TestStartRotation( application,  Vector2( 2.0f, 20.0f ), Vector2( 38.0f, 20.0f ),
                                   Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ), 1500 );
  TestEndRotation( application,  Vector2( 10.0f, 20.0f ), Vector2( 30.0f, 20.0f ),
                                 Vector2( 19.0f, 20.0f ), Vector2( 21.0f, 20.0f ), 2000);
  DALI_TEST_EQUALS(false, data.functorCalled, TEST_LOCATION);
  data.Reset();

  END_TEST;
}

