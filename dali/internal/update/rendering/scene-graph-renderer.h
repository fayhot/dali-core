#ifndef DALI_INTERNAL_SCENE_GRAPH_RENDERER_H
#define DALI_INTERNAL_SCENE_GRAPH_RENDERER_H

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
 */

#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/rendering/renderer.h> // Dali::Renderer
#include <dali/devel-api/rendering/renderer-devel.h>
#include <dali/internal/common/blending-options.h>
#include <dali/internal/common/type-abstraction-enums.h>
#include <dali/internal/event/common/event-thread-services.h>
#include <dali/internal/update/common/property-owner.h>
#include <dali/internal/update/common/uniform-map.h>
#include <dali/internal/update/common/scene-graph-connection-change-propagator.h>
#include <dali/internal/update/common/animatable-property.h>
#include <dali/internal/render/data-providers/render-data-provider.h>
#include <dali/internal/render/renderers/render-renderer.h>

namespace Dali
{

namespace Internal
{

namespace Render
{
class Renderer;
class Geometry;
}

namespace SceneGraph
{
class SceneController;

class Renderer;
typedef Dali::Vector< Renderer* > RendererContainer;
typedef RendererContainer::Iterator RendererIter;
typedef RendererContainer::ConstIterator RendererConstIter;

class TextureSet;
class Geometry;

class Renderer :  public PropertyOwner,
                  public UniformMapDataProvider,
                  public UniformMap::Observer,
                  public ConnectionChangePropagator::Observer
{
public:

  enum OpacityType
  {
    OPAQUE,
    TRANSPARENT,
    TRANSLUCENT
  };

  /**
   * Construct a new Renderer
   */
  static Renderer* New();

  /**
   * Destructor
   */
  virtual ~Renderer();

  /**
   * Overriden delete operator
   * Deletes the renderer from its global memory pool
   */
  void operator delete( void* ptr );

  /**
   * Set the texture set for the renderer
   * @param[in] textureSet The texture set this renderer will use
   */
  void SetTextures( TextureSet* textureSet );

  /**
   * Returns current texture set object
   * @return Pointer to the texture set
   */
  const TextureSet* GetTextures() const
  {
    return mTextureSet;
  }

  /**
   * Set the shader for the renderer
   * @param[in] shader The shader this renderer will use
   */
  void SetShader( Shader* shader );

  /**
   * Get the shader used by this renderer
   * @return the shader this renderer uses
   */
  const Shader& GetShader() const
  {
    return *mShader;
  }

  /**
   * Set the geometry for the renderer
   * @param[in] geometry The geometry this renderer will use
   */
  void SetGeometry( Render::Geometry* geometry );

  /**
   * Set the depth index
   * @param[in] depthIndex the new depth index to use
   */
  void SetDepthIndex( int depthIndex );

  /**
   * @brief Get the depth index
   * @return The depth index
   */
  int GetDepthIndex() const
  {
    return mDepthIndex;
  }

  /**
   * Set the face culling mode
   * @param[in] faceCullingMode to use
   */
  void SetFaceCullingMode( FaceCullingMode::Type faceCullingMode );

  /**
   * Get face culling mode
   * @return The face culling mode
   */
  FaceCullingMode::Type GetFaceCullingMode() const;

  /**
   * Set the blending mode
   * @param[in] blendingMode to use
   */
  void SetBlendMode( BlendMode::Type blendingMode );

  /**
   * Get the blending mode
   * @return The the blending mode
   */
  BlendMode::Type GetBlendMode() const;

  /**
   * Set the blending options. This should only be called from the update thread.
   * @param[in] options A bitmask of blending options.
   */
  void SetBlendingOptions( uint32_t options );

  /**
   * Get the blending options
   * @return The the blending mode
   */
  uint32_t GetBlendingOptions() const;

  /**
   * Set the blend color for blending operation
   * @param blendColor to pass to GL
   */
  void SetBlendColor( const Vector4& blendColor );

  /**
   * Get the blending color
   * @return The blend color
   */
  Vector4 GetBlendColor() const;

  /**
   * Set the index of first element for indexed draw
   * @param[in] firstElement index of first element to draw
   */
  void SetIndexedDrawFirstElement( uint32_t firstElement );

  /**
   * Get the index of first element for indexed draw
   * @return The index of first element for indexed draw
   */
  uint32_t GetIndexedDrawFirstElement() const;

  /**
   * Set the number of elements to draw by indexed draw
   * @param[in] elementsCount number of elements to draw
   */
  void SetIndexedDrawElementsCount( uint32_t elementsCount );

  /**
   * Get the number of elements to draw by indexed draw
   * @return The number of elements to draw by indexed draw
   */
  uint32_t GetIndexedDrawElementsCount() const;

  /**
   * @brief Set whether the Pre-multiplied Alpha Blending is required
   * @param[in] preMultipled whether alpha is pre-multiplied.
   */
  void EnablePreMultipliedAlpha( bool preMultipled );

  /**
   * @brief Query whether alpha is pre-multiplied.
   * @return True is alpha is pre-multiplied, false otherwise.
   */
  bool IsPreMultipliedAlphaEnabled() const;

  /**
   * Sets the depth buffer write mode
   * @param[in] depthWriteMode The depth buffer write mode
   */
  void SetDepthWriteMode( DepthWriteMode::Type depthWriteMode );

  /**
   * Get the depth buffer write mode
   * @return The depth buffer write mode
   */
  DepthWriteMode::Type GetDepthWriteMode() const;

  /**
   * Sets the depth buffer test mode
   * @param[in] depthTestMode The depth buffer test mode
   */
  void SetDepthTestMode( DepthTestMode::Type depthTestMode );

  /**
   * Get the depth buffer test mode
   * @return The depth buffer test mode
   */
  DepthTestMode::Type GetDepthTestMode() const;

  /**
   * Sets the depth function
   * @param[in] depthFunction The depth function
   */
  void SetDepthFunction( DepthFunction::Type depthFunction );

  /**
   * Get the depth function
   * @return The depth function
   */
  DepthFunction::Type GetDepthFunction() const;

  /**
   * Sets the render mode
   * @param[in] mode The render mode
   */
  void SetRenderMode( RenderMode::Type mode );

  /**
   * Sets the stencil function
   * @param[in] stencilFunction The stencil function
   */
  void SetStencilFunction( StencilFunction::Type stencilFunction );

  /**
   * Sets the stencil function mask
   * @param[in] stencilFunctionMask The stencil function mask
   */
  void SetStencilFunctionMask( int stencilFunctionMask );

  /**
   * Sets the stencil function reference
   * @param[in] stencilFunctionReference The stencil function reference
   */
  void SetStencilFunctionReference( int stencilFunctionReference );

  /**
   * Sets the stencil mask
   * @param[in] stencilMask The stencil mask
   */
  void SetStencilMask( int stencilMask );

  /**
   * Sets the stencil operation for when the stencil test fails
   * @param[in] stencilOperationOnFail The stencil operation
   */
  void SetStencilOperationOnFail( StencilOperation::Type stencilOperationOnFail );

  /**
   * Sets the stencil operation for when the depth test fails
   * @param[in] stencilOperationOnZFail The stencil operation
   */
  void SetStencilOperationOnZFail( StencilOperation::Type stencilOperationOnZFail );

  /**
   * Sets the stencil operation for when the depth test passes
   * @param[in] stencilOperationOnZPass The stencil operation
   */
  void SetStencilOperationOnZPass( StencilOperation::Type stencilOperationOnZPass );

  /**
   * Gets the stencil parameters
   * @return The stencil parameters
   */
  const Render::Renderer::StencilParameters& GetStencilParameters() const;

  /**
   * Bakes the opacity
   * @param[in] updateBufferIndex The current update buffer index.
   * @param[in] opacity The opacity
   */
  void BakeOpacity( BufferIndex updateBufferIndex, float opacity );

  /**
   * Gets the opacity
   * @param[in] bufferIndex The buffer to read from.
   * @return The opacity
   */
  float GetOpacity( BufferIndex updateBufferIndex ) const;

  /**
   * Sets the rendering behavior
   * @param[in] renderingBehavior The rendering behavior required.
   */
  void SetRenderingBehavior( DevelRenderer::Rendering::Type renderingBehavior );

  /**
   * Gets the rendering behavior
   * @return The rendering behavior
   */
  DevelRenderer::Rendering::Type GetRenderingBehavior() const;

  /**
   * Prepare the object for rendering.
   * This is called by the UpdateManager when an object is due to be rendered in the current frame.
   * @param[in] updateBufferIndex The current update buffer index.
   */
  void PrepareRender( BufferIndex updateBufferIndex );

  /**
   * Retrieve the Render thread renderer
   * @return The associated render thread renderer
   */
  Render::Renderer& GetRenderer();

  /**
   * Query whether the renderer is fully opaque, fully transparent or transparent.
   * @param[in] updateBufferIndex The current update buffer index.
   * @return OPAQUE if fully opaque, TRANSPARENT if fully transparent and TRANSLUCENT if in between
   */
  OpacityType GetOpacityType( BufferIndex updateBufferIndex, const Node& node ) const;

  /**
   * Called by the TextureSet to notify to the renderer that it has changed
   */
  void TextureSetChanged();

  /**
   * Called by the TextureSet to notify to the renderer that it is about to be deleted
   */
  void TextureSetDeleted();

  /**
   * Connect the object to the scene graph
   *
   * @param[in] sceneController The scene controller - used for sending messages to render thread
   * @param[in] bufferIndex The current buffer index - used for sending messages to render thread
   */
  void ConnectToSceneGraph( SceneController& sceneController, BufferIndex bufferIndex );

  /**
   * Disconnect the object from the scene graph
   * @param[in] sceneController The scene controller - used for sending messages to render thread
   * @param[in] bufferIndex The current buffer index - used for sending messages to render thread
   */
  void DisconnectFromSceneGraph( SceneController& sceneController, BufferIndex bufferIndex );

public: // Implementation of ConnectionChangePropagator
  /**
   * @copydoc ConnectionChangePropagator::AddObserver
   */
  void AddConnectionObserver(ConnectionChangePropagator::Observer& observer){};

  /**
   * @copydoc ConnectionChangePropagator::RemoveObserver
   */
  void RemoveConnectionObserver(ConnectionChangePropagator::Observer& observer){};

public: // UniformMap::Observer
  /**
   * @copydoc UniformMap::Observer::UniformMappingsChanged
   */
  virtual void UniformMappingsChanged( const UniformMap& mappings );

public: // ConnectionChangePropagator::Observer

  /**
   * @copydoc ConnectionChangePropagator::ConnectionsChanged
   */
  virtual void ConnectionsChanged( PropertyOwner& owner );

  /**
   * @copydoc ConnectionChangePropagator::ConnectedUniformMapChanged
   */
  virtual void ConnectedUniformMapChanged( );

  /**
   * @copydoc ConnectionChangePropagator::ConnectedUniformMapChanged
   */
  virtual void ObservedObjectDestroyed(PropertyOwner& owner);

public: // PropertyOwner implementation
  /**
   * @copydoc Dali::Internal::SceneGraph::PropertyOwner::ResetDefaultProperties()
   */
  virtual void ResetDefaultProperties( BufferIndex updateBufferIndex ){};

public: // From UniformMapDataProvider

  /**
   * @copydoc UniformMapDataProvider::GetUniformMapChanged
   */
  virtual bool GetUniformMapChanged( BufferIndex bufferIndex ) const{ return mUniformMapChanged[bufferIndex];}

  /**
   * @copydoc UniformMapDataProvider::GetUniformMap
   */
  virtual const CollectedUniformMap& GetUniformMap( BufferIndex bufferIndex ) const;

private:

  /**
   * Protected constructor; See also Renderer::New()
   */
  Renderer();

  /**
   * Update texture set to the render data provider
   */
  void UpdateTextureSet();

private:

  CollectedUniformMap          mCollectedUniformMap[2];           ///< Uniform maps collected by the renderer

  SceneController*             mSceneController;                  ///< Used for initializing renderers
  Render::Renderer*            mRenderer;                         ///< Raw pointer to the renderer (that's owned by RenderManager)
  TextureSet*                  mTextureSet;                       ///< The texture set this renderer uses. (Not owned)
  Render::Geometry*            mGeometry;                         ///< The geometry this renderer uses. (Not owned)
  Shader*                      mShader;                           ///< The shader this renderer uses. (Not owned)
  RenderDataProvider*          mRenderDataProvider;               ///< The render data provider
  OwnerPointer< Vector4 >      mBlendColor;                       ///< The blend color for blending operation

  Dali::Internal::Render::Renderer::StencilParameters mStencilParameters;         ///< Struct containing all stencil related options

  uint32_t                     mIndexedDrawFirstElement;          ///< first element index to be drawn using indexed draw
  uint32_t                     mIndexedDrawElementsCount;         ///< number of elements to be drawn using indexed draw
  uint32_t                     mBlendBitmask;                     ///< The bitmask of blending options
  uint32_t                     mRegenerateUniformMap;             ///< 2 if the map should be regenerated, 1 if it should be copied.
  uint32_t                     mResendFlag;                       ///< Indicate whether data should be resent to the renderer

  DepthFunction::Type          mDepthFunction:4;                  ///< Local copy of the depth function
  FaceCullingMode::Type        mFaceCullingMode:3;                ///< Local copy of the mode of face culling
  BlendMode::Type              mBlendMode:3;                      ///< Local copy of the mode of blending
  DepthWriteMode::Type         mDepthWriteMode:3;                 ///< Local copy of the depth write mode
  DepthTestMode::Type          mDepthTestMode:3;                  ///< Local copy of the depth test mode
  DevelRenderer::Rendering::Type mRenderingBehavior:2;            ///< The rendering behavior
  bool                         mUniformMapChanged[2];             ///< Records if the uniform map has been altered this frame
  bool                         mPremultipledAlphaEnabled:1;       ///< Flag indicating whether the Pre-multiplied Alpha Blending is required

public:

  AnimatableProperty< float >  mOpacity;                          ///< The opacity value
  int32_t                      mDepthIndex;                       ///< Used only in PrepareRenderInstructions

};


/// Messages
inline void SetTexturesMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, const TextureSet& textureSet )
{
  typedef MessageValue1< Renderer, TextureSet* > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  // Construct message in the message queue memory; note that delete should not be called on the return value
  new (slot) LocalType( &renderer, &Renderer::SetTextures, const_cast<TextureSet*>(&textureSet) );
}

inline void SetGeometryMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, const Render::Geometry& geometry )
{
  typedef MessageValue1< Renderer, Render::Geometry* > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  // Construct message in the message queue memory; note that delete should not be called on the return value
  new (slot) LocalType( &renderer, &Renderer::SetGeometry, const_cast<Render::Geometry*>(&geometry) );
}

inline void SetShaderMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, const Shader& shader )
{
  typedef MessageValue1< Renderer, Shader* > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  // Construct message in the message queue memory; note that delete should not be called on the return value
  new (slot) LocalType( &renderer, &Renderer::SetShader, const_cast<Shader*>( &shader ) );
}

inline void SetDepthIndexMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, int depthIndex )
{
  typedef MessageValue1< Renderer, int > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  // Construct message in the message queue memory; note that delete should not be called on the return value
  new (slot) LocalType( &renderer, &Renderer::SetDepthIndex, depthIndex );
}

inline void SetFaceCullingModeMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, FaceCullingMode::Type faceCullingMode )
{
  typedef MessageValue1< Renderer, FaceCullingMode::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetFaceCullingMode, faceCullingMode );
}

inline void SetBlendModeMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, BlendMode::Type blendingMode )
{
  typedef MessageValue1< Renderer, BlendMode::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetBlendMode, blendingMode );
}

inline void SetBlendingOptionsMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, uint32_t options )
{
  typedef MessageValue1< Renderer, uint32_t > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetBlendingOptions, options );
}

inline void SetBlendColorMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, const Vector4& blendColor )
{
  typedef MessageValue1< Renderer, Vector4 > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetBlendColor, blendColor );
}

inline void SetIndexedDrawFirstElementMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, uint32_t firstElement )
{
  typedef MessageValue1< Renderer, uint32_t > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetIndexedDrawFirstElement, firstElement );
}

inline void SetIndexedDrawElementsCountMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, uint32_t elementsCount )
{
  typedef MessageValue1< Renderer, uint32_t > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetIndexedDrawElementsCount, elementsCount );
}

inline void SetEnablePreMultipliedAlphaMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, bool preMultiplied )
{
  typedef MessageValue1< Renderer, bool > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::EnablePreMultipliedAlpha, preMultiplied );
}

inline void SetDepthWriteModeMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, DepthWriteMode::Type depthWriteMode )
{
  typedef MessageValue1< Renderer, DepthWriteMode::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetDepthWriteMode, depthWriteMode );
}

inline void SetDepthTestModeMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, DepthTestMode::Type depthTestMode )
{
  typedef MessageValue1< Renderer, DepthTestMode::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetDepthTestMode, depthTestMode );
}

inline void SetDepthFunctionMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, DepthFunction::Type depthFunction )
{
  typedef MessageValue1< Renderer, DepthFunction::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetDepthFunction, depthFunction );
}

inline void SetRenderModeMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, RenderMode::Type mode )
{
  typedef MessageValue1< Renderer, RenderMode::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetRenderMode, mode );
}

inline void SetStencilFunctionMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, StencilFunction::Type stencilFunction )
{
  typedef MessageValue1< Renderer, StencilFunction::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilFunction, stencilFunction );
}

inline void SetStencilFunctionMaskMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, int mask )
{
  typedef MessageValue1< Renderer, int > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilFunctionMask, mask );
}

inline void SetStencilFunctionReferenceMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, int stencilFunctionReference )
{
  typedef MessageValue1< Renderer, int > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilFunctionReference, stencilFunctionReference );
}

inline void SetStencilMaskMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, int stencilMask )
{
  typedef MessageValue1< Renderer, int > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilMask, stencilMask );
}

inline void SetStencilOperationOnFailMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, StencilOperation::Type stencilOperation )
{
  typedef MessageValue1< Renderer, StencilOperation::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilOperationOnFail, stencilOperation );
}

inline void SetStencilOperationOnZFailMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, StencilOperation::Type stencilOperation )
{
  typedef MessageValue1< Renderer, StencilOperation::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilOperationOnZFail, stencilOperation );
}

inline void SetStencilOperationOnZPassMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, StencilOperation::Type stencilOperation )
{
  typedef MessageValue1< Renderer, StencilOperation::Type > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetStencilOperationOnZPass, stencilOperation );
}

inline void BakeOpacityMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, float opacity )
{
  typedef MessageDoubleBuffered1< Renderer, float > LocalType;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::BakeOpacity, opacity );
}

inline void SetRenderingBehaviorMessage( EventThreadServices& eventThreadServices, const Renderer& renderer, DevelRenderer::Rendering::Type renderingBehavior )
{
  using LocalType = MessageValue1< Renderer, DevelRenderer::Rendering::Type >;

  // Reserve some memory inside the message queue
  uint32_t* slot = eventThreadServices.ReserveMessageSlot( sizeof( LocalType ) );

  new (slot) LocalType( &renderer, &Renderer::SetRenderingBehavior, renderingBehavior );
}

} // namespace SceneGraph
} // namespace Internal
} // namespace Dali

#endif //  DALI_INTERNAL_SCENE_GRAPH_RENDERER_H
