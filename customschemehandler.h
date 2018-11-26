#ifndef CUSTOMSCHEMEHANDLER_H
#define CUSTOMSCHEMEHANDLER_H

#include <string>
#include <iostream>

#include <QFile>
#include <nlohmann/json.hpp>

#include "customscheme.h"

using json = nlohmann::json;

class CustomSchemeHandler
{
public:
    static CustomSchemeHandler& getInstance() {
        static CustomSchemeHandler instance;
        return instance;
    }

    CustomSchemeHandler(CustomSchemeHandler const&) = delete;
    void operator=(CustomSchemeHandler const&) = delete;

    CustomScheme readCustomScheme(std::string filename);
    CustomScheme parseCustomScheme(std::string scheme_json);

    void saveCustomScheme(std::string filename, CustomScheme customScheme);

    void debug();

private:
    CustomSchemeHandler() {}
};

#endif // CUSTOMSCHEMEHANDLER_H
