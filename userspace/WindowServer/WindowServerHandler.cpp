#include "WindowServerHandler.h"
#include "unistd.h"
#include "LibWindowServer/IPC.h"
#include "stdio.h"

void WindowServerHandler::run()
{
    while(true)
    {
        u32 code = 0;
        u32 pid = 0;
        kprintf("waiting for message..\n");
        const int rc = std::get_message((char*)&code, sizeof(u32), pid);
        ASSERT(rc == sizeof(u32));
        handle_message_code(code, pid);
    }
}


void WindowServerHandler::handle_message_code(u32 code, u32 pid)
{
    (void)pid;

    kprintf("handle_message_code: %d\n", code);

    switch(code)
    {
       case WindowServerIPC::Code::CreateWindowRequest:
       {
           WindowServerIPC::CreateWindowRequest request;

            bool rc;
            rc = WindowServerIPC::recv_create_window_request(pid, request, false);
            ASSERT(rc);

            const Window w(request);

            WindowServerIPC::CreateWindowResponse response = {w.id(), w.buff_guid()};
            rc = WindowServerIPC::send_create_window_response(pid, response);
            ASSERT(rc);

            m_windows.append(w);
           break;
       } 

       case WindowServerIPC::Code::DrawWindow:
       {

           kprintf("draw window\n");
           WindowServerIPC::DrawWindow request;
           const bool rc = WindowServerIPC::recv_draw_request(pid, request, false);
           ASSERT(rc);

           Window window = get_window(request.window_guid);
           m_vga.draw((u32*)window.buff_addr(), window.x(), window.y(), window.width(), window.height());
           break;
       }

       default:
        {
           ASSERT_NOT_REACHED();
        }
    }
}

Window WindowServerHandler::get_window(u32 window_id)
{
    for(auto& window : m_windows)
    {
        if(window.id() == window_id)
        {
            return window;
        }
    }
    ASSERT_NOT_REACHED();
}