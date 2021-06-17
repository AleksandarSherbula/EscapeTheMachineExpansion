#include "Game.h"
#include "TimeAttack.h"

TimeAttack::TimeAttack()
{
    mInputName = "";
    mCanType = true;
    refreshLB = true;
    timeRunning = false;
    m_nStr = 0;

    mStart = std::chrono::system_clock::now();
    
    mCurrentMiliSeconds = 0;
    mCurrentSeconds = 0;
    mCurrentMinutes = 0;

    scoreList = ScoreList::NORMAL;

    mCurseWords =
    {
        "fuck",
        "shit",
        "shiat",
        "sheet",
        "bullshit",
        "ass",
        "anus",
        "cock",
        "cuck",
        "dick",
        "crap",
        "bullcrap",
        "hell",
        "damn",
        "cum",
        "devil",
        "satan",
        "wanker",
        "bollocks",
        "twat",
        "bitch",
        "whore",
        "slut",
        "niger",
        "neeger",
        "nigger",
        "neegger",
        "nigga",
        "niggah",
        "negro",
        "weeb"  // by AlterEgo's request
    };    
}

void TimeAttack::Update()
{     
    mEnd = std::chrono::system_clock::now();

    mCurrentMiliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(mEnd - mStart).count();
    mCurrentSeconds     = std::chrono::duration_cast<std::chrono::seconds>(mEnd - mStart).count();
    mCurrentMinutes     = std::chrono::duration_cast<std::chrono::minutes>(mEnd - mStart).count();

    while (mCurrentMiliSeconds >= 1000)
    {
        mCurrentMiliSeconds -= 1000;
    }

    while (mCurrentSeconds >= 60)
    {
        mCurrentSeconds -= 60;
    }    
}

void TimeAttack::Start()
{
    mStart = std::chrono::system_clock::now();
}

void TimeAttack::PrintTime()
{
   if (game->state != GameState::ENDING)
   {
       game->DrawStringDecal(olc::vi2d(0, 0), GetTimer(),
           olc::WHITE, { 2.0f, 2.0f });
   }
   else
   {       
       game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - (GetTimer().size() / 2) * 24, 24), GetTimer(),
           olc::WHITE, { 3.0f, 3.0f });
   }    
}

bool TimeAttack::FindCurseWord(const std::string& name)
{
    for (int i = 0; i < mCurseWords.size(); i++)
    {
        auto it = std::search(
            name.begin(), name.end(),
            mCurseWords[i].begin(), mCurseWords[i].end(),
            [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
        );

        if (it == name.end() && i == mCurseWords.size() - 1)
            return false;
        else if (it != name.end())
            return true;
    }    
}

void TimeAttack::ClearTimeBuffer()
{
    mTimeScoreBuf.str(std::string());
}

void TimeAttack::PrintLeaderboard(ScoreList sl, const std::string& name, int minutes, int seconds , int miliseconds)
{
    std::string mode = (sl == ScoreList::NORMAL ? "NORMAL" : "ENCORE");
    auto& items = (sl == ScoreList::NORMAL ? game->sb->vNormalScores : game->sb->vEncoreScores); 

    std::vector<olc::Pixel> listColors;

    for (int i = 0; i < items.size(); i++)
        listColors.push_back(olc::WHITE);

    game->DrawStringDecal(olc::vi2d(182, 5), mode, olc::WHITE, { 3.0f, 3.0f });
    for (int i = 0; i < items.size(); i++)
    {
        if (name == std::get<0>(items[i]) &&
            minutes == std::get<1>(items[i]) &&
            seconds == std::get<2>(items[i]) &&
            miliseconds == std::get<3>(items[i]))
        {
            listColors[i] = olc::YELLOW;
        }

        game->DrawStringDecal(olc::vi2d(144, i * 24 + 48), GetTime(items[i]).name, listColors[i]);
        game->DrawStringDecal(olc::vi2d(288, i * 24 + 48), GetTime(items[i]).buffer, listColors[i]);
        
    }
}

void TimeAttack::Reset()
{
    Start();
}

TimeScoreData TimeAttack::GetTime(std::tuple<std::string, uint32_t, uint32_t, uint32_t>& item)
{
    // name             item.get<0>
    // minutes          item.get<1>
    // seconds          item.get<2>
    // milliseconds     item.get<3>
    
    mTimeScoreBuf.str(std::string());

    mTimeScoreBuf << std::setfill('0') << std::setw(2) << std::get<1>(item) << ":";
    mTimeScoreBuf << std::setfill('0') << std::setw(2) << std::get<2>(item) << ".";
    mTimeScoreBuf << std::setfill('0') << std::setw(3) << std::get<3>(item);

    return { std::get<0>(item), mTimeScoreBuf.str() };
}

std::string TimeAttack::GetTimer()
{
    mCurrentTimeScoreBuf.str(std::string());

    mCurrentTimeScoreBuf << std::setfill('0') << std::setw(2) << mCurrentMinutes << ":";
    mCurrentTimeScoreBuf << std::setfill('0') << std::setw(2) << mCurrentSeconds << ".";
    mCurrentTimeScoreBuf << std::setfill('0') << std::setw(3) << mCurrentMiliSeconds;

    return mCurrentTimeScoreBuf.str();
}
