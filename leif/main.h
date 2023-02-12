/**
 * @brief Declares all main application methods.
 *
 * This file contains all methods and things used in the applications main
 * module.
 *
 * @author Dariusz Scharsig
 *
 * @date 17.09.2022
 */
#ifndef MAIN_H
#define MAIN_H

#include <QTranslator>

int main(int argc, char *argv[]);

void setApplicationInfo();
void loadTranslations(QTranslator *translator);
void cleanup();

#ifdef _WIN32
void setStyleSheet();
#endif

#endif // MAIN_H
