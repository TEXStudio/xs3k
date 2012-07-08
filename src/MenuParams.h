#ifndef XSLEDDING_MENUPARAMS_H_INCLUDED
#define XSLEDDING_MENUPARAMS_H_INCLUDED

#include <vector>


static const char* RACETYPE_PROPERTY_SOLO = "solo";
static const char* RACETYPE_PROPERTY_VERSUS = "versus";

static const char* COURSETYPE_PROPERTY_SNOW = "snow";
static const char* COURSETYPE_PROPERTY_ICE = "ice";
static const char* COURSETYPE_PROPERTY_DESERT = "desert";

static const char* SPECIES_PROPERTY_DOG = "dogs";
static const char* SPECIES_PROPERTY_MOUSE = "mice";
static const char* SPECIES_PROPERTY_DINO = "dinos";

static const char* DIFFICULTY_PROPERTY_EASY = "easy";
static const char* DIFFICULTY_PROPERTY_HARD = "hard" ;

static std::vector<std::vector<unsigned int> > OPTION_RESOLUTIONS;

#define PATH_STUFF ""
#define BASIC_ENTITIES_FILENAME "basicSceneEntities.ncf1"
#define	PS3_SHADER_EXTENSION ".cg"

#endif