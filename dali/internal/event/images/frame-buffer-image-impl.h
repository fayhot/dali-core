#ifndef DALI_INTERNAL_FRAME_BUFFER_IMAGE_H
#define DALI_INTERNAL_FRAME_BUFFER_IMAGE_H

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

// INTERNAL INCLUDES
#include <dali/internal/event/images/image-impl.h>
#include <dali/public-api/images/frame-buffer-image.h>
#include <dali/internal/event/rendering/frame-buffer-impl.h>

namespace Dali
{

namespace Internal
{

class FrameBufferImage;
typedef IntrusivePtr<FrameBufferImage> FrameBufferImagePtr;

/**
 * @copydoc Dali::FrameBufferImage
 */
class FrameBufferImage : public Image
{
public:

  /**
   * @copydoc Dali::FrameBufferImage::New(unsigned int, unsigned int, Pixel::Format)
   */
  static FrameBufferImagePtr New( unsigned int width, unsigned int height, Pixel::Format pixelFormat, RenderBuffer::Format bufferformat );

  /**
   * @copydoc Dali::FrameBufferImage::New(NativeImageInterface&)
   */
  static FrameBufferImagePtr New( NativeImageInterface& nativeImage );

  /**
   * @copydoc Dali::FrameBufferImage::FrameBufferImage
   */
  FrameBufferImage( unsigned int width, unsigned int height );

  /**
   * @copydoc Dali::FrameBufferImage::FrameBufferImage
   */
  FrameBufferImage( NativeImageInterface& image );

  /**
   * @return true if this FBO is targeting a NativeImageInterface
   */
  bool IsNativeFbo() const;

  /**
   * @return Internal FrameBuffer used by this FrameBufferImage
   */
  FrameBuffer* GetFrameBuffer() const
  {
    return mFrameBufferObject.Get();
  }

protected:
  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~FrameBufferImage();

private:
  // cached values for the size / pixel format we were created with. Needed to recreate us when we Connect() to stage and mTicket was reset from a previous call to Disconnect().
  FrameBufferPtr  mFrameBufferObject;
  NativeImageInterfacePtr mNativeImage;
  bool mIsNativeFbo;
}; // class FrameBufferImage

} // namespace Internal

/**
 * Helper methods for public API.
 */
inline Internal::FrameBufferImage& GetImplementation(Dali::FrameBufferImage& image)
{
  DALI_ASSERT_ALWAYS( image && "FrameBufferImage handle is empty" );

  BaseObject& handle = image.GetBaseObject();

  return static_cast<Internal::FrameBufferImage&>(handle);
}

inline const Internal::FrameBufferImage& GetImplementation(const Dali::FrameBufferImage& image)
{
  DALI_ASSERT_ALWAYS( image && "FrameBufferImage handle is empty" );

  const BaseObject& handle = image.GetBaseObject();

  return static_cast<const Internal::FrameBufferImage&>(handle);
}

} // namespace Dali

#endif // DALI_INTERNAL_FRAME_BUFFER_IMAGE_H
