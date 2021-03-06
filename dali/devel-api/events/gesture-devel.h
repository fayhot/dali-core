#ifndef DALI_GESTURE_DEVEL_H
#define DALI_GESTURE_DEVEL_H

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

#include <dali/public-api/events/gesture.h>

namespace Dali
{

namespace DevelGesture
{

/**
 * @copydoc Dali::Gesture::Type
 */
enum Type
{
  Pinch      = Gesture::Pinch,
  Pan        = Gesture::Pan,
  Tap        = Gesture::Tap,
  LongPress  = Gesture::LongPress,

  // Devel Gesture Types
  Rotation   = 1 << 4, ///< When the user rotates two fingers around a particular ares of the screen.
};

} // namespace DevelGesture

} // namespace Dali

#endif // DALI_GESTURE_DEVEL_H
