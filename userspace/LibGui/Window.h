#pragma once
#include "types.h"

class Window final
{

public:
    Window(
        u32 window_id,
        u32 buff_guid,
        void* buff_addr,
        u32 buff_size,
        u32 width,
        u32 height
    );

    u32 id() const {return m_window_id;}
    u32 buff_guid() const {return m_buff_guid;}
    void* buff_addr() const {return m_buff_addr;}
    u32 buff_size() const {return m_buff_size;}
    u32 width() const {return m_width;}
    u32 height() const {return m_height;}

    void clear();
    void update();

    void set_background_color(const u32 color) {m_background_color = color;}

private:
    u32 m_window_id;
    u32 m_buff_guid;
    void* m_buff_addr;
    u32 m_buff_size;
    u32 m_width;
    u32 m_height;

    u32 m_background_color;
};