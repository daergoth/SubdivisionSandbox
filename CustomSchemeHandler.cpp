#include "CustomSchemeHandler.h"

CustomScheme CustomSchemeHandler::openCustomScheme(QString filename){
    std::cout << "Opening file: " << filename.toStdString() << std::endl;
    QFile customSchemeFile(filename);
    if (customSchemeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray customSchemeByteArray = customSchemeFile.readAll();
        return parseCustomScheme(customSchemeByteArray.toStdString());
    } else {
        std::cerr << "Cannot read file!" << std::endl;
        throw std::exception("Cannot read file!");
    }
}

CustomScheme CustomSchemeHandler::parseCustomScheme(std::string scheme_json_string) {
    std::cout << "Open content: " << scheme_json_string << std::endl;
    json scheme_json = json::parse(scheme_json_string);

    CustomScheme result = scheme_json;
    std::cout << "Parsed content: " << result << std::endl;
    return result;
}

void CustomSchemeHandler::saveCustomScheme(QString filename, std::shared_ptr<CustomScheme> custom_scheme) {
    if (custom_scheme) {
        std::cout << "Saving to: " << filename.toStdString() << std::endl;
        QFile saveFile(filename);
        if (saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            json custom_scheme_json = *custom_scheme;
            std::string custom_scheme_string = custom_scheme_json.dump();
            std::cout << "Save content: " << custom_scheme_string << std::endl;

            QTextStream out(&saveFile);
            out << QString(custom_scheme_string.c_str());
        } else {
            std::cerr << "Cannot write file!" << std::endl;
            throw std::exception("Cannot write file!");
        }
    } else {
        std::cerr << "Nothing to save!" << std::endl;
    }
}

std::shared_ptr<CustomScheme> CustomSchemeHandler::getCurrentCustomScheme() {
    return std::shared_ptr<CustomScheme>(current_scheme);
}

void CustomSchemeHandler::setCurrentCustomScheme(CustomScheme custom_scheme) {

    this->current_scheme = std::make_shared<CustomScheme>(custom_scheme);
}

void CustomSchemeHandler::debug() {
    EvenWeights even(0.15, 0.35);

    std::vector<OddWeight> odds;
    odds.push_back(OddWeight(0, 0.5));
    odds.push_back(OddWeight(1, 0.25));

    Weights weights(odds, even);
    CustomScheme s(CustomSchemeMeshType::Tri, CustomSchemeRefinementType::Approx, 1, weights);

    json j = s;

    std::cout << j.dump() << std::endl;

    std::string s_string = j.dump();

    json j2 = json::parse(s_string);

    std::cout << (j == j2) << std::endl;

    CustomScheme s2 = j2;

    std::cout << s2 << std::endl;
}
