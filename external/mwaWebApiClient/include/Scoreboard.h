#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <cstdint>
#include <iostream>

#include "mwaWebApiClient.h"

class Scoreboard : public mwa::WebApiClient
{
public:
    Scoreboard();
    void NewScore(const std::string& mode, const std::string& name, uint32_t minutes, uint32_t seconds, uint32_t milliseconds);
    
    void IncrementCount(const std::string& mode);
    void RefreshCounts();

    void RefreshScores();
    void RefreshNormalScores();
    void RefreshEncoreScores();

private:
    std::string sBasePath;

    void parseScores(std::vector<std::tuple<std::string, uint32_t, uint32_t, uint32_t>>& scores, nlohmann::json& j);

public:
    std::vector<std::tuple<std::string, uint32_t, uint32_t, uint32_t>> vNormalScores;
    std::vector<std::tuple<std::string, uint32_t, uint32_t, uint32_t>> vEncoreScores;
    std::map<std::string, uint64_t> mCounts;

};

#endif
