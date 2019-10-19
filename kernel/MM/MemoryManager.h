#pragma once


#include "multiboot.h"
#include "types.h"
#include "stdlib.h"
#include "bits.h"
#include "Kassert.h"
#include "MM/MM_types.h"
#include "types.h"
#include "PageDirectory.h"

/*
    Virtual Memory map:
    (0MB-4MB - identity mapped)
    3MB - 4MB: kmalloc_eternal
	4MB-PAGE_SIZE...4MB - TempMap page (for a way to access a physical address)

    0xc0000000 - 0xffffffff : kernel address space
    0xc0000000 - 0xc0ffffff : kernel image (code + data)
    0xc1000000 - 0xc5000000 : kernel heap
	0xd0000000 - 0xd0001000 - kernel stack
*/



#define ERR_NO_FREE_FRAMES 1

constexpr u32 TEMPMAP_ADDR = (u32)((4*MB) - PAGE_SIZE);



class MemoryManager {
private:
	MemoryManager(); // singelton
	~MemoryManager();
    void init(multiboot_info_t* mbt);


public:
    static void initialize(multiboot_info_t* mbt);
	static MemoryManager& the();

	VirtualAddress temp_map(PhysicalAddress addr);
	void un_temp_map();
	void allocate(VirtualAddress virt_addr, bool writable, bool user_allowed);
	void flush_tlb(VirtualAddress addr);
	void flush_entire_tlb();
	/**
	 * Returns the Page table Entry for given virtual address
	 * - If create_new=true, and there is no Page Directory Entry for the address,
	 *   a new page table will be created
	 * -  if tempMap_PageTable=true, the Page Table responsible for this address will be temp_mapped to virtual address so it can be accessed.
	 * 
	 * Note: if tempMap_PageTable=true, you should make sure to call un_temp_map after you're done
	 *       manipulating the PTE
	 */
    PTE ensure_pte(VirtualAddress addr, bool create_new_PageTable=true, bool tempMap_pageTable=true);
	PDE get_pde(VirtualAddress virt_addr);
	PTE get_pte(VirtualAddress virt_addr, const PDE& pde);

	Frame get_free_frame(Err&, bool set_used=true);
	void set_frame_used(const Frame& frame);

	void set_frame_available(Frame frame);
	bool is_frame_available(const Frame frame);

	/**
	 * Return address of a new, cloned page directory
	 * The page directory is cloned from the current page directory
	 * Page tables are also clones, but the physical frames they point to are NOT cloned
	 */
	PageDirectory clone_page_directory();

	void copy_from_physical_frame(PhysicalAddress src, u8* dst);
	void copy_to_physical_frame(PhysicalAddress dst, u8* src);
	void memcpy_frames(PhysicalAddress dst, PhysicalAddress src);

private:
	u32 m_frames_avail_bitmap[N_FRAME_BITMAP_ENTRIES];
	PageDirectory* m_page_directory;
	bool m_tempmap_used;

};