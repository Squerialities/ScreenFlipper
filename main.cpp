#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static bool s_isFlipped     = false;
static bool s_wasUpsideDown = false;

static void applyFlip(PlayLayer* pl, bool flipped) {
    if (!pl) return;

    auto* scene = pl->getParent();
    if (!scene) scene = pl;

    float targetRot = flipped ? 180.f : 0.f;
    double duration = Mod::get()->getSettingValue<double>("flip-duration");

    if (duration <= 0.0) {
        scene->setRotation(targetRot);
    } else {
        auto rotate = CCRotateTo::create((float)duration, targetRot);
        auto eased  = CCEaseBackOut::create(rotate);
        scene->stopAllActions();
        scene->runAction(eased);
    }

    log::info("[ScreenFlipper] Screen is now {}", flipped ? "FLIPPED" : "normal");
}

struct $modify(ScreenFlipperLayer, PlayLayer) {

    void update(float dt) override {
        PlayLayer::update(dt);

        auto* player = m_player1;
        if (!player) return;

        bool nowUpsideDown = player->m_isUpsideDown;
        if (nowUpsideDown != s_wasUpsideDown) {
            s_wasUpsideDown = nowUpsideDown;
            s_isFlipped     = nowUpsideDown;
            applyFlip(this, s_isFlipped);
        }
    }

    void startGame() override {
        s_isFlipped     = false;
        s_wasUpsideDown = false;
        if (auto* scene = getParent()) scene->setRotation(0.f);
        else setRotation(0.f);
        PlayLayer::startGame();
    }

    void resetLevel() override {
        s_isFlipped     = false;
        s_wasUpsideDown = false;
        if (auto* scene = getParent()) scene->setRotation(0.f);
        else setRotation(0.f);
        PlayLayer::resetLevel();
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) override {
        s_isFlipped     = false;
        s_wasUpsideDown = false;
        return PlayLayer::init(level, useReplay, dontCreateObjects);
    }
};
