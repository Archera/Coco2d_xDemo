#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    _projectilesDestroyed = 0;
    //添加背景音乐
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
    
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("刘彬彬的游戏", "Thonburi", 34);
    pLabel->setColor(ccc3(0, 0, 0));
    
    // ask director the window size 得到屏幕大小
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen label在屏幕中的偏移量
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer 添加label
    this->addChild(pLabel, 1);
    
    //添加一个label用来记录总共消灭了多少怪物
    this->_countLbl = CCLabelTTF::create("start","Artial", 32);
    //_countLbl->retain();
    _countLbl->setColor( ccc3(0, 0, 0) );
    _countLbl->setPosition(ccp(30, size.height - 20));
    this->addChild(_countLbl);

    // add "HelloWorld" splash screen" 初始化精灵
    CCSprite* pSprite = CCSprite::create("gandalf.png",CCRectMake(0, 0, 70, 70));

    // position the sprite on the center of the screen 精灵在屏幕中的偏移量
    pSprite->setPosition( ccp(pSprite->getContentSize().width/2, size.height/2) );

    // add the sprite as a child to this layer 添加精灵
    this->addChild(pSprite, 0);
    
    //没1s调用创造敌对精灵的方法
    this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );
    
    //开启屏幕多点触控
    this->setTouchEnabled(true);
    //开启屏幕单点触控 一定要实现ccTouchBegan方法
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);

    
    //初始化缓存现有的怪物和子弹的数组
    _targets = new CCArray;
    _projectiles = new CCArray;
    //现在可以修改addTarget()，把新目标添加到目标数组中，并设置其tag为1。
    //修改ccTouchesEnded()，把新子弹加入到子弹数组中，并设置其tag为2。
    
    //好了，最后一件事，我们要把update()加入到schedule里让它每帧都能被调用
    this->schedule( schedule_selector(HelloWorld::update) );
    return true;
}

void HelloWorld::gameLogic(cocos2d::CCTime dt)
{
    
     this->addTarget();
    
}

//目标精灵
void HelloWorld::addTarget()
{
    CCSprite *target = CCSprite::create("lurtz.png",CCRectMake(0, 0, 70, 70));
    
    target->setTag(1);
    _targets->addObject(target);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    int minY = target->getContentSize().height/2;
    
    int maxY = size.height - minY;
    
    int rangeY = maxY - minY;
    
    int actualY = ( rand() % rangeY ) + minY;
    
    target->setPosition( ccp(size.width + (target->getContentSize().width/2),actualY) );
    this->addChild(target);
    
    //确定精灵的速度
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    
    int rangeDuration = maxDuration - minDuration;
    int actualDuration = ( rand() % rangeDuration ) + minDuration;
    
    //创建精灵的动作
    CCFiniteTimeAction *actionMove = CCMoveTo::create(actualDuration,ccp(0 - target->getContentSize().width/2, actualY));
//    CCFiniteTimeAction *actionMoveDone = CCCallFuncN::initWithTarget( this->callfuncN_selector(HelloWorld::spriteMoveFinished));
    
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this,callfuncN_selector(HelloWorld::spriteMoveFinished));
    
    target->runAction( CCSequence::create(actionMove,actionMoveDone, NULL) );
}

//精灵结束移动
void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    //之后，按下面修改spriteMoveFinished()。这里根据标记的不同，在对应的数组中移除精灵
      CCSprite *sprite = (CCSprite *)sender;
      this->removeChild(sprite, true);
    
    if (sprite->getTag()==1) {
        _targets->removeObject(sprite);
        GameOverScene *gameOverScene = GameOverScene::create();
        gameOverScene->getLayer()->getLabel()->setString("You Lose!");
        CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }
    else if (sprite->getTag()==2)
    {
        _projectiles->removeObject(sprite);
    }
}

//发射子弹
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //选择其中的一个触动工作
    CCTouch *touch = (CCTouch*)(touches->anyObject());
    
    CCPoint location = touch->getLocationInView();
    
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    //设置初始位置的射弹，及目标精灵(gandalf.png)的位置
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //projectile弹
    CCSprite *projectile = CCSprite::create("JStar.png",CCRectMake(0, 0, 23, 23));
    
    projectile->setTag(2);
    _projectiles->addObject(projectile);
    
    projectile->setPosition( ccp(20, winSize.height/2) );
    
    //确定抵消对投射的位置
    int offX = location.x - projectile->getPosition().x;
    int offY = location.y - projectile->getPosition().y;
    
    //如果我们向后发射子弹，则直接返回及不能作出向后发射子弹的操作
    if (offX <= 0) return;
    
    this->addChild(projectile);
    
    //确定我们想要射击,弹丸
    int realX = winSize.width + (projectile->getContentSize().width/2);
    float ratio = (float)offY / (float)offX;
    int realY = (realX * ratio) + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);
    
    //确定多远的长度我们射击
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
    float velocity = 480/1;
    float realMoveDuration = length/velocity;
    
    //移动弹到实际端点
    projectile->runAction( CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished)), NULL) );
    
    //添加子弹发射的声音
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

//检测每帧的碰撞，并从游戏中删除碰撞中的子弹和怪物。
void HelloWorld::update(float dt)
{
    CCArray *projectilesToDelete = new CCArray;
    CCArray* targetsToDelete =new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;
    
    CCARRAY_FOREACH(_projectiles, it)
    {
        CCSprite *projectile = dynamic_cast<CCSprite*>(it);
        CCRect projectileRect = CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width/2),projectile->getPosition().y - (projectile->getContentSize().height/2),projectile->getContentSize().width,projectile->getContentSize().height);
        
        CCARRAY_FOREACH(_targets, jt)
        {
            CCSprite *target = dynamic_cast<CCSprite*>(jt);
            CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),target->getPosition().y - (target->getContentSize().height/2),target->getContentSize().width,target->getContentSize().height);
            if (projectileRect.intersectsRect(targetRect))
            {
                targetsToDelete->addObject(target);
                projectilesToDelete->addObject(projectile);
            }
        }
    }
    
    //在下面方法增加计数并检查获胜条件，获胜了就显示"You Win!"界面
    CCARRAY_FOREACH(targetsToDelete, jt)
    {
        CCSprite *target = dynamic_cast<CCSprite*>(jt);
        _targets->removeObject(target);
        this->removeChild(target, true);
        
        _projectilesDestroyed++;

        CCString* ns=CCString::createWithFormat("%d",_projectilesDestroyed);
        const char *string=std::string(ns->m_sString).c_str();
        this->_countLbl->setString(string);
        if (_projectilesDestroyed > 30)
        {
            GameOverScene *gameOverScene = GameOverScene::create();
            gameOverScene->getLayer()->getLabel()->setString("You Win!");
             CCDirector::sharedDirector()->replaceScene(gameOverScene);
        }
    }
    
    CCARRAY_FOREACH(projectilesToDelete, it)
    {
        CCSprite* projectile = dynamic_cast<CCSprite*>(it);
        _projectiles->removeObject(projectile);
        this->removeChild(projectile, true);
    }
    projectilesToDelete->release();
    targetsToDelete->release();
}

HelloWorld::~HelloWorld()
{
    if (_targets) {
        _targets->release();
        _targets = NULL;
    }
    if (_projectiles) {
        _projectiles->release();
        _projectiles = NULL;
    }
}

HelloWorld::HelloWorld():_targets(NULL),_projectiles(NULL)
{
    
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
