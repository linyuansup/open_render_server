#include "render_engine.h"

int dbNum = 0;
struct database *dbs = NULL;

int init_engine(const char *vocaloid_path)
{
    std::wstring wstr = std::wstring(vocaloid_path, vocaloid_path + strlen(vocaloid_path));
    std::string err;
    bool result = vocaloid::Init(wstr, err);
    if (!result)
    {
        printf(err.c_str());
        return -1;
    }
    vocaloid::vdm::DatabaseManager &dbManager = vocaloid::GetDatabaseManager();
    dbNum = dbManager.NumVoiceBanks();
    dbs = (struct database *)malloc(sizeof(struct database) * dbNum);
    for (int i = 0; i < dbNum; i++)
    {
        vocaloid::vdm::VoiceBank vb = dbManager.GetVoiceBankByIndex(i);
        dbs[i].id = i;
        dbs[i].lang_id = vb.LangID();
        dbs[i].name = (char *)malloc(sizeof(char) * 256);
        wcstombs(dbs[i].name, vb.Name(), 256);
    }
    return 0;
}

struct database *get_databases()
{
    return dbs;
}

int get_database_count()
{
    return dbNum;
}

int render(const char *input, const char *des)
{
    std::wstring wstr = std::wstring(input, input + strlen(input));
    auto sequence = vocaloid::GetSequenceManager().OpenSequence(
        wstr,
        { vocaloid::vsm::VSMSamplingRate::_44100, 128, 0 });
    if (!sequence) {
        return -1;
    }
    auto tt = sequence.GetTrack(0).GetPart(0);
    return tt.Render(std::wstring(des, des + strlen(des)));
}

void stop()
{
    vocaloid::Destroy();
    free(dbs);
    dbs = NULL;
    dbNum = 0;
}
