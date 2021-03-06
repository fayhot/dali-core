/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

// CLASS HEADER
#include <dali/internal/update/manager/update-manager-debug.h>

// EXTERNAL INCLUDES
#include <sstream>
#include <iomanip>
#include <ios>

// INTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/public-api/common/constants.h>
#include <dali/public-api/math/degree.h>
#include <dali/public-api/math/radian.h>
#include <dali/public-api/math/vector2.h>

#ifdef DALI_PRINT_UPDATE_INFO

namespace Dali
{

namespace Internal
{

namespace SceneGraph
{

#if defined(DEBUG_ENABLED)
static Debug::Filter* gNodeLogFilter = Debug::Filter::New(Debug::Verbose, false, "LOG_UPDATE_MANAGER");
#endif

/**
 * Debug helper function.
 */
void PrintNodes( const Node& node, BufferIndex updateBufferIndex, int level )
{
  const Vector3& position = node.GetPosition(updateBufferIndex);
  const Vector3& scale = node.GetScale(updateBufferIndex);
  const Vector3& fullPos = node.GetWorldPosition(updateBufferIndex);
  const Quaternion& rotation = node.GetOrientation(updateBufferIndex);
  Vector3 axis;
  Radian angle;
  rotation.ToAxisAngle(axis, angle);

  std::string nodeName= DALI_LOG_GET_OBJECT_STRING((&node));

  {
    std::ostringstream oss;
    std::ios_base::fmtflags mask = oss.flags();
    mask &= ~std::ios_base::scientific;
    mask |=  std::ios_base::fixed;
    oss << std::setprecision(2) << std::setiosflags(mask)
        << std::setw(level*2) << std::setfill(' ') << "";
    oss << "Node "  << nodeName << " " << &node
        << "  Position (" << position.x << ", " << position.y << ", " << position.z << ")"
        << "  WorldPosition (" << fullPos.x << ", " << fullPos.y << ", " << fullPos.z << ")"
        << "  Orientation (" << Degree(angle).degree << "degrees <" << axis.x << ", " << axis.y << ", " << axis.z << ">)"
        << "  Scale (" << scale.x << ", " << scale.y << ", " << scale.z << ")"
        << std::endl;

    DALI_LOG_INFO(gNodeLogFilter, Debug::Verbose, "%s\n", oss.str().c_str());
  }

  {
    std::ostringstream oss;
    std::ios_base::fmtflags mask = oss.flags();
    mask &= ~std::ios_base::scientific;
    mask |=  std::ios_base::fixed;
    oss << std::setprecision(2) << std::setiosflags(mask)
        << std::setw(level*2) << std::setfill(' ') << "";

    std::string trafoMatrix = Debug::MatrixToString(node.GetWorldMatrix(updateBufferIndex), 2, level*2);
    DALI_LOG_INFO(gNodeLogFilter, Debug::Verbose, "%s\n", trafoMatrix.c_str());
  }

  ++level;

  for ( NodeConstIter iter = node.GetChildren().Begin(); iter != node.GetChildren().End(); ++iter )
  {
    PrintNodes(**iter, updateBufferIndex, level);
  }
}

void PrintNodeTree( const Node& node, BufferIndex bufferIndex, std::string indentation )
{
  std::cout << "Node " << &node
            << " \"" << node.mDebugString << "\""
            << " Origin: "       << node.GetParentOrigin()
            << " Anchor: "       << node.GetAnchorPoint()
            << " Size: "         << node.GetSize(bufferIndex)
            << " Pos: "          << node.GetPosition(bufferIndex)
            << " Ori: "          << node.GetOrientation(bufferIndex)
            << " Scale: "        << node.GetScale(bufferIndex)
            << " Color: "        << node.GetColor(bufferIndex)
            << " Visible: "      << node.IsVisible(bufferIndex)
            << " World Pos: "    << node.GetWorldPosition(bufferIndex)
            << " World Ori: "    << node.GetWorldOrientation(bufferIndex)
            << " World Scale: "  << node.GetWorldScale(bufferIndex)
            << " World Color: "  << node.GetWorldColor(bufferIndex)
            << " World Matrix: " << node.GetWorldMatrix(bufferIndex)
            << std::endl;

  for ( NodeConstIter iter = node.GetChildren().Begin(); iter != node.GetChildren().End(); ++iter)
  {
    std::cout << indentation << "|" << std::endl
              << indentation << "---->";

    std::string nextIndent = indentation;
    if ( (iter + 1) != node.GetChildren().End() )
    {
      nextIndent += "|    ";
    }
    else
    {
      nextIndent += "     ";
    }

    PrintNodeTree(**iter, bufferIndex, nextIndent);
  }
}

} // SceneGraph

} // Internal

} // Dali

#endif
