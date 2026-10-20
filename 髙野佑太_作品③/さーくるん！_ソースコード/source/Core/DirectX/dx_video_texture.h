#pragma once

struct Render_VideoTexture {
    ID3D11Resource* texture = nullptr;
    ID3D11ShaderResourceView* shader_resource_view = nullptr;
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    uint32_t xres = 0;
    uint32_t yres = 0;
    bool create(uint32_t xres, uint32_t yres, DXGI_FORMAT new_format, bool is_dynamic);
    void activate(int slot) const;
    void destroy();
    bool updateFromIYUV(const uint8_t* new_data, size_t data_size);
};

class VideoClock {
    uint64_t time_stamp;

public:

    VideoClock() {
        time_stamp = timeStamp();
    }

    static uint64_t timeStamp() {
        uint64_t now;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&now);
        return now;
    }

    float elapsed() {
        uint64_t now = timeStamp();
        uint64_t delta_ticks = now - time_stamp;

        LARGE_INTEGER freq;
        if (::QueryPerformanceFrequency(&freq)) {
            float delta_secs = (float)(delta_ticks) / (float)freq.QuadPart;
            time_stamp = now;
            return delta_secs;
        }
        return 0.f;
    }
};

struct VideoTexture {

    static bool createAPI();
    static void destroyAPI();

    struct InternalData;
    InternalData* internal_data = nullptr;

    bool enable() {
        return internal_data != nullptr;
    };

    bool create(const char* filename, bool loop);
    void destroy();
    bool update(float dt);

    void pause();
    void resume();
    bool hasFinished();
    Render_VideoTexture* getTexture();
    float getAspectRatio() const;
};