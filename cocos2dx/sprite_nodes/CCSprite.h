/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __SPITE_NODE_CCSPRITE_H__
#define __SPITE_NODE_CCSPRITE_H__

#include "base_nodes/CCNode.h"
#include "CCProtocols.h"
#include "textures/CCTextureAtlas.h"
#include "ccTypes.h"
#include "cocoa/CCDictionary.h"
#include <string>
#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class SpriteBatchNode;
class SpriteFrame;
class Animation;
class Rect;
class Point;
class Size;
class Texture2D;
struct transformValues_;

/**
 * @addtogroup sprite_nodes
 * @{
 */

/** 
 * Sprite is a 2d image ( http://en.wikipedia.org/wiki/Sprite_(computer_graphics) )
 *
 * Sprite can be created with an image, or with a sub-rectangle of an image.
 *
 * If the parent or any of its ancestors is a SpriteBatchNode then the following features/limitations are valid
 *    - Features when the parent is a BatchNode:
 *        - MUCH faster rendering, specially if the SpriteBatchNode has many children. All the children will be drawn in a single batch.
 *
 *    - Limitations
 *        - Camera is not supported yet (eg: OrbitCamera action doesn't work)
 *        - GridBase actions are not supported (eg: Lens, Ripple, Twirl)
 *        - The Alias/Antialias property belongs to SpriteBatchNode, so you can't individually set the aliased property.
 *        - The Blending function property belongs to SpriteBatchNode, so you can't individually set the blending function property.
 *        - Parallax scroller is not supported, but can be simulated with a "proxy" sprite.
 *
 *  If the parent is an standard Node, then Sprite behaves like any other Node:
 *    - It supports blending functions
 *    - It supports aliasing / antialiasing
 *    - But the rendering will be slower: 1 draw per children.
 *
 * The default anchorPoint in Sprite is (0.5, 0.5).
 */
class CC_DLL Sprite : public NodeRGBA, public TextureProtocol
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
public:

    static const int kSpriteIndexNotInitialized = -1; /// Sprite invalid index on the SpriteBatchNode

    /// @{
    /// @name Creators
    
    /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @return An empty sprite object that is marked as autoreleased.
     */
    static Sprite* create();
    
    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   filename The string which indicates a path to image file, e.g., "scene1/monster.png".
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Sprite* create(const char *filename);
    
    /**
     * Creates a sprite with an image filename and a rect.
     *
     * @param   filename The string wich indicates a path to image file, e.g., "scene1/monster.png"
     * @param   rect        Only the contents inside rect of filename's texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Sprite* create(const char *filename, const Rect& rect);
    
    /**
     * Creates a sprite with an exsiting texture contained in a Texture2D object
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   texture    A pointer to a Texture2D object.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Sprite* createWithTexture(Texture2D *texture);
    
    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   texture    A pointer to an existing Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Sprite* createWithTexture(Texture2D *texture, const Rect& rect);
    
    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   pSpriteFrame    A sprite frame which involves a texture and a rect
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Sprite* createWithSpriteFrame(SpriteFrame *pSpriteFrame);
    
    /**
     * Creates a sprite with an sprite frame name.
     *
     * A SpriteFrame will be fetched from the SpriteFrameCache by spriteFrameName param.
     * If the SpriteFrame doesn't exist it will raise an exception.
     *
     * @param   spriteFrameName A null terminated string which indicates the sprite frame name.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Sprite* createWithSpriteFrameName(const char *spriteFrameName);
    
    /// @}  end of creators group
    
    
    /// @{
    /// @name Initializers
    
    /**
     * Default constructor
     */
    Sprite(void);
    
    /**
     * Default destructor
     */
    virtual ~Sprite(void);
    
    /**
     * Initializes an empty sprite with nothing init.
     */
    virtual bool init(void);
    
    /**
     * Initializes a sprite with a texture.
     *
     * After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
     *
     * @param   texture    A pointer to an existing Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(Texture2D *texture);
    
    /**
     * Initializes a sprite with a texture and a rect.
     *
     * After initialization, the offset will be (0,0).
     *
     * @param   texture    A pointer to an exisiting Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @param   rect        Only the contents inside rect of this texture will be applied for this sprite.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(Texture2D *texture, const Rect& rect);
    
    /**
     * Initializes a sprite with a texture and a rect in points, optionally rotated.
     *
     * After initialization, the offset will be (0,0).
     * @note    This is the designated initializer.
     *
     * @param   texture    A Texture2D object whose texture will be applied to this sprite.
     * @param   rect        A rectangle assigned the contents of texture.
     * @param   rotated     Whether or not the texture rectangle is rotated.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(Texture2D *texture, const Rect& rect, bool rotated);
    
    /**
     * Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite
     *
     * @param   pSpriteFrame  A SpriteFrame object. It should includes a valid texture and a rect
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithSpriteFrame(SpriteFrame *pSpriteFrame);
    
    /**
     * Initializes a sprite with an sprite frame name.
     *
     * A SpriteFrame will be fetched from the SpriteFrameCache by name.
     * If the SpriteFrame doesn't exist it will raise an exception.
     *
     * @param   spriteFrameName  A key string that can fected a volid SpriteFrame from SpriteFrameCache
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithSpriteFrameName(const char *spriteFrameName);
    
    /**
     * Initializes a sprite with an image filename.
     *
     * This method will find filename from local file system, load its content to Texture2D,
     * then use Texture2D to create a sprite.
     * After initialization, the rect used will be the size of the image. The offset will be (0,0).
     *
     * @param   filename The path to an image file in local file system
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithFile(const char *filename);
    
    /**
     * Initializes a sprite with an image filename, and a rect.
     *
     * This method will find filename from local file system, load its content to Texture2D,
     * then use Texture2D to create a sprite.
     * After initialization, the offset will be (0,0).
     *
     * @param   filename The path to an image file in local file system.
     * @param   rect        The rectangle assigned the content area from texture.
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithFile(const char *filename, const Rect& rect);
    
    /// @} end of initializers

    /// @{
    /// @name BatchNode methods
    
    /**
     * Updates the quad according the rotation, position, scale values. 
     */
    virtual void updateTransform(void);
    
    /**
     * Returns the batch node object if this sprite is rendered by SpriteBatchNode
     *
     * @return The SpriteBatchNode object if this sprite is rendered by SpriteBatchNode,
     *         NULL if the sprite isn't used batch node.
     */
    virtual SpriteBatchNode* getBatchNode(void);
    /**
     * Sets the batch node to sprite
     * @warning This method is not recommended for game developers. Sample code for using batch node
     * @code
     * SpriteBatchNode *batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
     * Sprite *sprite = Sprite::createWithTexture(batch->getTexture(), Rect(0, 0, 57, 57));
     * batch->addChild(sprite);
     * layer->addChild(batch);
     * @endcode
     */
    virtual void setBatchNode(SpriteBatchNode *spriteBatchNode);
     
    /// @} end of BatchNode methods
    
    
    
    /// @{
    /// @name Texture methods
    
    /**
     * Updates the texture rect of the Sprite in points.
     * It will call setTextureRect:rotated:untrimmedSize with rotated = NO, and utrimmedSize = rect.size.
     */
    virtual void setTextureRect(const Rect& rect);
    
    /**
     * Sets the texture rect, rectRotated and untrimmed size of the Sprite in points.
     * It will update the texture coordinates and the vertex rectangle.
     */
    virtual void setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize);
    
    /**
     * Sets the vertex rect.
     * It will be called internally by setTextureRect.
     * Useful if you want to create 2x images from SD images in Retina Display.
     * Do not call it manually. Use setTextureRect instead.
     */
    virtual void setVertexRect(const Rect& rect);
    
    /// @} end of texture methods
    

    
    /// @{
    /// @name Frames methods
    
    /**
     * Sets a new display frame to the Sprite.
     */
    virtual void setDisplayFrame(SpriteFrame *pNewFrame);
    
    /**
     * Returns whether or not a SpriteFrame is being displayed
     */
    virtual bool isFrameDisplayed(SpriteFrame *pFrame) const;
    
    /**
     * Returns the current displayed frame.
     */
    virtual SpriteFrame* displayFrame(void);
    
    /// @} End of frames methods
    

    /// @{
    /// @name Animation methods
    /**
     * Changes the display frame with animation name and index.
     * The animation name will be get from the AnimationCache
     */
    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex);
    /// @}
    
    
    /// @{
    /// @name Sprite Properties' setter/getters
    
    /** 
     * Whether or not the Sprite needs to be updated in the Atlas.
     *
     * @return true if the sprite needs to be updated in the Atlas, false otherwise.
     */
    virtual bool isDirty(void) const { return _dirty; }
    
    /** 
     * Makes the Sprite to be updated in the Atlas.
     */
    virtual void setDirty(bool bDirty) { _dirty = bDirty; }
    
    /**
     * Returns the quad (tex coords, vertex coords and color) information. 
     */
    inline V3F_C4B_T2F_Quad getQuad(void) const { return _quad; }

    /** 
     * Returns whether or not the texture rectangle is rotated.
     */
    inline bool isTextureRectRotated(void) const { return _rectRotated; }
    
    /** 
     * Returns the index used on the TextureAtlas. 
     */
    inline int getAtlasIndex(void) const { return _atlasIndex; }
    
    /** 
     * Sets the index used on the TextureAtlas.
     * @warning Don't modify this value unless you know what you are doing
     */
    inline void setAtlasIndex(int atlasIndex) { _atlasIndex = atlasIndex; }

    /** 
     * Returns the rect of the Sprite in points 
     */
    inline const Rect& getTextureRect(void) { return _rect; }

    /**
     * Gets the weak reference of the TextureAtlas when the sprite is rendered using via SpriteBatchNode
     */
    inline TextureAtlas* getTextureAtlas(void) { return _textureAtlas; }
    
    /**
     * Sets the weak reference of the TextureAtlas when the sprite is rendered using via SpriteBatchNode
     */
    inline void setTextureAtlas(TextureAtlas *pobTextureAtlas) { _textureAtlas = pobTextureAtlas; }

    /** 
     * Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
     */
    inline const Point& getOffsetPosition(void) const { return _offsetPosition; }


    /** 
     * Returns the flag which indicates whether the sprite is flipped horizontally or not.
     *
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleX(sprite->getScaleX() * -1);
     *
     * @return true if the sprite is flipped horizaontally, false otherwise.
     */
    bool isFlipX(void) const;
    /**
     * Sets whether the sprite should be flipped horizontally or not.
     *
     * @param bFlipX true if the sprite should be flipped horizaontally, false otherwise.
     */
    void setFlipX(bool bFlipX);
    
    /** 
     * Return the flag which indicates whether the sprite is flipped vertically or not.
     * 
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleY(sprite->getScaleY() * -1);
     * 
     * @return true if the sprite is flipped vertically, flase otherwise.
     */
    bool isFlipY(void) const;
    /**
     * Sets whether the sprite should be flipped vertically or not.
     *
     * @param bFlipY true if the sprite should be flipped vertically, flase otherwise.
     */
    void setFlipY(bool bFlipY);
    
    /// @} End of Sprite properties getter/setters


    //
    // Overrides
    //
    /// @{
    /// @name Functions inherited from TextureProtocol
    virtual void setTexture(Texture2D *texture) override;
    virtual Texture2D* getTexture() const override;
    inline void setBlendFunc(const BlendFunc &blendFunc) override { _blendFunc = blendFunc; }
    inline const BlendFunc& getBlendFunc() const override { return _blendFunc; }
    /// @}

    /// @{
    /// @name Functions inherited from Node
    virtual void setScaleX(float scaleX) override;
    virtual void setScaleY(float scaleY) override;
    virtual void setPosition(const Point& pos) override;
    virtual void setRotation(float fRotation) override;
    virtual void setRotationX(float fRotationX) override;
    virtual void setRotationY(float fRotationY) override;
    virtual void setSkewX(float sx) override;
    virtual void setSkewY(float sy) override;
    virtual void removeChild(Node* child, bool bCleanup) override;
    virtual void removeAllChildrenWithCleanup(bool bCleanup) override;
    virtual void reorderChild(Node *child, int zOrder) override;
    virtual void addChild(Node *child) override;
    virtual void addChild(Node *child, int zOrder) override;
    virtual void addChild(Node *child, int zOrder, int tag) override;
    virtual void sortAllChildren() override;
    virtual void setScale(float fScale) override;
    virtual void setVertexZ(float fVertexZ) override;
    virtual void setAnchorPoint(const Point& anchor) override;
    virtual void ignoreAnchorPointForPosition(bool value) override;
    virtual void setVisible(bool bVisible) override;
    virtual void draw(void) override;
    /// @}

    /// @{
    /// @name Functions inherited from NodeRGBA
    virtual void setColor(const Color3B& color3) override;
    virtual void updateDisplayedColor(const Color3B& parentColor) override;
    virtual void setOpacity(GLubyte opacity) override;
    virtual void setOpacityModifyRGB(bool modify) override;
    virtual bool isOpacityModifyRGB(void) const override;
    virtual void updateDisplayedOpacity(GLubyte parentOpacity) override;
    /// @}

protected:
    void updateColor(void);
    virtual void setTextureCoords(Rect rect);
    virtual void updateBlendFunc(void);
    virtual void setReorderChildDirtyRecursively(void);
    virtual void setDirtyRecursively(bool bValue);

    //
    // Data used when the sprite is rendered using a SpriteSheet
    //
    TextureAtlas*       _textureAtlas;      /// SpriteBatchNode texture atlas (weak reference)
    int                 _atlasIndex;        /// Absolute (real) Index on the SpriteSheet
    SpriteBatchNode*    _batchNode;         /// Used batch node (weak reference)
    
    bool                _dirty;             /// Whether the sprite needs to be updated
    bool                _recursiveDirty;    /// Whether all of the sprite's children needs to be updated
    bool                _hasChildren;       /// Whether the sprite contains children
    bool                _shouldBeHidden;    /// should not be drawn because one of the ancestors is not visible
    AffineTransform     _transformToBatch;
    
    //
    // Data used when the sprite is self-rendered
    //
    BlendFunc        _blendFunc;            /// It's required for TextureProtocol inheritance
    Texture2D*       _texture;              /// Texture2D object that is used to render the sprite

    //
    // Shared data
    //

    // texture
    Rect _rect;                             /// Retangle of Texture2D
    bool   _rectRotated;                    /// Whether the texture is rotated

    // Offset Position (used by Zwoptex)
    Point _offsetPosition;
    Point _unflippedOffsetPositionFromCenter;

    // vertex coords, texture coords and color info
    V3F_C4B_T2F_Quad _quad;

    // opacity and RGB protocol
    bool _opacityModifyRGB;

    // image is flipped
    bool _flipX;                              /// Whether the sprite is flipped horizaontally or not.
    bool _flipY;                              /// Whether the sprite is flipped vertically or not.
};


// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __SPITE_NODE_CCSPRITE_H__
