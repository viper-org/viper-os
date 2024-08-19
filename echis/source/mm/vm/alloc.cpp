#include "atheris/private/mm/vm.h"
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
            void Init()
            {
                auto kernelAddressSpace = atheris::vm::AddressSpace::Kernel();
                atheris::vm::InitAllocator(kernelAddressSpace->freeNodes);
            }

            void* GetPagesImpl(std::list<VMAllocNode>& freeList, std::list<VMAllocatedRegion>& allocated, std::uint32_t pageCount, std::uint16_t flags)
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

                    //MapRegion(addressSpace, pageCount, base, flags);
                }
                else
                {
                    it->numPages -= pageCount;
                    it->base += pageCount * mm::physical::GetPageSize();
                }

                //MapRegion(addressSpace, pageCount, base, flags);
                allocated.push_back({
                    pageCount, base, flags::LazyMapping, flags
                });
                return reinterpret_cast<void*>(base);
            }

            void* GetPages(atheris::vm::AddressSpace* addressSpace, std::uint32_t pageCount, std::uint16_t flags)
            {
                return GetPagesImpl(addressSpace->freeNodes, addressSpace->allocatedRegions, pageCount, flags);
            }

            void* GetKernelPages(std::uint32_t pageCount, std::uint16_t flags)
            {
                return GetPages(atheris::vm::AddressSpace::Kernel(), pageCount, flags);
            }


            bool DoMarkUsed(std::list<VMAllocNode>& freeList, std::uint64_t base, std::uint32_t pageCount)
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

            bool MarkUsedImpl(std::list<VMAllocNode>& freeList, std::list<VMAllocatedRegion>& allocated, std::uint64_t base, std::uint32_t pageCount)
            {
                bool ret = DoMarkUsed(freeList, base, pageCount);
                if (ret)
                {
                    allocated.push_back({
                        pageCount, base, 0, 0
                    });
                }
                return pageCount;
            }

            bool MarkUsed(atheris::vm::AddressSpace* addressSpace, std::uint64_t base, std::uint32_t pageCount)
            {
                return MarkUsedImpl(addressSpace->freeNodes, addressSpace->allocatedRegions, base, pageCount);
            }
            
            bool MarkKernelUsed(std::uint64_t base, std::uint32_t pageCount)
            {
                return MarkUsed(atheris::vm::AddressSpace::Kernel(), base, pageCount);
            }
        }
    }
}