//
//  GameOverScene.cpp
//  Coco2d_xDemo
//
//  Created by 刘彬彬 on 13-10-11.
//
//

#include "GameOverScene.h"
#include "HelloWorldScene.h"
using namespace cocos2d;


//GameOverScene.cpp里有两个对象，一个场景(scene)和一个图层(layer)，场景可以包含多个图层，而这个图层只在屏幕正中间放了一个文字标签(label)，显示3秒种后返回到HelloWorldScene中
bool GameOverScene::init()
{
    if (CCScene::init())
    {
        this->_layer = GameOverLayer::create();
        this->_layer->retain();
        this->addChild(_layer);
        
        return true;
    }
    else
    {
        return false;
    }
}

GameOverScene::~GameOverScene()
{
    if (_layer) {
        _layer->release();
        _layer = NULL;
    }
}

bool GameOverLayer::init()
{
    if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->_label = CCLabelTTF::create("","Artial", 32);
        _label->retain();
        _label->setColor( ccc3(0, 0, 0) );
        _label->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(_label);
        
        this->runAction( CCSequence::create(CCDelayTime::create(3),CCCallFunc::create(this,callfunc_selector(GameOverLayer::gameOverDone)),NULL));
        
        return true;
    }
    else
    {
        return false;
    }
}

void GameOverLayer::gameOverDone()
{
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

GameOverLayer::~GameOverLayer()
{
    if (_label) {
        _label->release();
        _label = NULL;
    }
}