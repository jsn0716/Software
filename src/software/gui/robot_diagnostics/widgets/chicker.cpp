#include "software/gui/robot_diagnostics/widgets/chicker.h"

void setupChicker(
    Ui::AutoGeneratedMainWidget *widget,
    std::function<void(double, ChargeMode, ChickMode)> chicker_state_changed_callback)
{
    std::shared_ptr<double> chicker_power = std::make_shared<double>(0.0);
    std::shared_ptr<ChargeMode> charge_mode =
        std::make_shared<ChargeMode>(ChargeMode::DISCHARGE);
    std::shared_ptr<ChickMode> chick_mode = std::make_shared<ChickMode>(ChickMode::NONE);

    auto chicker_power_changed_callback = [chicker_power, charge_mode, chick_mode,
                                           chicker_state_changed_callback](double value) {
        *chicker_power = value;
        chicker_state_changed_callback(*chicker_power, *charge_mode, *chick_mode);
    };

    auto charge_mode_changed_callback =
        [chicker_power, charge_mode, chick_mode,
         chicker_state_changed_callback](ChargeMode mode) {
            *charge_mode = mode;
            if (*charge_mode == ChargeMode::DISCHARGE)
            {
                *chick_mode = ChickMode::NONE;
            }
            chicker_state_changed_callback(*chicker_power, *charge_mode, *chick_mode);
        };

    auto chick_mode_changed_callback = [chicker_power, charge_mode, chick_mode,
                                        chicker_state_changed_callback](ChickMode mode) {
        *chick_mode = mode;
        chicker_state_changed_callback(*chicker_power, *charge_mode, *chick_mode);
        if (*chick_mode == ChickMode::KICK || *chick_mode == ChickMode::CHIP)
        {
            // Reset ChickMode since kick/chip pressed should only trigger one kick/chip
            *chick_mode = ChickMode::NONE;
        }
    };

    setupSliderLineEdit(widget->lineEdit_chicker_power, widget->slider_chicker_power,
                        chicker_power_changed_callback, 0.0, 10.0, 10.0);

    setupChickerChargeRadioButtons(widget, charge_mode_changed_callback);
    setupChickerAutochickRadioButtons(widget, chick_mode_changed_callback);
    setupChickerPushButtons(widget, chick_mode_changed_callback);
}

void setupChickerChargeRadioButtons(
    Ui::AutoGeneratedMainWidget *widget,
    std::function<void(ChargeMode)> charge_mode_changed_callback)
{
    widget->radioButton_discharge->setChecked(true);

    auto on_charge_mode_button_changed =
        [widget, charge_mode_changed_callback](const bool toggle) {
            if (widget->radioButton_discharge->isChecked())
            {
                widget->pushButton_chicker_kick->setEnabled(false);
                widget->pushButton_chicker_chip->setEnabled(false);
                widget->radioButton_autokick->setEnabled(false);
                widget->radioButton_autochip->setEnabled(false);
                widget->radioButton_autochick_none->setChecked(true);
                charge_mode_changed_callback(ChargeMode::DISCHARGE);
            }
            else if (widget->radioButton_float->isChecked())
            {
                widget->radioButton_autokick->setEnabled(true);
                widget->radioButton_autochip->setEnabled(true);
                widget->pushButton_chicker_kick->setEnabled(true);
                widget->pushButton_chicker_chip->setEnabled(true);
                widget->radioButton_autochick_none->setChecked(true);
                charge_mode_changed_callback(ChargeMode::FLOAT);
            }
            else if (widget->radioButton_charge->isChecked())
            {
                widget->radioButton_autokick->setEnabled(true);
                widget->radioButton_autochip->setEnabled(true);
                widget->pushButton_chicker_kick->setEnabled(true);
                widget->pushButton_chicker_chip->setEnabled(true);
                widget->radioButton_autochick_none->setChecked(true);
                charge_mode_changed_callback(ChargeMode::CHARGE);
            }
        };

    QWidget::connect(widget->radioButton_discharge, &QRadioButton::toggled,
                     on_charge_mode_button_changed);
    QWidget::connect(widget->radioButton_float, &QRadioButton::toggled,
                     on_charge_mode_button_changed);
    QWidget::connect(widget->radioButton_charge, &QRadioButton::toggled,
                     on_charge_mode_button_changed);
}

void setupChickerAutochickRadioButtons(
    Ui::AutoGeneratedMainWidget *widget,
    std::function<void(ChickMode)> chick_mode_changed_callback)
{
    // Initial state is chicker discharged, so autochick radio buttons are disabled
    widget->radioButton_autochick_none->setChecked(true);
    widget->radioButton_autokick->setEnabled(false);
    widget->radioButton_autochip->setEnabled(false);

    auto on_autochick_mode_button_changed =
        [widget, chick_mode_changed_callback](const bool toggle) {
            if (widget->radioButton_autokick->isChecked())
            {
                widget->pushButton_chicker_kick->setEnabled(false);
                widget->pushButton_chicker_chip->setEnabled(false);
                chick_mode_changed_callback(ChickMode::AUTOKICK);
            }
            else if (widget->radioButton_autochip->isChecked())
            {
                widget->pushButton_chicker_kick->setEnabled(false);
                widget->pushButton_chicker_chip->setEnabled(false);
                chick_mode_changed_callback(ChickMode::AUTOCHIP);
            }
            else if (widget->radioButton_autochick_none->isChecked())
            {
                widget->pushButton_chicker_kick->setEnabled(true);
                widget->pushButton_chicker_chip->setEnabled(true);
                chick_mode_changed_callback(ChickMode::NONE);
            }
        };

    QWidget::connect(widget->radioButton_autokick, &QRadioButton::toggled,
                     on_autochick_mode_button_changed);
    QWidget::connect(widget->radioButton_autochip, &QRadioButton::toggled,
                     on_autochick_mode_button_changed);
    QWidget::connect(widget->radioButton_autochick_none, &QRadioButton::toggled,
                     on_autochick_mode_button_changed);
}

void setupChickerPushButtons(Ui::AutoGeneratedMainWidget *widget,
                             std::function<void(ChickMode)> chick_mode_changed_callback)
{
    // Initial state is chicker discharged, so chick push buttons are disabled
    widget->pushButton_chicker_kick->setEnabled(false);
    widget->pushButton_chicker_chip->setEnabled(false);

    auto kick_pushed = [widget, chick_mode_changed_callback]() {
        widget->radioButton_autochick_none->setChecked(true);
        chick_mode_changed_callback(ChickMode::KICK);
    };

    auto chip_pushed = [widget, chick_mode_changed_callback]() {
        widget->radioButton_autochick_none->setChecked(true);
        chick_mode_changed_callback(ChickMode::CHIP);
    };

    QWidget::connect(widget->pushButton_chicker_kick, &QPushButton::clicked, kick_pushed);

    QWidget::connect(widget->pushButton_chicker_chip, &QPushButton::clicked, chip_pushed);
}

void resetAutochickRadioButtons(Ui::AutoGeneratedMainWidget *widget) {}
