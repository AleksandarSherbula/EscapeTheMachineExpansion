#define MWA_IMPLEMENTATION
#include "mwaWebApiClient.h"
#include "ApiKey.h"

#include "Scoreboard.h"

Scoreboard::Scoreboard()
{
    SetHost(API_URL);

    SetSecret(API_KEY);

    sBasePath = API_BASE_PATH;
}

void Scoreboard::NewScore(const std::string& mode, const std::string& name, uint32_t minutes, uint32_t seconds, uint32_t milliseconds)
{
    nlohmann::json j;
    j["name"] = name;
    j["minutes"] = minutes;
    j["seconds"] = seconds;
    j["milliseconds"] = milliseconds;

    Post(sBasePath + "/scores/" + mode, j.dump());
}

void Scoreboard::IncrementCount(const std::string& mode)
{
    Post(sBasePath + "/count/" + mode);
}

void Scoreboard::RefreshCounts()
{
    nlohmann::json j = nlohmann::json::parse(Get(sBasePath + "/count"));

    for (auto& item : j.items())
    {
        mCounts[item.key()] = item.value();
    }
}

void Scoreboard::RefreshScores()
{
    RefreshNormalScores();
    RefreshEncoreScores();
}

void Scoreboard::RefreshNormalScores()
{
    nlohmann::json j = nlohmann::json::parse(Get(sBasePath + "/scores/normal"));
    parseScores(vNormalScores, j);
}

void Scoreboard::RefreshEncoreScores()
{
    nlohmann::json j = nlohmann::json::parse(Get(sBasePath + "/scores/encore"));
    parseScores(vEncoreScores, j);
}

void Scoreboard::parseScores(std::vector<std::tuple<std::string, uint32_t, uint32_t, uint32_t>>& scores, nlohmann::json& j)
{
    scores.clear();
    if (!j.empty())
    {
        for (auto& item : j)
        {
            scores.push_back(std::make_tuple(
                item["name"].get<std::string>(),
                item["minutes"].get<uint32_t>(),
                item["seconds"].get<uint32_t>(),
                item["milliseconds"].get<uint32_t>()
            ));
        }
    }
}