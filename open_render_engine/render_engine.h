#ifndef OPEN_RENDER_ENGINE_RENDER_ENGINE_H
#define OPEN_RENDER_ENGINE_RENDER_ENGINE_H

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "v5sdk/include/VDM/DatabaseManager.h"
#include "v5sdk/include/VDM/VoiceBank.h"
#include "v5sdk/include/VSM/WIVSMSequenceManager.h"
#include "v5sdk/include/VocaloidCore.h"

struct database
{
	char* name;
	int id;
	int lang_id;
};

int init_engine(const char* vocaloid_path);
struct database* get_databases();
int get_database_count();
int render(const char* input, const char* des);

#endif