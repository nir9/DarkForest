#pragma once
#include "types.h"
#include "Window.h"

class GuiManager
{
public:
    static GuiManager& the();

    u32 windowserver_pid() const {return m_windowserver_pid;};
    Window create_window(const u16 width, const u16 height);

    void draw(Window& window);

private:
    GuiManager();

    u32 m_windowserver_pid {0};
};