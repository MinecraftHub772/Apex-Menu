#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/CCTextInputNode.hpp>

using namespace geode::prelude;

// Global Toggles
static bool g_noclip = false;
static bool g_speedhack = false;
static float g_speedMultiplier = 2.0f;

static bool g_fpsBypass = false;
static float g_targetFPS = 120.0f;

static bool g_instantComplete = false;
static bool g_autoSpammer = false;
static bool g_jumpAlways = false;
static bool g_hideUI = false;

// Popup Menu
class ApexMenuPopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("Apex Menu");

        auto contentLayout = ColumnLayout::create()
            ->setGap(8.0f)
            ->setAxisReverse(true);
        
        m_mainLayer->setLayout(contentLayout);

        auto addToggle = [this](const char* labelText, bool currentVal, std::function<void()> callback) {
            auto toggleSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            auto toggleSpriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
            
            auto toggle = CCMenuItemToggler::create(
                toggleSprite,
                toggleSpriteOn,
                this,
                nullptr
            );
            toggle->setScale(0.8f);
            toggle->setToggled(currentVal);

            auto label = CCLabelBMFont::create(labelText, "bigFont.fnt");
            label->setScale(0.4f);
            label->setAnchorPoint({0.0f, 0.5f});

            auto rowContainer = CCLayer::create();
            rowContainer->setContentSize({240.0f, 30.0f});
            
            toggle->setPosition({10.0f, 15.0f});
            label->setPosition({30.0f, 15.0f});
            
            rowContainer->addChild(toggle);
            rowContainer->addChild(label);
            
            m_mainLayer->addChild(rowContainer);
        };

        addToggle("Noclip", g_noclip, [this]() { g_noclip = !g_noclip; });
        addToggle("Speedhack (2x)", g_speedhack, [this]() { g_speedhack = !g_speedhack; });
        addToggle("FPS Bypass (120)", g_fpsBypass, [this]() { 
            g_fpsBypass = !g_fpsBypass;
            if (g_fpsBypass) {
                CCDirector::sharedDirector()->setAnimationInterval(1.0f / g_targetFPS);
            } else {
                CCDirector::sharedDirector()->setAnimationInterval(1.0f / 60.0f);
            }
        });
        addToggle("Instant Complete", g_instantComplete, [this]() { g_instantComplete = !g_instantComplete; });
        addToggle("Auto Spammer", g_autoSpammer, [this]() { g_autoSpammer = !g_autoSpammer; });
        addToggle("Always Jump", g_jumpAlways, [this]() { g_jumpAlways = !g_jumpAlways; });
        addToggle("Hide UI", g_hideUI, [this]() { g_hideUI = !g_hideUI; });

        m_mainLayer->updateLayout();
        return true;
    }

public:
    static ApexMenuPopup* create() {
        auto ret = new (std::nothrow) ApexMenuPopup();
        if (ret && ret->initAnchored(260.0f, 240.0f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// Player Hooks
class $modify(MyPlayerObject, PlayerObject) {
    void destroyPlayer(bool p0) override {
        if (g_noclip) {
            return;
        }
        PlayerObject::destroyPlayer(p0);
    }

    void update(float dt) override {
        float actualDt = dt;
        if (g_speedhack) {
            actualDt *= g_speedMultiplier;
        }
        if (g_jumpAlways && !this->m_isAccelerating) {
            this->pushButton(PlayerButton::Jump);
        }
        PlayerObject::update(actualDt);
    }
};

// PlayLayer Hooks
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        PlayLayer::update(dt);

        if (g_instantComplete && !this->m_hasCompletedLevel && this->m_player1) {
            this->playEndAnimationToPos({0, 0}, false);
        }

        if (g_autoSpammer && this->m_player1) {
            this->m_player1->pushButton(PlayerButton::Jump);
            this->m_player1->releaseButton(PlayerButton::Jump);
        }

        if (g_hideUI && this->m_uiLayer) {
            this->m_uiLayer->setVisible(false);
        } else if (!g_hideUI && this->m_uiLayer) {
            this->m_uiLayer->setVisible(true);
        }
    }
};

// Menu Hook
class $modify(MyMenuLayer, MenuLayer) {
    bool init() override {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");
        if (!menu) {
            menu = this->getChildByID("main-menu");
        }

        if (!menu) {
            return true;
        }

        auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
        if (!btnSprite) {
            btnSprite = CCSprite::create();
        }
        btnSprite->setScale(0.7f);

        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MyMenuLayer::onOpenApexMenu)
        );
        
        btn->setID("apex-menu-btn");

        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

    void onOpenApexMenu(CCObject*) {
        ApexMenuPopup::create()->show();
    }
};
