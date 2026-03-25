#ifndef BUILD_H
#define BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

// PaperShip version information
extern char gBuildVersion[];
extern unsigned short gBuildVersionMajor;
extern unsigned short gBuildVersionMinor;
extern unsigned short gBuildVersionPatch;

extern char gGitBranch[];
extern char gGitCommitHash[];
extern char gGitCommitTag[];
extern char gBuildDate[];

#ifdef __cplusplus
}
#endif

#endif // BUILD_H
