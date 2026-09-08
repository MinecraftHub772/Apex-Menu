#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

// Global Toggles
bool g_noclip = false;
bool g_speedhack = false;
float g_speedMultiplier = 2.0f;

bool g_fpsBypass = false;
float g_targetFPS = 120.0f;

bool g_instantComplete = false;
bool g_autoSpammer = false;
bool g_jumpAlways = false;
bool g_hideUI = false;

// Popup Menu
class ApexMenuPopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("Apex Menu");

        auto contentLayout = ColumnLayout::create()
            ->setGap(8.0f)
            ->setAxisReverse(true);
        
        m_mainLayer->setLayout(contentLayout);

        auto addToggle = [this](const char* labelText, bool currentVal, SEL_MenuHandler callback) {
            auto toggle = CCMenuItemToggler::createWithStandardSprites(this, callback, 0.5f);
            toggle->toggle(currentVal);

            auto label = CCLabelBMFont::create(labelText, "bigFont.fnt");
            label->setScale(0.4f);

            auto rowArray = CCArray::create();
            rowArray->addObject(toggle);
            rowArray->addObject(label);

            auto menuRow = CCMenu::createWithArray(rowArray);
            menuRow->setLayout(RowLayout::create()->setGap(8.0f));
            menuRow->updateLayout();
            m_mainLayer->addChild(menuRow);
        };

        addToggle("Noclip", g_noclip, menu_selector(ApexMenuPopup::onNoclip));
        addToggle("Speedhack (2x)", g_speedhack, menu_selector(ApexMenuPopup::onSpeedhack));
        addToggle("FPS Bypass (120)", g_fpsBypass, menu_selector(ApexMenuPopup::onFPSBypass));
        addToggle("Instant Complete", g_instantComplete, menu_selector(ApexMenuPopup::onInstantComplete));
        addToggle("Auto Spammer", g_autoSpammer, menu_selector(ApexMenuPopup::onAutoSpammer));
        addToggle("Always Jump", g_jumpAlways, menu_selector(ApexMenuPopup::onJumpAlways));
        addToggle("Hide UI", g_hideUI, menu_selector(ApexMenuPopup::onHideUI));

        m_mainLayer->updateLayout();
        return true;
    }

    void onNoclip(CCObject*) { g_noclip = !g_noclip; }
    void onSpeedhack(CCObject*) { g_speedhack = !g_speedhack; }
    void onInstantComplete(CCObject*) { g_instantComplete = !g_instantComplete; }
    void onAutoSpammer(CCObject*) { g_autoSpammer = !g_autoSpammer; }
    void onJumpAlways(CCObject*) { g_jumpAlways = !g_jumpAlways; }
    void onHideUI(CCObject*) { g_hideUI = !g_hideUI; }

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
        auto ret = new ApexMenuPopup();
        if (ret && ret->initAnchored(260.0f, 200.0f)) {
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
        if (g_noclip) return;
        PlayerObject::destroyPlayer(p0);
    }

    void update(float dt) override {
        if (g_speedhack) dt *= g_speedMultiplier;
        if (g_jumpAlways) this->pushButton(PlayerButton::Jump);
        PlayerObject::update(dt);
    }
};

// PlayLayer Hooks
class $modify(MyPlayLayer, PlayLayer) {
    void update(float dt) override {
        PlayLayer::update(dt);

        if (g_instantComplete && !this->m_hasCompletedLevel) {
            this->playEndAnimationToPos({0, 0}, false);
        }

        if (g_autoSpammer && this->m_player1) {
            this->m_player1->pushButton(PlayerButton::Jump);
            this->m_player1->releaseButton(PlayerButton::Jump);
        }

        if (g_hideUI && this->m_uiLayer) {
            this->m_uiLayer->setVisible(false);
        }
    }
};

// Menu Hook
class $modify(MyMenuLayer, MenuLayer) {
    bool init() override {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        if (!menu) menu = this->getChildByID("main-menu");

        auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
        btnSprite->setScale(0.7f);

        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MyMenuLayer::onOpenApexMenu)
        );

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenApexMenu(CCObject*) {
        ApexMenuPopup::create()->show();
    }
};
