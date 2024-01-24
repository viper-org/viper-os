void libc_alloc_init();
int main();

int libc_start_main()
{
    libc_alloc_init();

    return main();
}