#pragma once

/// \file logging.h
/// \brief Zeitgestempelte Ausgabe von Log-Nachrichten mit optionaler Modul- und Funktionskennung.

#include <stdarg.h>
#include <stdlib.h>   // abort() fuer LOG_ASSERT

/// \brief Gibt eine Nachricht ohne Formatierung aus.
/// \param message auszugebende Nachricht (nullterminiert).
/// \note Zeitstempel (Nanosekunden seit dem ersten Log) wird automatisch vorangestellt; leere
///       Nachrichten werden nicht ausgegeben. Thread-sicher.
void logging_log_message(const char* message);

/// \brief Gibt eine printf-formatierte Nachricht aus.
/// \param format printf-Formatstring.
/// \param ... Werte zu den Formatplatzhaltern.
/// \note Ausgabe wird auf 1024 Bytes begrenzt; Zeitstempel wird automatisch vorangestellt.
void logging_log_formatted(const char* format, ...);

/// \brief Gibt eine formatierte Nachricht mit Modul- und Funktionskennung aus.
/// \param module_id Kennung des Moduls, z. B. "database".
/// \param function Name der aufrufenden Funktion.
/// \param format printf-Formatstring.
/// \param ... Werte zu den Formatplatzhaltern.
/// \note Ausgabe wird auf 1024 Bytes begrenzt; Zeitstempel wird automatisch vorangestellt.
void logging_log_with_ID(const char* module_id, const char* function, const char* format, ...);

/// \brief Loggt eine formatierte Nachricht mit Modul- und Funktionskennung.
/// \param module_id Kennung des aufrufenden Moduls.
/// \param ... printf-Formatstring und zugehoerige Werte (format, ...).
/// \note Als Funktionsname wird automatisch __FUNCTION__ des Aufrufers mitgeloggt.
/// \note Greift nur, wenn LOGGING_ENABLED == 1 ist; andernfalls loest das Makro zu ((void)0) auf
///       und die Argumente werden nicht ausgewertet.
#if LOGGING_ENABLED == 1
    #define LOG(module_id, ...) logging_log_with_ID(module_id, __FUNCTION__, __VA_ARGS__)
#else
    #define LOG(module_id, ...) ((void)0)
#endif

/// \brief Zusicherung, die unter ALLEN Umstaenden gelten muss — die Wache gegen stille Fehlschlaege
/// an den Stellen, an denen Ressourcen knapp werden koennen (Vertex-Bereich, Block-Tabelle,
/// Material-Plaetze, Kommando-Warteschlange, Szenen-Eintraege).
///
/// Sie ist NICHT `assert()` aus `<assert.h>`: das loest im Abnahmebau (NDEBUG) zu nichts auf, und
/// genau dort laufen die langen Fahrten. Sie ist auch nicht an LOGGING_ENABLED gebunden — die
/// Meldung geht immer raus.
///
/// Verletzt sie sich, wird die Bedingung mit Datei und Zeile GEMELDET und der Lauf ABGEBROCHEN.
/// Der Abbruch ist Absicht: wer eine Ressourcengrenze still ueberschreitet, verliert Objekte aus
/// dem Bild, waehrend die Zaehler sie weiter als gezeichnet fuehren (genau das war der Fall
/// „gezeichnet 7 von 56" ohne Uranus). Ein Abbruch beim ersten Auftreten ist billiger als ein
/// Fehlbild, das niemand meldet.
/// \param module_id Kennung des Moduls (wie bei LOG).
/// \param bedingung Bedingung, die gelten MUSS.
/// \param ... printf-Format und Werte fuer die Erklaerung (PFLICHT) — sie nennt die Groessen,
///            mit denen sich die Grenze pruefen und anheben laesst.
/// \note Die Zusicherung gehoert an die GRENZE, nicht in die Schleife: nur wo eine Grenze
///       ueberschritten werden kann, nicht bei jedem Aufruf.
#define LOG_ASSERT(module_id, bedingung, ...)                                                      \
    do {                                                                                           \
        if (!(bedingung)) {                                                                        \
            logging_log_with_ID(module_id, __FUNCTION__, __FILE__ ":%d: ZUSICHERUNG VERLETZT: %s",  \
                                __LINE__, #bedingung);                                             \
            logging_log_with_ID(module_id, __FUNCTION__, __VA_ARGS__);                             \
            abort();                                                                               \
        }                                                                                          \
    } while (0)
