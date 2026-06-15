// ===================================================
// movie.h ui制御
// 
// hara sougo  2025/02/19
// ===================================================

#ifndef _MOVIE_H_
#define _MOVIE_H_


// ===================================================
// 列挙体宣言
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

#ifdef IS_GAME_MANAGER
void InitMovie();
void UninitMovie();
void UpdateMovie();
void DrawMovie();
#endif

bool LoadMovieSingle(
    std::string path/*,
    std::function<void(void)> afterFunc*/
);
bool LoadMovie(
    std::string path, bool loop = false,
    std::string afterMovPath = {}, bool afterMovLoop = false
);
void UnloadMovie();

#endif