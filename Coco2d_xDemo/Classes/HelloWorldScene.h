#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayerColor
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    //出现敌人
    void addTarget();
    
    void spriteMoveFinished(CCNode* sender);
    
    void gameLogic(cocos2d::CCTime dt);

    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    void update(float dt);
    
    HelloWorld();//构造函数的声明
    ~HelloWorld();//析构函数
    
    
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
protected:
    cocos2d::CCArray *_targets;//缓存现有的怪物。
    cocos2d::CCArray *_projectiles;//缓存现有的子弹。
     int _projectilesDestroyed;//用来计算英雄杀掉了多少个怪物
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _countLbl, Label);
};

#endif // __HELLOWORLD_SCENE_H__
