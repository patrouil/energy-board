//
// Created by Patrick Rouillon on 06/09/2026.
//
#ifndef ENERGY_BOARD_PAGEROUTER_H
#define ENERGY_BOARD_PAGEROUTER_H

#include "Display.h"
#include "Screen.h"


/**
 * @enum ScreenId
 * @brief Identifiants des écrans disponibles.
 */
enum class ScreenId : uint8_t
{
    WELCOME_PAGE = 0,  ///< Page d'accueil.
    BOOT_PAGE = 1,     ///< Page "À propos".
    SCREEN_COUNT        ///< Nombre total d'écrans.
};

/**
 * @class PageRouter
 * @brief Gestionnaire de routage des écrans.
 * Crée dynamiquement les instances des écrans à la première demande.
 */
class PageRouter
{
public:
    explicit PageRouter(Display& display);
    ~PageRouter();

    /**
     * @brief Récupère ou crée une instance d'écran.
     * @param id Identifiant de l'écran.
     * @return Screen* Pointeur vers l'écran.
     */
    Screen* get_screen(ScreenId id);

    /**
     * @brief Affiche un écran.
     * @param id Identifiant de l'écran à afficher.
     */
    void display_screen(ScreenId id);

private:
    Display& display;
    Screen* screens[static_cast<uint8_t>(ScreenId::SCREEN_COUNT)] = {nullptr};
};

#endif