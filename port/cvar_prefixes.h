#pragma once

// CVar prefix concatenation macros for PaperShip
// The CVAR_PREFIX_* values are set as compile definitions from cmake/papership-cvars.cmake

#define CVAR_COSMETIC(var) CVAR_PREFIX_COSMETIC "." var
#define CVAR_AUDIO(var) CVAR_PREFIX_AUDIO "." var
#define CVAR_CHEAT(var) CVAR_PREFIX_CHEAT "." var
#define CVAR_ENHANCEMENT(var) CVAR_PREFIX_ENHANCEMENT "." var
#define CVAR_SETTING(var) CVAR_PREFIX_SETTING "." var
#define CVAR_WINDOW(var) CVAR_PREFIX_WINDOW "." var
#define CVAR_DEVELOPER_TOOLS(var) CVAR_PREFIX_DEVELOPER_TOOLS "." var
#define CVAR_GENERAL(var) CVAR_PREFIX_GENERAL "." var
#define CVAR_GAMEPLAY_STATS(var) CVAR_PREFIX_GAMEPLAY_STATS "." var
