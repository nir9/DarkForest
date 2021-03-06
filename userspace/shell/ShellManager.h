#pragma once

#include "types/String.h"
#include "types/vector.h"

class ShellManager
{
public:
    ShellManager();

public:
    void process_command(const String& command);

private:
    void do_ls(const Vector<String>& cmd_parts);
    void do_echo(const Vector<String>& cmd_parts);
    void do_cd(const Vector<String>& cmd_parts);
    void do_touch(const Vector<String>& cmd_parts);
    void do_mkdir(const Vector<String>& cmd_parts);
    void print_prompt();

    bool find_app(const String& name, String& app_path);

private:
    String m_current_directory;
};

