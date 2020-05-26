#pragma once
#include "../World/Level.h"

class LevelManager {
   public:
    LevelManager();

   private:
    Level* m_current_level;
};
