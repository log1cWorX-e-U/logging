#pragma once

/// \file logging.h
/// \brief Zeitgestempelte Ausgabe von Log-Nachrichten mit optionaler Modul- und Funktionskennung.

#include <stdarg.h>

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
