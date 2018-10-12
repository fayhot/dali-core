/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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
#include <dali/internal/update/rendering/render-instruction-container.h>

// INTERNAL INCLUDES
#include <dali/internal/update/rendering/render-instruction.h>

namespace Dali
{

namespace Internal
{

namespace SceneGraph
{

RenderInstructionContainer::RenderInstructionContainer()
: mInstructions{}
{
}

RenderInstructionContainer::~RenderInstructionContainer()
{
}

void RenderInstructionContainer::ResetAndReserve( BufferIndex bufferIndex, size_t capacityRequired )
{
  // Only re-allocate if necessary.
  if( mInstructions.size() < capacityRequired )
  {
    mInstructions.reserve( capacityRequired );
  }
  mInstructions.clear();
}

size_t RenderInstructionContainer::Count( BufferIndex bufferIndex )
{
  return mInstructions.size();
}

RenderInstruction& RenderInstructionContainer::At( BufferIndex bufferIndex, size_t index )
{
  DALI_ASSERT_DEBUG( index < mInstructions.size() );

  return *mInstructions[ index ];
}

void RenderInstructionContainer::PushBack( BufferIndex index, RenderInstruction* renderInstruction )
{
  mInstructions.push_back( renderInstruction );
}

void RenderInstructionContainer::DiscardCurrentInstruction( BufferIndex updateBufferIndex )
{
  mInstructions.pop_back();
}


} // namespace SceneGraph

} // namespace Internal

} // namespace Dali