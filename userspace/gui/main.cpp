#include "unistd.h"
#include "stdio.h"
#include "asserts.h"
#include "mman.h"
#include "string.h"
#include "malloc.h"
#include "types/vector.h"
#include "types/String.h"
#include "kernel/errs.h"
#include "ioctl_common.h"
#include "asserts.h"

constexpr u32 SHM_GUID = 123;

constexpr char VGA_PATH[] = "/dev/vga";

u32 vga_width;
u32 vga_height;
u32 vga_pitch;

void* create_framebuffer()
{
    int vga_fd = std::open(VGA_PATH);
    ASSERT(vga_fd >= 0);
    IOCTL::VGA::Data data = {};
    const int rc = std::ioctl(vga_fd, static_cast<u32>(IOCTL::VGA::Code::GET_DIMENSIONS), &data);
    ASSERT(rc == E_OK);

    vga_width = data.width;
    vga_height = data.height;
    vga_pitch = data.pitch;
    const u32 size = vga_pitch*vga_height;

    void* addr = 0;
    const int rc2 = std::create_shared_memory(SHM_GUID, size, addr);
    kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~`");
    ASSERT(rc2 == E_OK);
    kprintf("gui: shared mem: 0x%x\n", addr);

    return addr;
}

// void query_shared_mem()
// {
//     void* addr = 0;
//     u32 size = 0;
//     const int rc = std::open_shared_memory(SHM_GUID, addr, size);
//     ASSERT(rc == E_OK);
//     kprintf("shell: shared mem: 0x%x\n", addr);
//     kprintf("shell: shared mem char: %d\n", reinterpret_cast<char*>(addr)[0]);
// }

int main() {
    printf("gui!\n");
    std::sleep_ms(1000);
    u32 pid = 0;
    const int rc = std::get_pid_by_name("WindowServer", pid);
    ASSERT(rc == E_OK);
    kprintf("WindowServer pid: %d\n", pid);

    u32* frame_buffer = reinterpret_cast<u32*>(create_framebuffer());

    u32 x = 700;
    u32 y = 200;
    u32 width = 100;
    u32 height = 200;

    for(size_t i = 0;;++i)
    {
        memset(frame_buffer, 0, vga_height*vga_pitch);
        for(size_t row = y; row < y+height; ++row)
        {
            for(size_t col = x; col < x+width; ++col)
            {
                u32* pixel = frame_buffer + (row*vga_width + col);
                *pixel = (0xdeadbeef*i);
            }
        }

        kprintf("sent message\n");
        const int msg_rc = std::send_message(pid, (u32)SHM_GUID);
        ASSERT(msg_rc == E_OK);
        std::sleep_ms(500);
    }

    return 0;
}