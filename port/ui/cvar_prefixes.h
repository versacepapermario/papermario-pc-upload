#pragma once

/**
 * CVar prefix helper macros for PaperShip
 * These create full CVar paths from the cmake-defined prefixes.
 */

// Helper macro to concatenate CVar prefix with suffix
#define CVAR_CONCAT_(prefix, suffix) prefix "." suffix
#define CVAR_CONCAT(prefix, suffix) CVAR_CONCAT_(prefix, suffix)

// Game-specific CVar accessors
#define CVAR_ENHANCEMENT(name) CVAR_CONCAT(CVAR_PREFIX_ENHANCEMENT, name)
#define CVAR_CHEAT(name) CVAR_CONCAT(CVAR_PREFIX_CHEAT, name)
#define CVAR_SETTING(name) CVAR_CONCAT(CVAR_PREFIX_SETTING, name)
#define CVAR_COSMETIC(name) CVAR_CONCAT(CVAR_PREFIX_COSMETIC, name)
#define CVAR_AUDIO(name) CVAR_CONCAT(CVAR_PREFIX_AUDIO, name)
#define CVAR_DEVELOPER_TOOLS(name) CVAR_CONCAT(CVAR_PREFIX_DEVELOPER_TOOLS, name)
#define CVAR_GENERAL(name) CVAR_CONCAT(CVAR_PREFIX_GENERAL, name)
#define CVAR_WINDOW(name) CVAR_CONCAT(CVAR_PREFIX_WINDOW, name)
#define CVAR_GAMEPLAY_STATS(name) CVAR_CONCAT(CVAR_PREFIX_GAMEPLAY_STATS, name)
