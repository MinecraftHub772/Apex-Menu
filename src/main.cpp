#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>

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
        
        if (contentLayout) {
            m_mainLayer->setLayout(contentLayout);
        }

        // Noclip Toggle
        auto noclipToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onNoclip),
            0.8f
        );
        if (noclipToggle) {
            noclipToggle->toggle(g_noclip);
            m_mainLayer->addChild(noclipToggle);
        }

        auto noclipLabel = CCLabelBMFont::create("Noclip", "bigFont.fnt");
        if (noclipLabel) {
            noclipLabel->setScale(0.4f);
            m_mainLayer->addChild(noclipLabel);
        }

        // Speedhack Toggle
        auto speedToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onSpeedhack),
            0.8f
        );
        if (speedToggle) {
            speedToggle->toggle(g_speedhack);
            m_mainLayer->addChild(speedToggle);
        }

        auto speedLabel = CCLabelBMFont::create("Speedhack (2x)", "bigFont.fnt");
        if (speedLabel) {
            speedLabel->setScale(0.4f);
            m_mainLayer->addChild(speedLabel);
        }

        // FPS Bypass Toggle
        auto fpsToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onFPSBypass),
            0.8f
        );
        if (fpsToggle) {
            fpsToggle->toggle(g_fpsBypass);
            m_mainLayer->addChild(fpsToggle);
        }

        auto fpsLabel = CCLabelBMFont::create("FPS Bypass (120)", "bigFont.fnt");
        if (fpsLabel) {
            fpsLabel->setScale(0.4f);
            m_mainLayer->addChild(fpsLabel);
        }

        // Instant Complete Toggle
        auto completeToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onInstantComplete),
            0.8f
        );
        if (completeToggle) {
            completeToggle->toggle(g_instantComplete);
            m_mainLayer->addChild(completeToggle);
        }

        auto completeLabel = CCLabelBMFont::create("Instant Complete", "bigFont.fnt");
        if (completeLabel) {
            completeLabel->setScale(0.4f);
            m_mainLayer->addChild(completeLabel);
        }

        // Auto Spammer Toggle
        auto spamToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onAutoSpammer),
            0.8f
        );
        if (spamToggle) {
            spamToggle->toggle(g_autoSpammer);
            m_mainLayer->addChild(spamToggle);
        }

        auto spamLabel = CCLabelBMFont::create("Auto Spammer", "bigFont.fnt");
        if (spamLabel) {
            spamLabel->setScale(0.4f);
            m_mainLayer->addChild(spamLabel);
        }

        // Jump Always Toggle
        auto jumpToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onJumpAlways),
            0.8f
        );
        if (jumpToggle) {
            jumpToggle->toggle(g_jumpAlways);
            m_mainLayer->addChild(jumpToggle);
        }

        auto jumpLabel = CCLabelBMFont::create("Always Jump", "bigFont.fnt");
        if (jumpLabel) {
            jumpLabel->setScale(0.4f);
            m_mainLayer->addChild(jumpLabel);
        }

        // Hide UI Toggle
        auto hideToggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(ApexMenuPopup::onHideUI),
            0.8f
        );
        if (hideToggle) {
            hideToggle->toggle(g_hideUI);
            m_mainLayer->addChild(hideToggle);
        }

        auto hideLabel = CCLabelBMFont::create("Hide UI", "bigFont.fnt");
        if (hideLabel) {
            hideLabel->setScale(0.4f);
            m_mainLayer->addChild(hideLabel);
        }

        if (m_mainLayer) {
            m_mainLayer->updateLayout();
        }

        return true;
    }

    void onNoclip(CCObject*) {
        g_noclip = !g_noclip;
    }

    void onSpeedhack(CCObject*) {
        g_speedhack = !g_speedhack;
    }

    void onInstantComplete(CCObject*) {
        g_instantComplete = !g_instantComplete;
    }

    void onAutoSpammer(CCObject*) {
        g_autoSpammer = !g_autoSpammer;
    }

    void onJumpAlways(CCObject*) {
        g_jumpAlways = !g_jumpAlways;
    }

    void onHideUI(CCObject*) {
        g_hideUI = !g_hideUI;
    }

    void onFPSBypass(CCObject*) {
        g_fpsBypass = !g_fpsBypass;
        if (g_fpsBypass) {
            CCDirector::sharedDirector()->setAnimationInterval(1.0f / g_targetFPS);
        } else {
            CCDirector::sharedDirector()->setAnimationInterval(1.0f / 60.0f);
        }
    }

public:
    static ApexMenuPopup* create() {
        auto ret = new (std::nothrow) ApexMenuPopup();
        if (ret && ret->initAnchored(280.0f, 300.0f, "GJ_square01.png")) {
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
        
        PlayerObject::update(actualDt);
        
        if (g_jumpAlways) {
            this->pushButton(PlayerButton::Jump);
        }
    }
};

// PlayLayer Hooks
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        PlayLayer::update(dt);

        if (g_instantComplete && this->m_player1 && !this->m_hasCompletedLevel) {
            this->playEndAnimationToPos({0, 0}, false);
        }

        if (g_autoSpammer && this->m_player1) {
            this->m_player1->pushButton(PlayerButton::Jump);
            this->m_player1->releaseButton(PlayerButton::Jump);
        }

        if (this->m_uiLayer) {
            if (g_hideUI) {
                this->m_uiLayer->setVisible(false);
            } else {
                this->m_uiLayer->setVisible(true);
            }
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
            if (!btnSprite) {
                return true;
            }
        }

        btnSprite->setScale(0.7f);

        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MyMenuLayer::onOpenApexMenu)
        );

        if (btn) {
            btn->setID("apex-menu-btn");
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenApexMenu(CCObject*) {
        auto popup = ApexMenuPopup::create();
        if (popup) {
            popup->show();
        }
    }
};
