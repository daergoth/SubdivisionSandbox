#include "customschemehandler.h"

CustomScheme CustomSchemeHandler::readCustomScheme(std::string filename){
    QString qfilename(filename.c_str());
    QFile customSchemeFile(qfilename);
    if (customSchemeFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray customSchemeByteArray = customSchemeFile.readAll();

        return parseCustomScheme(customSchemeByteArray.toStdString());
    } else {
        throw std::exception("Cannot read file!");
    }
}

CustomScheme CustomSchemeHandler::parseCustomScheme(std::string scheme_json_string) {
    json scheme_json = json::parse(scheme_json_string);
    CustomScheme result = scheme_json;
    return result;
}

void CustomSchemeHandler::saveCustomScheme(std::string filename, CustomScheme customScheme) {

}

void CustomSchemeHandler::debug() {
    EvenWeights even(0.15, 0.35);

    std::vector<OddWeight> odds;
    odds.push_back(OddWeight(0, 0.5));
    odds.push_back(OddWeight(1, 0.25));

    Weights weights(odds, even);
    CustomScheme s(MeshType::Triangular, RefinementType::Approximating, 1, weights);

    json j = s;

    std::cout << j.dump() << std::endl;

    std::string s_string = j.dump();

    json j2 = json::parse(s_string);

    std::cout << ((bool)(j == j2)) << std::endl;

    CustomScheme s2 = j2;

    std::cout << s2 << std::endl;
}
