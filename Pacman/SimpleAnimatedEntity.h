#pragma once

#include "Entity.h"
#include "S2D/S2D.h"
using namespace S2D;

/// <summary> An entity that cycles through a single set of animation frames. </summary>
class SimpleAnimatedEntity : public Entity
{
private:
    int _animFrameCount;            // number of frames in animation
    int _animFrameWidth;            // width of animation frame
    int _animFrameDuration;         // interval between animation frames
    int _animCurrentTime;           // current time since last update
    int _animCurrentFrame;          // current frame in animation
    
    Texture2D* _texture;

protected:
    SimpleAnimatedEntity(Rect* sourceRect, Rect* destRect, int animFrameCount, int animFrameWidth, int animFrameDuration, int animCurrentTime, int animCurrentFrame);
    ~SimpleAnimatedEntity();
    
    Rect* _sourceRect;              
    Rect* _destRect;

public:
    void Update(int elapsedTime);

    void SetTexture(Texture2D* texture);

    Rect* GetBoundingBox();
    void SetScreenPosition(float x, float y);
    
    void Draw() override;
};
