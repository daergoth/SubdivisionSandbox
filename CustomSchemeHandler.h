#ifndef CUSTOMSCHEMEHANDLER_H
#define CUSTOMSCHEMEHANDLER_H

#include <string>
#include <iostream>
#include <memory>

#include <QFile>
#include <QTextStream>
#include <nlohmann/json.hpp>

#include "CustomScheme.h"

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

    CustomScheme openCustomScheme(QString filename);
    CustomScheme parseCustomScheme(std::string scheme_json_string);

    void saveCustomScheme(QString filename, std::shared_ptr<CustomScheme> custom_scheme);

    std::shared_ptr<CustomScheme> getCurrentCustomScheme();
    void setCurrentCustomScheme(CustomScheme custom_scheme);

    void debug();

private:
    CustomSchemeHandler() {}

    std::shared_ptr<CustomScheme> current_scheme;
};

#endif // CUSTOMSCHEMEHANDLER_H
