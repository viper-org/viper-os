#include "atheris/private/mm/vm.h"
#include <mm/vmm.h>
#include <mm/pmm.h>

namespace echis
{
    namespace vm
    {
        void* GetPages(atheris::vm::AddressSpace* addressSpace, uint64_t count, uint16_t flags)
        {
            if (count == 0)
            {
                return nullptr;
            }

            flags |=  atheris::vm::flags::lazy;
            flags &= ~atheris::vm::flags::present;

            const auto removeFromFreeList = [addressSpace](atheris::vm::VMNode* current, atheris::vm::VMNode* previous) {
                if (previous)
                {
                    previous->next = current->next;
                }
                else
                {
                    addressSpace->freeList = current->next;
                }
            };

            atheris::vm::VMNode* current  = addressSpace->freeList;
            atheris::vm::VMNode* previous = nullptr;
            while (current != nullptr)
            {
                if (current->pages >= count)
                {
                    if (current->pages == count)
                    {
                        removeFromFreeList(current, previous);
                        uint64_t addr = current->base;
                        delete current;
                        atheris::vm::MapPages(addressSpace, 0, addr, flags, count);
                        return reinterpret_cast<void*>(addr);
                    }

                    removeFromFreeList(current, previous);
                    uint64_t addr = current->base;

                    current->pages -= count;
                    current->base += count * pmm::GetPageSize();

                    current->next = addressSpace->freeList;
                    addressSpace->freeList = current;

                    atheris::vm::MapPages(addressSpace, 0, addr, flags, count);
                    return reinterpret_cast<void*>(addr);
                }

                previous = current;
                current = current->next;
            }
            return nullptr; // TODO: OOM
        }

        void FreePages(atheris::vm::AddressSpace* addressSpace, void* pages, uint64_t count)
        {
            if (pages == nullptr || count == 0)
            {
                return;
            }

            // TODO: Free physical pages and unmap

            atheris::vm::VMNode* newNode = new atheris::vm::VMNode;
            newNode->base  = reinterpret_cast<uint64_t>(pages);
            newNode->pages = count;
            newNode->next  = addressSpace->freeList;

            addressSpace->freeList = newNode;
        }
    }
}