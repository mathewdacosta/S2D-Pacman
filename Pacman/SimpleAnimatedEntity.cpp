#include "SimpleAnimatedEntity.h"


SimpleAnimatedEntity::SimpleAnimatedEntity(Rect* sourceRect, Rect* destRect, int animFrameCount, int animFrameWidth, int animFrameDuration, int animCurrentTime, int animCurrentFrame)
{
    _texture = nullptr;
    _sourceRect = sourceRect;
    _destRect = destRect;
    _animFrameCount = animFrameCount;
    _animFrameWidth = animFrameWidth;
    _animFrameDuration = animFrameDuration;
    _animCurrentTime = animCurrentTime;
    _animCurrentFrame = animCurrentFrame;
}

SimpleAnimatedEntity::~SimpleAnimatedEntity()
{
}

void SimpleAnimatedEntity::Update(int elapsedTime)
{
    // Skip animation when duration <= 0; allows non-animating sprites e.g. walls
    if (_animFrameDuration <= 0)
        return;
    
    // Increment munchie animation frame
    _animCurrentTime += elapsedTime;
    if (_animCurrentTime > _animFrameDuration)
    {
        _animCurrentTime = 0;
        _animCurrentFrame = (_animCurrentFrame + 1) % _animFrameCount;
        _sourceRect->X = _sourceRect->Width * _animCurrentFrame;
    }
}

void SimpleAnimatedEntity::SetTexture(Texture2D* texture)
{
    _texture = texture;
}

Rect* SimpleAnimatedEntity::GetBoundingBox()
{
    return _destRect;
}

void SimpleAnimatedEntity::SetScreenPosition(float x, float y)
{
    _destRect->X = x;
    _destRect->Y = y;
}

void SimpleAnimatedEntity::Draw()
{
    SpriteBatch::Draw(_texture, _destRect, _sourceRect);
}
