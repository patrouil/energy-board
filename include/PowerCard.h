//
// Created by Vibe Code on 03/10/2026.
// with the assistance of Mistral Vibe Code.
//

#pragma once

#include <lvgl.h>

/*
 * PowerCard : widget composite affichant une mesure de puissance electrique.
 *
 *   - rectangle de 125 px de large et 100 px de haut.
 *   - bord de 2 px, coins arrondis, couleur de bord parametrable.
 *   - couleur de fond parametrable.
 *   - en bas, un texte centre, police de hauteur 12.
 *   - au-dessus, la valeur de la puissance, police de hauteur 20.
 *   - a droite de la valeur, l'unite, police de hauteur 10.
 *
 * La puissance est transmise en watts (int32_t, valeur pouvant etre negative).
 *
 * Regles d'affichage (valeur absolue) :
 *   - < 1000 W          : valeur entiere, unite "Wh".
 *   - < 10000 W         : valeur / 1000, 2 chiffres apres la virgule, unite "kWh".
 *   - >= 10000 W        : valeur / 1000, 1 chiffre apres la virgule, unite "kWh".
 *
 * Ainsi jamais plus de 3 digits ne sont affiches.
 */
class PowerCard
{
public:
    PowerCard() = default;
    ~PowerCard();

    lv_obj_t* create(lv_obj_t* parent);

    void setPower(int32_t watts);
    void setTitle(const char* title);
    void setBorderColor(lv_color_t color);
    void setBackgroundColor(lv_color_t color);

    lv_obj_t* getContainer() const { return container; }

private:

    static const lv_coord_t CARD_WIDTH = 125;
    static const lv_coord_t CARD_HEIGHT = 100;
    static const lv_coord_t CARD_BORDER_WIDTH = 2;
    static const lv_coord_t CARD_RADIUS = 8;
    static const uint8_t VALUE_BUFFER_SIZE = 16;

    lv_obj_t* container = nullptr;
    lv_obj_t* valueLabel = nullptr;
    lv_obj_t* unitLabel = nullptr;
    lv_obj_t* titleLabel = nullptr;
    char valueBuffer[VALUE_BUFFER_SIZE] = {0};
    char titleBuffer[VALUE_BUFFER_SIZE] = {0};
    int32_t watts = 0;

    void updateValue();
};
