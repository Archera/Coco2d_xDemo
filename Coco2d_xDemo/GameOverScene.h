//
//  GameOverScene.h
//  Coco2d_xDemo
//
//  Created by 刘彬彬 on 13-10-11.
//
//

#ifndef __Coco2d_xDemo__GameOverScene__
#define __Coco2d_xDemo__GameOverScene__

#include "cocos2d.h"

class GameOverLayer : public cocos2d::CCLayerColor
{
public:
    GameOverLayer():_label(NULL) {};
    virtual ~GameOverLayer();
    bool init();
    CREATE_FUNC(GameOverLayer);
    void gameOverDone();
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
};

class GameOverScene : public cocos2d::CCScene
{
public:
    GameOverScene():_layer(NULL) {};
    ~GameOverScene();
    bool init();
    CREATE_FUNC(GameOverScene);
    CC_SYNTHESIZE_READONLY(GameOverLayer*, _layer, Layer);
};

#endif /* defined(__Coco2d_xDemo__GameOverScene__) */
