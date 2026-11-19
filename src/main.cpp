#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(TimedPercentPlayLayer, PlayLayer) {
    struct Fields {
        float m_elapsedTime = 0.0f;
        bool m_reached = false;
    };

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_elapsedTime = 0.0f;
        m_fields->m_reached = false;
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        if (m_fields->m_reached) {
            if (m_percentageLabel) {
                m_percentageLabel->setString("100%");
            }
            if (m_progressFill && m_progressBar) {
                m_progressFill->setTextureRect({
                    0.0f,
                    0.0f,
                    m_progressBar->getTextureRect().size.width,
                    m_progressFill->getTextureRect().size.height
                });
            }
        }
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            return;
        }

        if (m_isDead || m_hasCompletedLevel || m_fields->m_reached) {
            return;
        }

        float targetSeconds = static_cast<float>(
            Mod::get()->getSettingValue<double>("time-seconds")
        );

        m_fields->m_elapsedTime += dt;

        if (m_fields->m_elapsedTime >= targetSeconds) {
            m_fields->m_reached = true;
            this->updateProgressbar();

            if (Mod::get()->getSettingValue<bool>("show-notification")) {
                Notification::create("100% Reached!", NotificationIcon::Success)->show();
            }
        }
    }
};
