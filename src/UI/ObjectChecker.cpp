#include "ObjectChecker.h"

std::atomic_int ObjectChecker::chunkCount{0};
std::atomic_int ObjectChecker::subChunkCount{0};
std::atomic_int ObjectChecker::renderableCount{0};
std::atomic_int ObjectChecker::meshCount{0};
