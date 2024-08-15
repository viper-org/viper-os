#include "mm/vm/node.h"
#include <mm/vm/alloc.h>

#include <mm/physical.h>

#include <algorithm>
#include <lazy_init.h>

namespace echis
{
    namespace vm
    {
        namespace alloc
        {
            static lazy_init<std::list<VMAllocNode>> kernelFreeList;

            void Init()
            {
                kernelFreeList = std::list<VMAllocNode>();
                atheris::vm::InitAllocator(*kernelFreeList);
            }

            static inline void MapRegion(atheris::vm::AddressSpace* addressSpace, std::uint32_t pageCount, std::uint64_t virtualBase, std::uint16_t flags)
            {
                for (std::uint64_t i = 0; i < pageCount * mm::physical::GetPageSize(); i += mm::physical::GetPageSize())
                    {
                        atheris::vm::MapPage(addressSpace,
                            mm::physical::GetPage(),
                            virtualBase + i,
                            flags);
                    }
            }

            void* GetPagesImpl(atheris::vm::AddressSpace* addressSpace, std::list<VMAllocNode>& freeList, std::uint32_t pageCount, std::uint16_t flags)
            {
                if (pageCount == 0)
                {
                    return nullptr;
                }

                // TODO: Find best fit instead of first fit
                auto it = std::find_if(freeList.begin(), freeList.end(), [pageCount](auto node){
                    return node.numPages >= pageCount;
                });

                if (it == freeList.end())
                {
                    return nullptr;
                }

                auto base = it->base;
                if (it->numPages == pageCount)
                {
                    freeList.erase(it);

                    MapRegion(addressSpace, pageCount, base, flags);
                }
                else
                {
                    it->numPages -= pageCount;
                    it->base += pageCount * mm::physical::GetPageSize();
                }

                MapRegion(addressSpace, pageCount, base, flags);
                return reinterpret_cast<void*>(base);
            }

            void* GetPages(atheris::vm::AddressSpace* addressSpace, std::uint32_t pageCount, std::uint16_t flags)
            {
                return GetPagesImpl(addressSpace, addressSpace->nodes, pageCount, flags);
            }

            void* GetKernelPages(std::uint32_t pageCount, std::uint16_t flags)
            {
                return GetPagesImpl(nullptr, *kernelFreeList, pageCount, flags);
            }


            bool MarkUsedImpl(std::list<VMAllocNode>& freeList, std::uint64_t base, std::uint32_t pageCount)
            {
                if (pageCount == 0) return true;

                auto it = std::find_if(freeList.begin(), freeList.end(), [base, pageCount](const auto& node){
                    return base >= node.base && base + pageCount * mm::physical::GetPageSize() <= node.base + node.numPages * mm::physical::GetPageSize();
                });

                if (it == freeList.end()) return false;

                if (base == it->base && base + pageCount * mm::physical::GetPageSize() == it->base + it->numPages * mm::physical::GetPageSize())
                {
                    freeList.erase(it);
                    return true;
                }

                VMAllocNode top;
                top.numPages = (it->base + it->numPages * mm::physical::GetPageSize()) - (base + pageCount * mm::physical::GetPageSize()) / mm::physical::GetPageSize();
                top.base = base + pageCount * mm::physical::GetPageSize();
                freeList.push_back(top);

                it->numPages = (base - it->base) / mm::physical::GetPageSize();

                return true;
            }

            bool MarkUsed(atheris::vm::AddressSpace* addressSpace, std::uint64_t base, std::uint32_t pageCount)
            {
                return MarkUsedImpl(addressSpace->nodes, base, pageCount);
            }
            
            bool MarkKernelUsed(std::uint64_t base, std::uint32_t pageCount)
            {
                return MarkUsedImpl(*kernelFreeList, base, pageCount);
            }
        }
    }
}