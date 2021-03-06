#pragma once

#include "FileSystem.h"
#include "device.h"
#include "types/vector.h"

class DevFS: public FileSystem {
public:
    static DevFS& the();
    static void initiailize();
    File* open(const Path& path) override;

    virtual bool list_directory(const Path& path, Vector<DirectoryEntry>& res) override;

    virtual bool is_directory(const Path& path) override;
    virtual bool is_file(const Path& path) override;

    virtual bool create_entry(const Path& path, DirectoryEntry::Type type) override;

    void add_device(Device* device);

private:
    DevFS() : FileSystem(Path("/dev")) {}
    Vector<Device*> devices;

};
