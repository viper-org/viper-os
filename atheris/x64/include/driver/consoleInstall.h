#ifndef VIPEROS_ATHERIS_X64_DRIVER_CONSOLE_INSTALL_H
#define VIPEROS_ATHERIS_X64_DRIVER_CONSOLE_INSTALL_H 1

namespace x64
{
    constexpr int ConsoleInstallError = 1;

    /*
     * Installs the console with the font
     * provided as a limine module with name
     * 'font.bmp'
     *
     * Returns 0 on success
     * Returns 1 on error
     */
    int InstallConsole();
}

#endif // VIPEROS_ATHERIS_X64_DRIVER_CONSOLE_INSTALL_H